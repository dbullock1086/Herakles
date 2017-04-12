#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

import time, subprocess, Herakles

class IPbus:
    def __init__ (self, host, verbose=False):
        self.host       = host
        self.verbose    = verbose
        self.delayShort = 0.001
        self.delayLong  = 0.03
        pass

    def _getnspec(self):
        rb = self.Read (0x9f)
        nsamp = rb & 0xff
        nchan = (rb>>8) & 0xff
        pass

    def _dummycheck(self):
        self.Write (0x0, 77)
        rb = self.Read (0x0)
        if rb == 77: return True
        return False

    def _mapping (self, pmt):
        if pmt < 0:
            md = abs(pmt) - 1
            fpga = 4
            card = 3
            pass
        else:
            md = pmt/12
            fpga = ((pmt%2)<<1) + (1-(pmt/6)%2)
            card = 2 - ((pmt/2) % 3)
            pass
        return [md, fpga, card]

    def Connect (self):
        print 'IPbus: connect to host', self.host
        pingtest = subprocess.Popen (['ping', '-c1', self.host],
                                     stdout=subprocess.PIPE).communicate()[0]
        if float (pingtest.split('% package loss')[0].split()[-1]):
            print 'IPbus: unable to connect to IP address', self.host
            quit ()
            pass
        self.ipbus = Herakles.Uhal (self.host, 50001)
        self.ipbus.SetVerbose (self.verbose)
        self.Sync ()
        if not self._dummycheck ():
            print 'IPbus: unable to write/read dummy register'
            quit ()
            pass
        pass

    def IPRead (self, add, num=1): return self.ipbus.Read (add, num)

    def IPWrite (self, add, vals): self.ipbus.Write (add, vals)

    def IPSync (self): self.ipbus.Sync ()

    def SyncClear (self): self.IPWrite (0x6, 0x1)

    def SyncRest (self): self.IPWrite (0x6, 0x0)

    def SyncEnable (self): self.IPWrite (0x6, 0x2)

    def SyncLoop (self, num):
        self.IPWrite (0x6, num<<2|0x2)
        time.sleep (self.delayLong)
        pass

    def SyncWrite (self, bcid, dba, val):
        self.IPWrite (0x10000, [bcid<<16|dba,val])
        time.sleep (self.delayShort)
        pass

    def SyncFlush (self, bcid, dba):
        self.SyncWrite (bcid, dba, 0x8<<28)
        time.sleep (self.delayLong)
        pass

    def AsyncWrite (self, dba, val):
        self.IPWrite (0x10002, [dba,val])
        time.sleep (self.delayShort)
        pass

    def AsyncFlush (self):
        self.AsyncWrite (0x1, 0x8<<24)
        time.sleep (self.delayLong)
        pass

    def RODRead (self, gain, pmt, num):
        addr = 0x100 * (pmt+1)
        if not gain: addr += 0x3000
        return self.IPRead (addr, num)

    def RODWrite (self, bcid, val):
        self.IPWrite (0x10004, [bcid<<16,val])
        time.sleep (self.delayShort)
        pass

    def RODReadInteg (self, pmt):
        addr = 0x20128 + (pmt<<4)
        return self.IPRead (addr+5) & 0xffff

    def TTCRead (self, pmt, setting):
        [md, fpga, card] = self._mapping (pmt)
        self.AsyncWrite (0x1,
                         (1<<22)+(fpga<<18)+(card<<16)+(setting<<12)+0x111)
        self.AsyncFlush ()
        addr = 0x10021 + 0x10*pmt
        return self.IPRead ((md<<12)+addr) & 0xfff

    def TTCWrite (self, pmt, setting, data=0):
        [md, fpga, card] = self._mapping (pmt)
        self.AsyncWrite (0x1, (1<<22)+(fpga<<18)+(card<<16)+(setting<<12)+data)
        self.AsyncFlush ()
        pass

    def IntegConfig (self, freq): self.AsyncWrite (0x115, freq)

    def IntegWrite (self, pmt, tph, tpl, integEna, integGain, tena):
        cmd = (tph<<10) + (tpl<<8) + (integEna<<7) + (integGain<<3) + (tena<<2)
        self.TTCWrite (pmt, setting, cmd)
        pass

    def DACRead (self, pmt, gain):
        for setting in xrange(0x8+(2*gain), 0xa+(2*gain)):
            self.TTCWrite (setting, 0x111)
            pass
        rb = self.IPRead (0x10022+(pmt*0x10) + 2*(1-gain), 2)
        return [a & 0xfff for a in rb]

    def DACWrite (self, pmt, bias, gain):
        plus = 1278 + int(bias*983./4096.)
        minus = 2261 - int(bias*983./4096.+.5)
        print 'IPbus: calibrate bias', hex(bias), 'using', hex(plus), hex(minus)
        self.TTCWrite (0x4+(2*gain), plus)
        self.TTCWrite (0x5+(2*gain), minus)
        self.TTCWrite (0xc+gain, 0xa0)
        rb = self.DACRead (pmt, gain)
        if rb[0] != plus:
            print '    PMT', pmt+1, 'gain', gain, 'plus is not set!', hex(rb[0])
            pass
        if rb[1] != minus:
            print '    PMT', pmt+1, 'gain', gain, 'minus is not set!', hex(rb[1])
            pass
        pass

    def HVReset (self, pmt, setting=0x44):
        self.IPWrite (0x21000, ((setting<<24)+(pmt<<20)+((1-(pmt%2))<<16)))
        pass

    def HVWrite (self, pmt, data):
        self.IPWrite (0x21010+pmt, data)
        self.HVReset (pmt, 0x48)
        pass

    def HVWriteOffset (self, pmt, data): self.IPWrite (0x21040+pmt, data)

    def HVReadVoltage (self, pmt, num=1): return self.IPRead (0x21110+pmt, num)

    def HVReadTemp (self, pmt):
        [md, fpga, card] = self._mapping (pmt)
        return self.IPRead (0x10015 + 0x7*(fpga%2) + (fpga/2)*0x1000)
    pass
