#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.comdbullock1086/Herakles

class Nomenclature (object):
    def VarTitle (self, var):
        #### get a descriptive title for the variable
        if var == 'cap': title = 'Capacitance'
        elif var == 'charge': title = 'Charge'
        elif var == 'ped': title = 'Bias'
        elif var == 'samples': title = 'Samples'
        elif var == 'crc': title = 'Error Count'
        elif var == 'pedratio': title = 'Sample/Pedestal'
        elif var == 'hfmean': title = 'HF Mean'
        elif var == 'hfstd': title = 'HF Std'
        elif var == 'pedestal': title = 'Pulse Pedestal'
        elif var == 'height': title = 'Pulse Height'
        elif var == 'phase': title = 'Pulse Phase'
        elif var == 'width': title = 'Pulse Width'
        elif var == 'chisqr': title = 'Pulse Chi2'
        elif var == 'ndf': title = 'Pulse NDF'
        elif var == 'chgratio': title = 'Pulse Height/Charge'
        elif var == 'fastfit': title = 'Fast Integral'
        elif var == 'fastratio': title = 'Fast Ratio'
        pass

    def VarELName (self, var, gain, pmt):
        if var == 'evt': name = 'EvtRange'
        elif var == 'cap': name = 'CapRange'
        elif var == 'charge': name = 'ChargeRange'
        elif var == 'ped': name = 'PedRange'
        elif var == 'samples': name = 'SampleRange'
        elif var == 'crc': name = 'CRCError'
        elif var == 'fastfit': name = 'FastFit'
        elif var == 'fastratio': name = 'FastRatio'
        elif var == 'pedratio': name = 'PedRatio'
        elif var == 'Pedestal': name = 'Pedestal'
        elif var == 'height': name = 'Height'
        elif var == 'phase': name = 'Phase'
        elif var == 'width': name = 'Width'
        elif var == 'chisqr': name = 'ChiSqr'
        elif var == 'ndf': name = 'NDF'
        elif var == 'chgratio': name = 'ChgRatio'
        elif var == 'hfmean': name = 'hfmean'
        elif var == 'hfstd': name = 'hfstd'
        pass

    def VarMinMax (self, var):
        if var in ['cap', 'charge', 'hfcorr']: minmax = False
        else: minmax = True
        return minmax

    def VarSteps (self, var):
        if var == 'cap': name = 'CapSteps'
        elif var == 'charge': name = 'ChargeSteps'
        elif var == 'ped': name = 'PedSteps'
        else: name = ''
        pass

    def ArrName (self, var, gain, pmt):
        name = var
        if var in ['ped', 'samples']:
            if gain: name += '_hi'
            else: name += '_lo'
            name += '[%i]' % pmt
            pass
        elif var in ['crc', 'fastfit', 'fastratio', 'pedratio',
                     'pedestal', 'height', 'phase', 'width',
                     'chisqr', 'ndf', 'chgratio',
                     'hfmean', 'hfstd']:
            name += '[%i][%i]' % (gain, pmt)
            pass
        return name
    pass
