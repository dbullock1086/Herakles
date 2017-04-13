#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *

class Fitter (object):
    def Fit (self, name, mode):
        self.par = {}
        # number of entires in the histogram
        self.par['num'] = self.hists[name].GetEntries ()
        # number of bins in the histogram
        self.par['binx'] = self.hists[name].GetNbinsX ()
        # minimum x-value of the histogram
        self.par['minx'] = self.hists[name].GetXaxis().GetXmax()
        # maximum x-value of the histogram
        self.par['maxx'] = self.hists[name].GetXaxis().GetXmin()
        # size of x-axis
        self.par['size'] = self.par['maxx'] - self.par['minx']
        # bin width
        self.par['bins'] = self.par['size'] / self.par['binx']
        # location of bin with the least entries
        self.par['minbin'] = self.hists[name].GetMinimumBin ()
        # location of bin with the most entries
        self.par['maxbin'] = self.hists[name].GetMaximumBin ()
        # least number of entries in a bin
        self.par['minval'] = self.hists[name].GetMinimum ()
        # most number of entries in a bin
        self.par['maxval'] = self.hists[name].GetMaximum ()
        # size of entry difference
        self.par['valr'] = self.par['maxval'] - self.par['minval']
        # mean value of the histogram
        self.par['mean'] = self.hists[name].GetMean ()
        # standard deviation of the histogram
        self.par['std'] = self.hists[name].GetStdDev ()

        if   mode == 'linear': self._linear ()
        elif mode == 'gaus':   self._gaus ()
        elif mode == 'dgaus':  self._dgaus ()
        elif mode == 'pulse':  self._pulse ()
        self.hists[name].Fit (self.model, 'ww0qRS')
        pass

    def _linear (self):
        #### fit to a linear model
        # note: restrict function range to non-saturated values
	xmin = False
	for xbin in xrange (1, self.par['binx']+1):
	    val = self.hists[name].GetBinContent (xbin)
	    if xmin:
                # establish the upper bin limit
		if val < 4096:
                    maxx = self.hists[name].GetXaxis().GetBinUpEdge (xbin)
                    pass
		else: break
		pass
	    else:
                # establish the lower bin limit
		if val < 4096:
		    minx = self.hists[name].GetXaxis().GetBinLowEdge (xbin)
		    xmin = True
		    pass
		pass
	    pass

        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               '[0]*x+[1]',
                               minx. maxx)

        self.model.SetParName   (0, 'slope')
        #self.model.SetParLimits (0, ?, ?)
        #self.model.SetParameter (0, ?)

        self.model.SetParName   (1, 'intercept')
        #self.model.SetParLimits (1, ?, ?)
        #self.model.SetParameter (1, ?)
        pass

    def _gaus (self):
        #### fit to a Gaussian model
        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               '[0]*TMath::Gaus(x,[1],[2],1)',
                               self.par['minx'], self.par['maxx'])

        # normalization should be the total number of entries
        self.model.SetParName   (0, 'norm')
        self.model.SetParLimits (0, self.par['num']/2, 2*self.par['num'])
        self.model.SetParameter (0, self.par['num'])

        # central location should match the histogram mean
        self.model.SetParName   (1, 'mu')
        self.model.SetParLimits (1, self.par['mean'] - self.par['size']/4,
                                 self.par['mean'] + self.par['size']/4)
        self.model.SetParameter (1, self.par['mean'])

        # width should match the histogram standard deviation
        self.model.SetParName   (2, 'sigma')
        self.model.SetParLimits (2, 0, self.par['size'])
        self.model.SetParameter (2, self.par['std'])
        pass

    def _dgaus (self):
        #### fit to a double Gaussian model
        # note: shared central value
        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               '[0]*(TMath::Gaus(x,[1],[2],1)+TMath::Gaus(x,[1],[3],1))/2',
                               self.par['minx'], self.par['maxx'])

        # normalization should be the total number of entries
        self.model.SetParName   (0, 'norm')
        self.model.SetParLimits (0, self.par['num']/2, 2*self.par['num'])
        self.model.SetParameter (0, self.par['num'])

        # central location should match the histogram mean
        self.model.SetParName   (1, 'mu')
        self.model.SetParLimits (1, self.par['mean'] - self.par['size']/4,
                                 self.par['mean'] + self.par['size']/4)
        self.model.SetParameter (1, self.par['mean'])

        # first Gaussian should have a width upper-bound
        # by the histogram standard deviation
        self.model.SetParName   (2, 'sigma1')
        self.model.SetParLimits (2, 0, self.par['std'])
        self.model.SetParameter (2, self.par['std']/2)

        # second Gaussian should have a width lower-bound
        # by the histogram standard deviation
        self.model.SetParameter (3, 'sigma2')
        self.model.SetParLimits (3, self.par['std'], self.par['size'])
        self.model.SetParameter (3, self.par['std'] + \
                                 (self.par['size']-self.par['std'])/2)
        pass

    def _pulse (self):
        #### fit to a pulse model
        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               ROOT.TD.PulseShape,
                               self.par['minx'], self.par['maxx'])
        # Is the pulse positive or negative?
        # ignore the maximum and minimum values then determine if
        # max or min is farther from the remaining average
        approx = 0
        for xbin in xrange(1, self.par['binx']+1):
            if (xbin == self.par['minbin']): continue
            if (xbin == self.par['maxbin']): continue
            approx += self.hists[name].GetBinContent (xbin)
            pass
        approx /= self.par['binx'] - 2
        if (abs(self.par['maxval']-approx) > abs(approx-self.par['minval'])):
            pos = True
            pass
        else: pos = False

        self.model.SetParName (0, 'pedestal')
        self.model.SetParName (1, 'height')
        self.model.SetParName (2, 'phase')
        if pos: # the pulse is positive
            # pedestal should be in the lower half of the range
            self.hists[name].SetParLimits (0, self.par['minval'],
                                           self.par['valr']/2)
            self.hists[name].SetParameter (0, self.par['minval'] + \
                                           self.par['valr']/8)
            # height can be actually be larger than the range
            self.hists[name].SetParLimits (1, self.par['valr']/2,
                                           2*self.par['valr'])
            self.hists[name].SetParameter (1, self.par['valr'])
            # phase is somewhere near the maximum
            self.hists[name].SetParLimits (2, self.par['maxbin']-3,
                                           self.par['maxbin']+1)
            self.hists[name].SetParameter (2, self.par['maxbin'] - 1)
            pass
        else: # the pulse is negative
            # pedestal should be in the upper half of the range
            self.hists[name].SetParLimits (0, self.par['valr']/2,
                                           self.par['maxval'])
            self.hists[name].SetParameter (0, self.par['maxval'] - \
                                           self.par['valr']/8)
            # height can actually be larger than the range
            self.hists[name].SetParLimits (1, self.par['valr']/2,
                                           2*self.par['valr'])
            self.hists[name].SetParameter (1, self.par['valr'])
            # phase is somewhere near the minimum
            self.hists[name].SetParLimits (2, self.par['minbin'] - 3,
                                           self.par['minbin'] + 1)
            self.hists[name].SetParameter (2, self.par['minbin'] - 1)
            pass
        # the width should be fairly stable
        # (fit success is also sensitive to constraining this parameter)
        self.model.SetParName   (3, 'width')
        self.model.SetParLimits (3, 0.9, 1.0)
        self.model.SetParameter (3, 0.95)
        pass

    def FitRes (self, name):
        #### perform the fit
        fitres = self.hists[name].GetFunction (self.hists[name].GetName() + \
                                               '_fit')
        return fitres
    pass
