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

'''
cd ~
mkdir PrometeoSource
cd PrometeoSource
git clone https://github.com/dbullock1086/Herakles
'''

Now create a "work" area in a directory suitable for working with large files (~ 10 MB to 10 GB):

'''
cd ~/workarea
mkdir PrometeoTest
cd PrometeoTest
'''

Copy the `setup.sh` file from the source directory into this new area:

'''
cp ~/PrometeoSource/Herakles/share/setup.sh .
'''

Edit this setup file to ensure that
- the RootCore version is acceptable
- the source directory is correct
- the working directory is correct

Source `setup.sh` and it should build RootCore in your user area, then it will return to your working directory before defining other environment variables. Always start from your working directory and source this setup file for each session on lxplus.

**Note:** You probably also want to copy `batch.sh` from the same directory and modify it, which is useful for streamlining the lxbatch submission.

### Running

As long as `$PATH` and `$PYTHONPATH` are defined to the appropriate locations in the source directory, you should be ready to run some tests. The basic commands will look like this:

```
herakles biasnoise Data_20170101_PedStability.root Hist_20170101_PedStability.root --fit gaus
```

This will run a pre-defined test called `biasnoise`. The input data comes from the file `Data_20170101_PedNoise.root`. The test analyzes the "sample" branch from the input TTree by plotting a histogram and fitting to a (single) Gaussian for each channel. These histograms and fit parameters are also summarized into a full overview for the minidrawer. The histograms are output to a new file called `Hist_20170101_PedStability.root`.

There are additional configurations for each test, so a familiarity with the flexibility of the code is encouraged.
