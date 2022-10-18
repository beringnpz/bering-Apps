
# coding: utf-8

# # Running FEAST with romscom
# 
# This script runs a 1-year simulation of the FEAST model.  Note that this uses the 10-layer H16 version of Bering10K+BEST_NPZ. 

# ## Environment setup

# In[1]:


import os
import sys
from datetime import datetime, timedelta
import romscom.romscom as rc
from romsascii import feastutilities as fst

dryrun = True
dryrunnco = True


# ## Baseline parameters
# 
# The baseline input parameters are held in a series of YAML files.  These can be considered the starting point for our ROMS application.  From here, we can make adjustments that are simulation-specific.  We start by reading these values into dictionaries.

# In[2]:


ocean = rc.readparamfile("ocean_bering10k_old.yaml", tconvert=True)
spos  = rc.readparamfile("bering_bestnpzH16_spos.yaml")
npz   = rc.readparamfile("bestnpzH16_bpar.yaml")
ipar  = rc.readparamfile("ice_budgell.yaml")
feast = rc.readparamfile("feast_bpar.yaml")


# ## Some pre-simulation setup
# 

# In[3]:


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

# In[4]:


# Set the initialization file name

ocean['ININAME'] = os.path.join(fol['in'], f"ini_{simname}_spinup.nc")
ocean['NRREC'] = -1 # run like a restart simulation

# Path to 10-layer, H16 hindcast simulation
hindcast_folder = '/gscratch/bumblereem/bering10k/output/hindcasts/npz_201904_aclim/Out'

# Find the time slice in that simulation closest to July 1 of previous year
prevjuly = datetime(targetyear - 1, 7, 1)
closest = fst.findclosesttime(hindcast_folder, prevjuly)

# # Use NCO utilities to slice the nearest time into a new file

cmd1 = ['ncks', '-O', '-d', 'ocean_time,{:d}'.format(closest['idx']), 
           closest['filename'], ocean['ININAME']]
if dryrunnco:
    print(' '.join(cmd1))
else:
    subprocess.run(cmd1)
    
# # Use NCO utilities to append the fish variables from the correct year to this slice

fishfile = f"/gscratch/bumblereem/bering10k/input/ini/ini_feast_{targetyear}.nc"
cmd2 = ['ncks', '-C', '-A', fishfile, ocean['ININAME']]
if dryrunnco:
    print(' '.join(cmd2))
else:
    subprocess.run(cmd2)


# ### Set year-specific input files
# 
# Forcing and boundary files are stored by year, so we need to point to the correct ones (keeping in mind that the spinup will take place in the year prior to our target year)

# In[5]:


# Set boundary file matching year and number of layers

ncinputfolder = '/gscratch/bumblereem/bering10k/input/'

ocean['BRYNAME'] = os.path.join(ncinputfolder, "hindcast_cfs", f"{prevjuly.year}", 
                                f"roms-cfs-bry-N{ocean['N']}-{prevjuly.year}.nc")

# Forcing files

ocean['FRCNAME'] = [os.path.join(ncinputfolder, 'generic', 'tides_OTBS.nc'), 
                    os.path.join(ncinputfolder, 'generic', 'sss.clim.nc'),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-lwrad-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-Pair-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-Qair-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-rain-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-swrad-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-Tair-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-Uwind-{prevjuly.year}.nc"),
                    os.path.join(ncinputfolder,'hindcast_cfs', f"{prevjuly.year}", f"roms-cfs-atmos-Vwind-{prevjuly.year}.nc")]

ocean['NFFILES'] = len(ocean['FRCNAME'])


# ### Bio parameters
# 
# The biological parameters combine the BEST_NPZ defaults with the FEAST parameters.

# In[6]:


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

bparfile = os.path.join(fol['in'], f"{simname}_spinup_bpar.in")

rc.dict2standardin(bpar, compress=False, file=bparfile)

ocean['BPARNAM'] = bparfile


# ### Run
# 
# Run the spinup portion of the simulation

# In[7]:


romscmd = ["mpirun", "/gscratch/bumblereem/romsexecs/oceanM_feast_H16"]

enddate = datetime(targetyear,1,1)

rc.runtodate(ocean, simdir, simname, enddate=enddate, dryrunflag=dryrun, addcounter="all")

