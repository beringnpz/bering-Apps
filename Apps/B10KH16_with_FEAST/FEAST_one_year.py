
# coding: utf-8

# # Running FEAST with romscom
# 
# Author: Kelly Kearney
# 
# This script runs a 1-year simulation of the FEAST model.  Note that this uses the 10-layer H16 version of Bering10K+BEST_NPZ.
# 
# I've written this notebook as a semi-tutorial on my python/YAML-based method of running ROMS simulations in general, tailored to the FEAST model.

# ## Environment setup
# 
# We import a few python packages here, including the ROMS Communication Toolbox (romscom), and the FEAST-related tools from the mostly-deprecated romscom predecessor, romsascii.
# 
# We also provide a few user flags here, designed to be turned on an off as necessary by the user.  The `dryrun` flag allows you to run all the file setup stuff up to the first active call to ROMS, but then exit without making that call; this is useful to test that everything is working properly without needing to submit a multi-CPU batch job.  The `buildini` flag sets whether to create the initialization files.  Because this step requires NCO, and the NCO libraries in conda don't play nicely with the mox MPI libraries, this step needs to be separated from actually running ROMS; run this script at least once with `dryrun = True` and `buildini = True`, then with `dryrun = False` and `buildini = False`.  Finally, the `moxlocal` flag is one that alters file paths to indicate local access vs a remote mount; this should be kept set to `True` if you're not me using my Mac.

# In[1]:


# Package import

import os
import sys
import subprocess
from datetime import datetime, timedelta
import netCDF4 as nc

import romscom.romscom as rc
import romscom.rcutils as rcu
from romsascii import feastutilities as fst

# User flags

dryrun    = True  # True to test the setup but not call ROMS exec
buildini  = False # True to build the initialization file (requires NCO)
dryrunnco = False # True to print NCO commands rather than run them (if buildini = True)
moxlocal  = True  # True if running from mox, false if remote-testing

if (not dryrun) and buildini:
    raise Exception("Cannot run ROMS while buildini is active")


# ## Baseline parameters
# 
# The ROMS Communcation Toolbox is designed around two main concepts:
#  
#  1. Storing simulation parameters in YAML files (rather than the ROMS .in test format) makes those parameter files both human-readable and machine parseable, which allows for automation while also allowing for copious commenting to help with project documentation.
#  2. Organizing the setup and running of ROMS simulations via a script provides better maintainability and documentation of those simulations than can be achieved through manual edits to the input files.
# 
# With that in mind, the majority of the parameter-setting related to this simulation should be done within the application's YAML files.  Change values as needed, add comments, etc.  These can be considered the starting point for our ROMS application (in this case, the Bering10K-H16 with FEAST application).  From here, we will use this script to make adjustments that are simulation-specific.  We start by reading these baseline values into dictionaries.

# In[ ]:


ocean = rc.readparamfile("ocean_bering10k_old.yaml", tconvert=True) # Primary standard input
spos  = rc.readparamfile("bering_bestnpzH16_spos.yaml")             # Stations
npz   = rc.readparamfile("bestnpzH16_bpar.yaml")                    # Biology, BEST_NPZ (H16)
ipar  = rc.readparamfile("ice_budgell.yaml")                        # Ice
feast = rc.readparamfile("feast_bpar.yaml")                         # Biology, FEAST


# ## Some pre-simulation setup
# 
# Here, we prepare for a FEAST one-year simulation.  The target year is set, and the simulation is named using that year.  We set the current folder as our working folder, and add In, Out, and Log folders that will hold our simulation-specific input files, output files, and standard output/error logs, respectively.  Finally, we convert the ice and station data read in in the previous step from the YAML dictionary format to ROMS standard input format files.  The resulting standard input files are stripped-down versions of the YAML files, including just the parameters without any comments.

# In[ ]:


# Simulation year

targetyear = 2005

# Simulation name

simname = f"feastH16_{targetyear}"

# Location to place In/Out/Log folders

simdir = "."

# Create In/Out/Log folders
# folder names are returned to fol dictionary

fol = rc.simfolders(simdir, create=True)

# The ice and station output will not change throughout the simulation, 
# so write these to file now, and update the ocean parameters accordingly

iparfile = os.path.join(fol['in'], f"{simname}_ipar.in")
sposfile = os.path.join(fol['in'], f"{simname}_spos.in")

rc.dict2standardin(ipar, compress=False, file=iparfile)
rc.dict2standardin(spos, compress=False, file=sposfile)

ocean['SPOSNAM'] = sposfile
ocean['IPARNAM'] = iparfile


# ## The spinup
# 
# A FEAST simulation starts with a spinup period, spanning July 1-Jan 1, where most FEAST dynamics are turned off, and the fish are just programmed to swim to where their prey is.
# 
# ### Build initialization file
# 
# The initialization file is created by taking a slice of a previously-run BEST_NPZ hindcast and adding the FEAST dye variables to that file.

# In[ ]:


# Set the initialization file name

ocean['ININAME'] = os.path.join(fol['in'], f"ini_{simname}_spinup.nc")
ocean['NRREC'] = -1 # run like a restart simulation

# Path to 10-layer, H16 hindcast simulation
if moxlocal:
    moxdir = '/gscratch/bumblereem/' # local to mox
else:
    moxdir = '/Users/kakearney/Documents/Research/Working/mox_bumblereem/' # remote mount
    
hindcast_folder = os.path.join(moxdir, 'bering10k/output/hindcasts/npz_201904_aclim/Out');

# Start from July of previous year

prevjuly = datetime(targetyear - 1, 7, 1)

if buildini:

    # Find the time slice in that simulation closest to July 1 of previous year
    
    closest = fst.findclosesttime(hindcast_folder, prevjuly)

    # Use NCO utilities to slice the nearest time into a new file

    cmd1 = ['ncks', '-O', '-d', 'ocean_time,{:d}'.format(closest['idx']), 
               closest['filename'], ocean['ININAME']]
    if dryrunnco:
        print(' '.join(cmd1))
    else:
        subprocess.run(cmd1)

    # Use NCO utilities to append the fish variables from the correct year to this slice

    fishfile = f"/gscratch/bumblereem/bering10k/input/ini/ini_feast_{targetyear}.nc"
    cmd2 = ['ncks', '-C', '-A', fishfile, ocean['ININAME']]
    if dryrunnco:
        print(' '.join(cmd2))
    else:
        subprocess.run(cmd2)


# ### Set year-specific input files
# 
# Forcing and boundary files are stored by year, so we need to point to the correct ones (keeping in mind that the spinup will take place in the year prior to our target year)

# In[ ]:


# Set boundary file matching year and number of layers

ncinputfolder = os.path.join(moxdir, 'bering10k/input/')

ocean['BRYNAME'] = os.path.join(ncinputfolder, "hindcast_cfs", f"{prevjuly.year}", 
                                f"roms-cfs-bry-N{ocean['N']}-{prevjuly.year}.nc")

# Forcing files

ocean['FRCNAME'] = [os.path.join(ncinputfolder, 'generic', 'tides_OTBS.nc'), 
                    os.path.join(ncinputfolder, 'generic', 'sss.clim.nc'),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-lwrad-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-Pair-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-Qair-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-rain-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-swrad-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-Tair-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-Uwind-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", 
                                 f"roms-cfs-atmos-Vwind-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'river', f"runoff_kearney_{prevjuly.year}.nc")]

ocean['NFFILES'] = len(ocean['FRCNAME'])


# ### Bio parameters
# 
# The biological parameters combine the BEST_NPZ defaults with the FEAST parameters.

# In[ ]:


# Spinup flags

feast['feast_coupled']     = 0
feast['feast_mort']        = 0
feast['feast_fishing']     = 0
feast['feast_growth']      = 0
feast['feast_recruitment'] = 0

# Make sure feast and npz parameters align

feast['mpredCop'] = npz['mpredCop']
feast['mpredNCa'] = npz['mpredNCa']
feast['mpredEup'] = npz['mpredEup']

# Combine npz and feast dicts into one

bpar = npz
bpar.update(feast) 

# Write to file

bparfile = os.path.join(fol['in'], f"{simname}_00_bpar.in")

rc.dict2standardin(bpar, compress=False, file=bparfile)

ocean['BPARNAM'] = bparfile


# ### Run
# 
# Run the spinup portion of the simulation.  Here, we specify that the restart-counter should start at 0 so that later, the counter will properly correspond with the individual weeks.

# In[ ]:


romscmd = ["mpirun", os.path.join(moxdir, "romsexecs/oceanM_feast_H16")]

enddate = datetime(targetyear,1,1)

rc.runtodate(ocean, simdir, simname, enddate=enddate, dryrunflag=dryrun, addcounter="all", 
             romscmd=romscmd, count=0)


# ## The target year simulation
# 
# The primary target year simulation is run week by week, starting from the spinup simulation.
# 
# ### Switch input files
# 
# We need to change the initialization file to point to the last restart file from the spinup, and switch the forcing and boundary files to the target year.

# In[ ]:


# Boundary 

ocean['BRYNAME'] = os.path.join(ncinputfolder, "hindcast_cfs", f"{targetyear}", 
                                f"roms-cfs-bry-N{ocean['N']}-{targetyear}.nc")

# Forcing files

ocean['FRCNAME'] = [os.path.join(ncinputfolder, 'generic', 'tides_OTBS.nc'), 
                    os.path.join(ncinputfolder, 'generic', 'sss.clim.nc'),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-lwrad-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-Pair-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-Qair-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-rain-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-swrad-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-Tair-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-Uwind-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{targetyear}", 
                                 f"roms-cfs-atmos-Vwind-{targetyear}.nc"),
                    os.path.join(ncinputfolder,'river', f"runoff_kearney_{targetyear}.nc")]

ocean['NFFILES'] = len(ocean['FRCNAME'])


# ### Bio parameters
# 
# Switch spinup-related flags back on, and write new bio parameter file.

# In[ ]:


# Spinup flags

feast['feast_coupled']     = 1
feast['feast_mort']        = 1
feast['feast_fishing']     = 1
feast['feast_growth']      = 1
feast['feast_recruitment'] = 1

# Combine npz and feast dicts into one

bpar = npz
bpar.update(feast) 

# Write to file

bparfile = os.path.join(fol['in'], f"{simname}_01-52_bpar.in")

rc.dict2standardin(bpar, compress=False, file=bparfile)

ocean['BPARNAM'] = bparfile


# ### Archiving parameters
# 
# For the primary simulation, we switch to daily output.  We set the file length to be 2 weeks, which, when combined with the week-by-week running below, will result in one file per "week", even when we add an extra day or two (in the first and last week of the simulation).

# In[ ]:


ocean['NHIS'] = timedelta(days=1)
ocean['NAVG'] = timedelta(days=1)
ocean['NRST'] = timedelta(hours=6)
ocean['NDEFHIS'] = timedelta(weeks=2)
ocean['NDEFAVG'] = timedelta(weeks=2)


# ### Run simulation week by week

# In[ ]:


for weeknum in range(1,53):
    
    # Add (or switch out) appropriate catch forcing file
    
    catchfile = os.path.join(ncinputfolder, "catch", 
                             f"nc_{targetyear}", 
                             f"FEAST_catch_year{targetyear}_week{weeknum}.nc")
    if 'FEAST_catch' in ocean['FRCNAME'][-1]:
        ocean['FRCNAME'][-1] = catchfile
    else:
        ocean['FRCNAME'] = ocean['FRCNAME'] + [catchfile]
        ocean['NFFILES'] = len(ocean['FRCNAME'])
        
    # Check to make sure the last existing restart file counter corresponds to the 
    # prior week

    rstinfo = rcu.parserst(os.path.join(fol['out'], simname))
    if (not rstinfo['lastfile']):
        print(f"  No restart file found to initialize week {weeknum}")
        break
    if (rstinfo['count'] != (weeknum)):
        print(f"  Last restart counter ({rstinfo['count']}) does not match week number ({weeknum})")
        break
        
    # Update start date so the internal ROMS counters start from the beginning of
    # the week.  This keeps the output archiving files aligned with the weeks.
    
    if weeknum == 1:
        f = nc.Dataset(ocean['ININAME'], 'r')
        tunit = f.variables['ocean_time'].units
        tcal = f.variables['ocean_time'].calendar
        t1 = nc.num2date(f.variables['ocean_time'][-1], units=tunit, calendar=tcal)
        ocean['DSTART'] = t1 # Last time in spinup
    else:
        ocean['DSTART'] = datetime(targetyear,1,1) + timedelta(weeks=weeknum-1)
        
    # Update the end date
    
    if weeknum == 52:
        enddate = datetime(targetyear+1,1,1)
    else:
        enddate = datetime(targetyear,1,1) + timedelta(weeks=weeknum)
        
    # Run
    
    rc.runtodate(ocean, simdir, simname, enddate=enddate, dryrunflag=dryrun, addcounter="all", 
             romscmd=romscmd)
    
    

