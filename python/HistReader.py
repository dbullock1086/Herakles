#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *

class HistReader (object):
    def OpenFile (self, fname, option='read'):
        #### open the eventloop output file
        self.rfile = ROOT.TFile.Open (fname, option)
        pass

    def InitRange (self):
        hist = self.rfile.Get ('EvtRange')
        self.evtmin = hist.GetBinContent (1)
        self.evtmax = hist.GetBinContent (2)
        self.nevts = self.evtmin - self.evtmax
        hist.IsA().Destructor (hist)
        pass

    def GetRange (self, var, gain=0, pmt=0):
        #### get the number of bins and range for a particular variable
        #    for a particular gain and channel
        if var == 'evt':
            minval = self.evtmin
            maxval = self.evtmax
            steps = int(ROOT.TMath.Sqrt (self.nevts))
            pass

        elif var == 'cap':
            hist = self.rfile.Get ('CapRange')
            minval = hist.GetBinContent (1)
            maxval = hist.GetBiNContent (2)
            hist.IsA().Destructor (hist)
            hist = self.rfile.Get ('CapSteps')
            steps = hist.GetBinContent (1)
            hist.IsA().Destructor (hist)
            pass

        elif var == 'charge':
            hist = self.rfile.Get ('ChargeRange')
            minval = hist.GetBinContent (1)
            maxval = hist.GetBinContent (2)
            hist.IsA().Destructor (hist)
            hist = self.rfile.Get ('ChargeSteps')
            steps = hist.GetBinContent (1)
            hist.IsA().Destructor (hist)
            pass

        elif var == 'ped':
            if gain:
                hist = self.rfile.Get ('PedRange_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('PedRange_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('PedSteps_lo')
                steps = hist.GetBinContent (pmt + 1)
                hist.IsA().Destructor (hist)
                pass
            else:
                hist = self.rfile.Get ('PedRange_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('PedRange_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('PedSteps_hi')
                steps = hist.GetBinContent (pmt + 1)
                hist.IsA().Destructor (hist)
                pass
            pass

        elif var == 'samples':
            if gain:
                hist = self.rfile.Get ('SampleRange_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('SampleRange_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('SampleRange_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('SampleRange_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'crc':
            if gain:
                hist = self.rfile.Get ('CRCError_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('CRCError_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('CRCError_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('CRCError_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'fastfit':
            if gain:
                hist = self.rfile.Get ('FastFit_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('FastFit_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('FastFit_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('FastFit_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'fastratio':
            if gain:
                hist = self.rfile.Get ('FastRatio_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('FastRatio_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('FastRatio_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('FastRatio_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'pedratio':
            if gain:
                hist = self.rfile.Get ('PedRatio_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('PedRatio_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('PedRatio_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('PedRatio_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'pedestal':
            if gain:
                hist = self.rfile.Get ('Pedestal_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Pedestal_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('Pedestal_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Pedestal_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'height':
            if gain:
                hist = self.rfile.Get ('Height_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Height_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('Height_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Height_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'phase':
            if gain:
                hist = self.rfile.Get ('Phase_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Phase_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('Phase_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Phase_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'width':
            if gain:
                hist = self.rfile.Get ('Width_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Width_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('Width_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('Width_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'chisqr':
            if gain:
                hist = self.rfile.Get ('ChiSqr_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('ChiSqr_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('ChiSqr_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('ChiSqr_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'ndf':
            if gain:
                hist = self.rfile.Get ('NDF_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('NDF_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('NDF_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('NDF_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'chgratio':
            if gain:
                hist = self.rfile.Get ('ChgRatio_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('ChgRatio_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('ChgRatio_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('ChgRatio_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'hfmean':
            if gain:
                hist = self.rfile.Get ('HFMean_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('HFMean_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('HFMean_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('HFMean_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        elif var == 'hfstd':
            if gain:
                hist = self.rfile.Get ('HFStd_lo_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('HFStd_lo_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            else:
                hist = self.rfile.Get ('HFStd_hi_min')
                hist.IsA().Destructor (hist)
                minval = hist.GetBinContent (pmt + 1)
                hist = self.rfile.Get ('HFStd_hi_max')
                hist.IsA().Destructor (hist)
                maxval = hist.GetBinContent (pmt + 1)
                steps = int(ROOT.TMath.Sqrt (self.nevts))
                pass
            pass

        if steps >= 200: steps = 200
        valsize = maxval - minval
        binsize = valsize / (steps - 1)
        maxval += binsize
        return [steps, minval, maxval]

    def _own (self, dname, tname, title):
        self.owned[dname] = self.rfile.Get (tname)
        self.owned[dname].SetDirectory (0)
        self.owned[dname].SetTitle (title)
        pass

    def OwnHist (self, name):
        if name == 'ped':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'PedRange_hi_min'
                    title = '%s Pedestal Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'PedRange_hi_max'
                    title = '%s Pedestal Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'PedRange_lo_min'
                    title = '%s Pedestal Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'PedRange_lo_max'
                    title = '%s Pedestal Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif name == 'samples':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'SampleRange_hi_min'
                    title = '%s Sample Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'SampleRange_hi_max'
                    title = '%s Sample Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'SampleRange_lo_min'
                    title = '%s Sample Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'SampleRange_lo_max'
                    title = '%s Sample Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'crc':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'CRCError_hi_min'
                    title = '%s CRC Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'CRCError_hi_max'
                    title = '%s CRC Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'CRCError_lo_min'
                    title = '%s CRC Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'CRCError_lo_max'
                    title = '%s CRC Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'fastfit':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'FastFit_hi_min'
                    title = '%s Fast Integral Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'FastFit_hi_max'
                    title = '%s Fast Integral Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'FastFit_lo_min'
                    title = '%s Fast Integral Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'FastFit_lo_max'
                    title = '%s Fast Integral Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'fastratio':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'FastRatio_hi_min'
                    title = '%s Fast Integral/Charge Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'FastRatio_hi_max'
                    title = '%s Fast Integral/Charge Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'FastRatio_lo_min'
                    title = '%s Fast Integral/Charge Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'FastRatio_lo_max'
                    title = '%s Fast Integral/Charge Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'pedratio':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'PedRatio_hi_min'
                    title = '%s Sample/Pedestal Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'PedRatio_hi_max'
                    title = '%s Sample/Pedestal Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'PedRatio_lo_min'
                    title = '%s Sample/Pedestal Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'PedRatio_lo_max'
                    title = '%s Sample/Pedestal Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'pedestal':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'Pedestal_hi_min'
                    title = '%s Pulse Pedestal Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'Pedestal_hi_max'
                    title = '%s Pulse Pedestal Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'Pedestal_lo_min'
                    title = '%s Pulse Pedestal Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'Pedestal_lo_max'
                    title = '%s Pulse Pedestal Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'height':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'Height_hi_min'
                    title = '%s Pulse Height Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'Height_hi_max'
                    title = '%s Pulse Height Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'Height_lo_min'
                    title = '%s Pulse Height Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'Height_lo_max'
                    title = '%s Pulse Height Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'phase':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'Phase_hi_min'
                    title = '%s Pulse Phase Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'Phase_hi_max'
                    title = '%s Pulse Phase Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'Phase_lo_min'
                    title = '%s Pulse Phase Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'Phase_lo_max'
                    title = '%s Pulse Phase Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'width':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'Width_hi_min'
                    title = '%s Pulse Width Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'Width_hi_max'
                    title = '%s Pulse Width Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'Width_lo_min'
                    title = '%s Pulse Width Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'Width_lo_max'
                    title = '%s Pulse Width Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'chisqr':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'ChiSqr_hi_min'
                    title = '%s Pulse Chi2 Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'ChiSqr_hi_max'
                    title = '%s Pulse Chi2 Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'ChiSqr_lo_min'
                    title = '%s Pulse Chi2 Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'ChiSqr_lo_max'
                    title = '%s Pulse Chi2 Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'ndf':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'NDF_hi_min'
                    title = '%s Pulse NDF Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'NDF_hi_max'
                    title = '%s Pulse NDF Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'NDF_lo_min'
                    title = '%s Pulse NDF Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'NDF_lo_max'
                    title = '%s Pulse NDF Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'chgratio':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'ChgRatio_hi_min'
                    title = '%s Pulse Height/Charge Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'ChgRatio_hi_max'
                    title = '%s Pulse Height/Charge Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'ChgRatio_lo_min'
                    title = '%s Pulse Height/Charge Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'ChgRatio_lo_max'
                    title = '%s Pulse Height/Charge Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'hfmean':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'HFMean_hi_min'
                    title = '%s HF Mean Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'HFMean_hi_max'
                    title = '%s HF Mean Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'HFMean_lo_min'
                    title = '%s HF Mean Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'HFMean_lo_max'
                    title = '%s HF Mean Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'hfstd':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'HFStd_hi_min'
                    title = '%s HF Std Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'HFStd_hi_max'
                    title = '%s HF Std Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'HFStd_lo_min'
                    title = '%s HF Std Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'HFStd_lo_max'
                    title = '%s HF Std Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'hfcorr':
            dname = name + '_HFCorr'
            tname = 'CorrHF'
            title = '%s HF Correlation' % self.name
            self._own (dname, tname, title)
            pass

        elif var == 'lfmean':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'LFMean_hi_min'
                    title = '%s LF Mean Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'LFMean_hi_max'
                    title = '%s LF Mean Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'LFMean_lo_min'
                    title = '%s LF Mean Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'LFMean_lo_max'
                    title = '%s LF Mean Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass

        elif var == 'lfstd':
            for gain in self.gains:
                if gain:
                    dname = name + '_hi_min'
                    tname = 'LFStd_hi_min'
                    title = '%s LF Std Gain1 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_hi_max'
                    tname = 'LFStd_hi_max'
                    title = '%s LF Std Gain1 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                else:
                    dname = name + '_lo_min'
                    tname = 'LFStd_lo_min'
                    title = '%s LF Std Gain0 Min' % self.name
                    self._own (dname, tname, title)
                    dname = name + '_lo_max'
                    tname = 'LFStd_lo_max'
                    title = '%s LF Std Gain0 Max' % self.name
                    self._own (dname, tname, title)
                    pass
                pass
            pass
        pass
    pass

    def CloseFile (self):
        # always be tidy
        self.rfile.Close ()
        pass
    pass
