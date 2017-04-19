#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

# Fitter.py
# A class that fits the histograms for a particular gain and PMT.

from ROOTBase import *

class Fitter (object):
    def Fit (self, name, mode):
        # mode-specific fit modeling
        if   mode == 'linear': self._linear ()
        elif mode == 'gaus':   self._gaus ()
        elif mode == 'dgaus':  self._dgaus ()
        elif mode == 'pulse':  self._pulse ()

        # perform fit to model
        self.hists[name].Fit (self.model, 'ww0qRS')
        pass

    def _linear (self):
        #### fit to a linear model
        # note: restrict function range to non-saturated and non-null values
        binx = self.hists[name].GetNbinsX ()
	xmin = False
	for xbin in xrange (1, binx + 1):
	    val = self.hists[name].GetBinContent (xbin)
	    if xmin:
                # establish the upper bin limit
		if val < 4095 and val > 0:
                    maxx = self.hists[name].GetXaxis().GetBinUpEdge (xbin)
                    pass
		else: break
		pass
	    else:
                # establish the lower bin limit
		if val < 4095 and val > 0:
		    minx = self.hists[name].GetXaxis().GetBinLowEdge (xbin)
		    xmin = True
		    pass
		pass
	    pass

        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               '[0]*x+[1]',
                               minx. maxx)

        self.model.SetParName   (0, 'slope')
        self.model.SetParName   (1, 'intercept')
        pass

    def _gaus (self):
        #### fit to a Gaussian model
        minx = self.hists[name].GetXaxis().GetXmax()
        maxx = self.hists[name].GetXaxis().GetXmin()
        num  = self.hists[name].GetEntries ()
        mean = self.hists[name].GetMean ()
        size = maxx - minx
        std  = self.hists[name].GetStdDev ()

        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               '[0]*TMath::Gaus(x,[1],[2],1)',
                               minx, maxx)

        # normalization should be the total number of entries
        self.model.SetParName   (0, 'norm')
        self.model.SetParLimits (0, num/2., 2.*num)
        self.model.SetParameter (0, num)

        # central location should match the histogram mean
        self.model.SetParName   (1, 'mu')
        self.model.SetParLimits (1, mean - size/4., mean + size/4.)
        self.model.SetParameter (1, mean)

        # width should match the histogram standard deviation
        self.model.SetParName   (2, 'sigma')
        self.model.SetParLimits (2, 0, size)
        self.model.SetParameter (2, std)
        pass

    def _dgaus (self):
        #### fit to a double Gaussian model
        minx = self.hists[name].GetXaxis().GetXmax()
        maxx = self.hists[name].GetXaxis().GetXmin()
        num  = self.hists[name].GetEntries ()
        mean = self.hists[name].GetMean ()
        size = maxx - minx
        std  = self.hists[name].GetStdDev ()

        # note: shared central value
        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               '[0]*(TMath::Gaus(x,[1],[2],1)+TMath::Gaus(x,[1],[3],1))/2',
                               minx, maxx)

        # normalization should be the total number of entries
        self.model.SetParName   (0, 'norm')
        self.model.SetParLimits (0, num/2., 2.*num)
        self.model.SetParameter (0, num)

        # central location should match the histogram mean
        self.model.SetParName   (1, 'mu')
        self.model.SetParLimits (1, mean - size/4., mean + size/4.)
        self.model.SetParameter (1, mean)

        # first Gaussian should have a width upper-bound
        # by the histogram standard deviation
        self.model.SetParName   (2, 'sigma1')
        self.model.SetParLimits (2, 0, std)
        self.model.SetParameter (2, std/2.)

        # second Gaussian should have a width lower-bound
        # by the histogram standard deviation
        self.model.SetParameter (3, 'sigma2')
        self.model.SetParLimits (3, std, size)
        self.model.SetParameter (3, std + (size-std/2.))
        pass

    def _pulse (self):
        #### fit to a pulse model
        if not hasattr (self, 'pulsefit'): self.pulsefit = ROOT.TD.PulseFit ()

        binx   = self.hists[name].GetNbinsX ()
        minx   = self.hists[name].GetXaxis().GetXmax()
        maxx   = self.hists[name].GetXaxis().GetXmin()
        num    = self.hists[name].GetEntries ()
        mean   = self.hists[name].GetMean ()
        size   = maxx - minx
        std    = self.hists[name].GetStdDev ()
        minbin = self.hists[name].GetMinimumBin ()
        maxbin = self.hists[name].GetMaximumBin ()
        minval = self.hists[name].GetMinimum ()
        maxval = self.hists[name].GetMaximum ()
        valr   = maxval - minval

        self.model = ROOT.TF1 (self.hists[name].GetName() + '_fit',
                               self.pulsefit.PulseShape,
                               minx, maxx)
        # Is the pulse positive or negative?
        # ignore the maximum and minimum values then determine if
        # max or min is farther from the remaining average
        approx = 0.
        for xbin in xrange(1, binx + 1):
            if (xbin == minbin): continue
            if (xbin == maxbin): continue
            approx += self.hists[name].GetBinContent (xbin)
            pass
        approx /= binx - 2.
        if (abs(maxval-approx) > abs(approx-minval)):
            pos = True
            pass
        else: pos = False

        # these three will depend on whether the pulse is positive or negative
        self.model.SetParName (0, 'pedestal')
        self.model.SetParName (1, 'height')
        self.model.SetParName (2, 'phase')

        if pos: # the pulse is positive
            # pedestal should be in the lower half of the range
            self.hists[name].SetParLimits (0, minval, valr/2.)
            self.hists[name].SetParameter (0, minval + valr/8.)
            # height can actually be larger than the range
            self.hists[name].SetParLimits (1, valr/2., 2.*valr)
            self.hists[name].SetParameter (1, valr)
            # phase is somewhere near the maximum
            # note: this could fail if the pulse is clipped
            self.hists[name].SetParLimits (2, maxbin - 5., maxbin + 5.)
            self.hists[name].SetParameter (2, maxbin)
            pass
        else: # the pulse is negative
            # pedestal should be in the upper half of the range
            self.hists[name].SetParLimits (0, valr/2., maxval)
            self.hists[name].SetParameter (0, maxval - valr/8.)
            # height can actually be larger than the range
            self.hists[name].SetParLimits (1, valr/2., 2.*valr)
            self.hists[name].SetParameter (1, valr)
            # phase is somewhere near the minimum
            self.hists[name].SetParLimits (2, minbin - 5., minbin + 5.)
            self.hists[name].SetParameter (2, minbin)
            pass
        # the width should be fairly stable
        # (fit success is also sensitive to constraining this parameter)
        self.model.SetParName   (3, 'width')
        self.model.SetParLimits (3, 0.9, 1.0)
        self.model.SetParameter (3, 0.95)
        pass
    pass
