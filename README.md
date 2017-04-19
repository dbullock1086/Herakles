# Herakles

### written by Daniel Bullock, 2013-2017

#### https://github.com/dbullock1086/Herakles

This software was written to aid in the efforts of the TileDemo and Prometeo projects as part of the ATLAS TileCal Collaboration at CERN. This software is intended to provide diagnostic analysis of the TileCal minidrawers by running raw data from the TileCal minidrawers through a series of quality tests.

There are two other frameworks that Herakles currently relies on:
- A data collection routine that interacts directly with the hardware to cycle through various conditions and retrieve data. Herakles can then analyze this data.
- A graphical presentation of the plots from Herakles and a module-specific evaluation of parameters.

However, Herakles does have legacy code that once performed these routines at an earlier stage of development. If there is interest in reviving these parts, these functionalities can be re-incorporated (contact me by email).

### Setup

This setup assumes that you are working on lxplus. It is anticipated that it should also work on a local system if you have a RootCore setup -- just make the appropriate changes where necessary.

Clone the repository to a "user" area:

```
cd ~
mkdir PrometeoSource
cd PrometeoSource
git clone https://github.com/dbullock1086/Herakles
```

Now create a "work" area in a directory suitable for working with large files (~ 10 MB to 10 GB):

```
cd ~/workarea
mkdir PrometeoTest
cd PrometeoTest
```

Copy some files from the source directory into this new area:

```
cp ~/PrometeoSource/Herakles/share/setup.sh .
cp ~/PrometeoSource/Herakles/share/batch.sh .
cp ~/PrometeoSource/Herakles/share/history.json .
cp ~/PrometeoSource/Herakles/share/hourly_cronjob.py .
```

Edit the `setup.sh` file to ensure that
- the RootCore version is acceptable
- the source directory is correct
- the working directory is correct

Source `setup.sh` and it should build RootCore in your user area, then it will return to your working directory before defining other environment variables. Always start from your working directory and source this setup file for each session on lxplus.

**Note:** You probably also want to edit `batch.sh`, which is useful for streamlining the lxbatch submission.

### Running

As long as `$PYTHONPATH` is defined to the appropriate location in the source directory, you should be ready to run some tests. The basic commands will look like this:

```
herakles SampleNoise Data_20170101_PedStability.root Hist_20170101_PedStability.root
```

This will run a pre-defined test called `biasnoise`. The input data comes from the file `Data_20170101_PedNoise.root`. The test analyzes the "sample" branch from the input TTree by plotting a histogram and fitting to a (single) Gaussian for each channel. These histograms and fit parameters are also summarized into a full overview for the minidrawer. The histograms are output to a new file called `Hist_20170101_PedStability.root`.

There are three command-line options that you may find useful:

- `--gains` -- The module returns data with a high-gain amplification and a low-gain amplification. If you want just the low gain, use `--gains 0`. Conversely, use `--gains 1` for just the high gain. By default, both gains are used, which can also be ensured by typing `--gains 0 1`. The only valid choices are zero and one.

- `--channels` -- Each module has 48 PMTs. If you want a subset of them, just add them to the channels option list. Remember that Herakles uses a zero-based numbering convention, so the option `--channels 0 1 2 3` selects the first four PMTs and `--channels 44 45 46 47` selects the last four PMTs. The only valid choices are zero through 47.

- `--window` -- An event is sampled 128 times. You can study all 128 samples at a time, or you can limit the range to a window of indices. For example, `--window 0 32` will ignore anything after the 31st sample (since 32 is not inclusive to a lower-edge binning). **Note:** During the development of the sROD, the 32-sample window was copied 4 times to occupy the full 128 samples, which had severely misleading consequences for analyses that used the full window.

### Basic Overview

This section is just an at-a-glance description of how the software analyses the data. If you are curious about the details of steps or what options are available, the comments in the source code are your best documentation.

The basic flow is:

1. Raw data is passed through EventLoop that runs a sequence of event-by-event algorithms. Your end result will be histograms, so some of these algorithms just determine minimum and maximum values and put them in a histogram that summarizes the state of individual channels (separated by gain). Other algorithms may be more derive new branches that will make it easier to study the data.

2. The derived data is passed through MultiDraw algorithms to study each channel (gain and PMT) of the entire module. Once the histogram limits are determined, filling them with data ensures that you can see all of the events with a binning that makes sense. If an algorithm adds a new branch, you can also fill a histogram with values from that branch.

3. Some of the histograms are fit to expected models. If a channel (gain and PMT) is fit to a particular model, a summary histogram of all PMTs is updated to include the values of each parameter for a comparison.

Your first interaction with `Herakles` is probably the command line as shown in the previous section. The `SampleNoise` term is indicating a particular test routine (a series of configurations and control instructions) that are designed to investigate the performance of a particular subsystem in the TileDemo mini-drawer. This command is a particular case in `share/herakles_scripts.sh`, which points to a source code in `share/SampleNoise.py`.

Within this file, you will find a class by the same name. In this file, you will find some configurations:

- `self.SetTree ('dataTree')` -- The raw input probably contains `TTree` objects for the raw data, the parameters, and the DCS. This line indicates that `Herakles` should examine the one called "dataTree".
- `self.AddTDAlg ('SampleRange')` -- This is a simple algorithm that determines the minimum and maximum values of the `samples_hi` and `samples_lo` branches. This will become important later.
- `self.CopyBranch ('samples')` -- From raw data, we need to copy this branch from the raw data to the derived set. We will be making histograms from the derived set.
- `self.AddMDHist ('samples', fit='gaus')` -- A 1D histogram will be drawn for each gain and PMT, and each histogram will later be fit to a (single) Gaussian distribution.
- `self.OwnELHist ('samples')` -- It may be interesting to see if different PMTs have different minimum and maximum values, so we want to keep the output of the `SampleRange` algorithm and place it in the final output histogram.

Such a script file is not limited to what you see here. You can have several instances of `AddTDAlg` in case you want more test information in the final analysis. In that case, you probably need more `CopyBranch` instructions and so on. Look at some of the other test scripts for more ideas. There are additional configurations for each test, so a familiarity with the flexibility of the code is encouraged.

### EventLoop Algorithms

If you are interested in testing the behavior of part of the mini-drawer, the algorithms in the `Herakles` source code are where you should look.

**Caution:** If you think you need to write a new algorithm, please be mindful of the mess you might introduce. The algorithms are intended to be run in parallel and add as little as possible to the complexity of the EventLoop job. Therefore, algorithms should be cooperative, and no two algorithms should have an overlap in what they do.

There are some very simple algorithms that just determine minimum and maximum values:

- `EvtRange`
- `CapRange`
- `ChargeRange`
- `PedRange`
- `SampleRange`

The first three return histograms that have two bins: the minimum value in bin 1 and the maximum value in bin 2. The pedestal values and sample values have four histograms (two gains times two max/min values) that have 48 bins (one for each PMT). Additionally, the capacitance, charge, and pedestal are independent variables, so they include histograms that count the number steps -- or changes in value, i.e. for a linearity test.

Three algorithms just count the number of extreme values:

- `CRCError`
- `Saturation`
- `NullValue`

These algorithms look for certain values in the `samples_*` branches. CRC values are negative numbers or numbers that exceed the 12-bit range, usually a purposefully invalid number to indicate a firmware error. Saturated values are numbers at the 12-bit ceiling of 4095. Null values are zero. These three are a good example of keeping algorithms simple; they are separate even though you probably want all three in one test. **Note:** Algorithms that analyze the `samples_*` branches will skip these three values.

There is an algorithm that studies the high-frequency response of the `samples_*` branch:

- `SampleHF`

Because there are 128 samples per event, the high-frequency algorithm determines the average value per event and the standard deviation of values. These are derived values, and therefore a new branch is created in the output.

There is an algorithm that compares two branches:

- `PedRatio`

This calculates the `samples_*` value divided by the `ped_*` value, and adds it as a new branch in the output.

There are algorithms for studying pulses:

- `PulseFit`
- `FastFit`

The first one takes the 128 samples from each event and fits it to a calibrated pulse shape. The branches added by this algorithm include the parameters of the fit (pedestal, height, phase, width) and the probability of the fit (using chi-squared and the number of degrees of freedom). It also adds a height/charge branch. This algorithm can be a little slow, so the second algorithm makes a coarser approximation of the fit by subtracting the `ped_*` value and integrating.

One more algorithm can also be useful for pulses:

- `SampleLF`

This algorithm looks separately at sample number and channel number (128 bin by 48 bin histograms), and displays the mean value and standard deviation of each. For example, it can show you the pulse shape (by color spectrum) for each PMT simulataneously over a large number of events.

And one algorithm looks for systematic cross-talk:

- `CorrHF`

This calculates the linear correlation for all gains and channels. If the noise is really random, the values should tend toward zero. As the values tend to reach -1 or +1, a systematic link between channels may exist.
