#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

class Branches:
    def __init__ (self): pass

    def VarTitle (self, var):
        if var == 'evt':          title = 'Event Number'
        elif var == 'cap':        title = 'Capacitance'
        elif var == 'charge':     title = 'Charge'
        elif var == 'ped':        title = 'Pedestal Bias'
        elif var == 'samples':    title = 'Sample Values'
        elif var == 'saturation': title = 'Saturation Count'
        elif var == 'null':       title = 'Null Count'
        elif var == 'crc':        title = 'CRC Count'
        elif var == 'hfmean':     title = 'HF Mean'
        elif var == 'hfstd':      title = 'HF StdDev'
        elif var == 'pedestal':   title = 'Pulse Pedestal'
        elif var == 'height':     title = 'Pulse Height'
        elif var == 'phase':      title = 'Pulse Phase'
        elif var == 'width':      title = 'Pulse Width'
        elif var == 'prob':       title = 'Pulse Prob'
        elif var == 'fastfit':    title = 'Fast Fit'
        return title

    def VarAlg (self, var):
        if var == 'evt':          name = 'EvtRange'
        elif var == 'cap':        name = 'CapRange'
        elif var == 'charge':     name = 'ChargeRange'
        elif var == 'ped':        name = 'PedRange'
        elif var == 'samples':    name = 'SampleRange'
        elif var == 'saturation': name = 'Saturation'
        elif var == 'null':       name = 'NullValue'
        elif var == 'crc':        name = 'CRCError'
        elif var == 'hfmean':     name = 'SampleHF'
        elif var == 'hfstd':      name = 'SampleHF'
        elif var == 'pedestal':   name = 'PulseFit'
        elif var == 'height':     name = 'PulseFit'
        elif var == 'phase':      name = 'PulseFit'
        elif var == 'width':      name = 'PulseFit'
        elif var == 'prob':       name = 'PulseFit'
        elif var == 'fastfit':    name = 'FastFit'
        return name

    def VarRange (self, var):
        if var == 'evt':          name = 'EvtRange'
        elif var == 'cap':        name = 'CapRange'
        elif var == 'charge':     name = 'ChargeRange'
        elif var == 'ped':        name = 'PedRange'
        elif var == 'samples':    name = 'SampleRange'
        elif var == 'saturation': name = 'Saturation'
        elif var == 'null':       name = 'Null'
        elif var == 'crc':        name = 'CRCError'
        elif var == 'hfmean':     name = 'HFMean'
        elif var == 'hfstd':      name = 'HFStd'
        elif var == 'pedestal':   name = 'Pedestal'
        elif var == 'height':     name = 'Height'
        elif var == 'phase':      name = 'Phase'
        elif var == 'width':      name = 'Width'
        elif var == 'prob':       name = 'Prob'
        elif var == 'fastfit':    name = 'FastFit'
        return name

    def VarSteps (self, var):
        if var == 'cap':      name = 'CapSteps'
        elif var == 'charge': name = 'ChargeSteps'
        elif var == 'ped':    name = 'PedSteps'
        else:                 name = ''
        return name

    def VarChannel (self, var):
        if var in ['cap', 'charge']: channel = False
        else: channel = True
        return channel

    def VarArr (self, var, gain, pmt):
        name = var
        if var in ['ped', 'samples']:
            if gain: name += '_hi'
            else: name += '_lo'
            name += '[%i]' % pmt
            pass
        elif var in ['crc', 'hfmean', 'hfstd',
                     'pedestal', 'height', 'phase', 'width', 'prob',
                     'fastfit']:
            name += '[%i][%i]' % (gain, pmt)
            pass
        return name

    def VarRaw (self, var):
        if var == 'evt':          raw = True
        elif var == 'cap':        raw = True
        elif var == 'charge':     raw = True
        elif var == 'ped':        raw = True
        elif var == 'samples':    raw = True
        elif var == 'saturation': raw = False
        elif var == 'null':       raw = False
        elif var == 'crc':        raw = False
        elif var == 'hfmean':     raw = False
        elif var == 'hfstd':      raw = False
        elif var == 'pedestal':   raw = False
        elif var == 'height':     raw = False
        elif var == 'phase':      raw = False
        elif var == 'width':      raw = False
        elif var == 'prob':       raw = False
        elif var == 'fastfit':    raw = False
        return raw
    pass
