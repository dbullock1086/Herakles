#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.comdbullock1086/Herakles

# Nomenclature.py
# This class is used to consolidate all of the name references into a single
# location. It is intended to remove a lot of clutter from the other scripts
# whenever new algorithms are added.

class Nomenclature (object):
    def VarTitle (self, var):
        #### get a descriptive title for the variable
        if var == 'cap': title = 'Capacitance'
        elif var == 'charge': title = 'Charge'
        elif var == 'ped': title = 'Bias'
        elif var == 'samples': title = 'Samples'
        elif var == 'saturation': title = 'Saturation Count'
        elif var == 'null': title = 'Null Count'
        elif var == 'crc': title = 'Error Count'
        elif var == 'pedratio': title = 'Sample/Pedestal'
        elif var == 'hfmean': title = 'HF Mean'
        elif var == 'hfstd': title = 'HF Std'
        elif var == 'pedestal': title = 'Pulse Fit Pedestal'
        elif var == 'height': title = 'Pulse Fit Height'
        elif var == 'phase': title = 'Pulse Fit Phase'
        elif var == 'width': title = 'Pulse Fit Width'
        elif var == 'prob': title = 'Pulse Fit Probability'
        elif var == 'fastfit': title = 'Fast Integral'
        pass

    def VarELName (self, var, gain, pmt):
        if var == 'evt': name = 'EvtRange'
        elif var == 'cap': name = 'CapRange'
        elif var == 'charge': name = 'ChargeRange'
        elif var == 'ped': name = 'PedRange'
        elif var == 'samples': name = 'SampleRange'
        elif var == 'saturation': name = 'Saturation'
        elif var == 'null': name = 'NullValue'
        elif var == 'crc': name = 'CRCError'
        elif var == 'hfmean': name = 'hfmean'
        elif var == 'hfstd': name = 'hfstd'
        elif var == 'pedratio': name = 'PedRatio'
        elif var == 'pedestal': name = 'Pedestal'
        elif var == 'height': name = 'Height'
        elif var == 'phase': name = 'Phase'
        elif var == 'width': name = 'Width'
        elif var == 'prob': name = 'Prob'
        elif var == 'fastfit': name = 'FastFit'
        pass

    def VarMinMax (self, var):
        if var in ['cap', 'charge']: minmax = False
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
        elif var in ['crc', 'hfmean', 'hfstd', 'pedratio',
                     'pedestal', 'height', 'phase', 'width', 'prob',
                     'fastfit', 'fastratio']:
            name += '[%i][%i]' % (gain, pmt)
            pass
        return name
    pass
