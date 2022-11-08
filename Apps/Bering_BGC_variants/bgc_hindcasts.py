# BGC Hindcasts: testing

import os
import sys
from datetime import datetime, timedelta

import romscom.romscom as rc

# BGC setup

dryrun = False

bioname  = ["bestnpz", "cobalt", "banas", "phys"]
romsexec = ["./romsM_bestnpz_202211071253",
             "./romsM_cobalt_202211071304", 
              "./romsM_banas_202211071326",
               "./romsM_phys_202211071317"]

mpiexec = "mpirun"

enddate = datetime(1986,1,1)
dstart = datetime(1985,1,1,0,0)

# Defaults

appfol = "."

ocean   = rc.readparamfile(os.path.join(appfol, "bering_ocean.yaml"), tconvert=True)
station = rc.readparamfile(os.path.join(appfol, "bering_spos.yaml"))
ice     = rc.readparamfile(os.path.join(appfol, "bering_ipar.yaml"))

bio = [rc.readparamfile(os.path.join(appfol, f"bering_bpar_{x}.yaml")) for x in bioname if x != "phys"]

# Time variables

ocean['DSTART'] = dstart
ocean['NTIMES'] = enddate - ocean['DSTART']

# Set archiving time steps and file size

ocean['NSTA'] = timedelta(hours=6)
ocean['NAVG'] = timedelta(weeks=1)
ocean['NHIS'] = timedelta(weeks=1)
ocean['NDIA'] = timedelta(weeks=1)
ocean['NDEFAVG'] = timedelta(weeks=10)
ocean['NDEFHIS'] = timedelta(weeks=10)
ocean['NDEFDIA'] = timedelta(weeks=10)

# Adjust input based on years to run 
# (template file only holds a few years worth)
# TODO: ROMS code currently crashes (badly) if you supply more than 100 forcing 
# multi-file files... need to fix that to run the full time

yrs = range(1984,1990) #2016) # 1984-2015, 1984 added to protect against small gaps at start

ocean["FRCNAME"] = ["./Data/BarrowCO2/atmo_co2_barrow_1970_2020.nc", 
                    "./Data/Iron/zeros_Bering10K_ironflux_1970-2025.nc",
                    "./Data/salinity/sss.clim.nc",
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-Pair-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-Qair-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-Tair-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-Uwind-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-Vwind-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-rain-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-swrad-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-atmos-northPacific-lwrad-{x}.nc" for x in yrs],
                    [f"./Data/KearneyEtAl2019/KearneyEtAl2019_Bering10K_Runoff_{x}.nc" for x in yrs],
                    [f"./Data/KearneyEtAl2019/KearneyEtAl2019-Mathis2011_Bering10K_RunoffCarbon_{x}.nc" for x in yrs]
                   ]
ocean["NFFILES"] = len(ocean["FRCNAME"])

ocean["BRYNAME"] = ["./Data/WOA/WOA2018-Bering10K-N30-brybgc.nc",
                    [f"./Data/CFS/{x}/CFS-ocean-Bering10K-N30-bryocn-{x}.nc" for x in yrs],
                    [f"./Data/CFS/{x}/CFS-ocean-ESPER-Bering10K-N30-brycarbon-{x}.nc"  for x in yrs]
                   ]
ocean["NBCFILES"] = len(ocean["BRYNAME"])

# Run simulations

for ii in range(0,len(bioname)):
    
    print(f"Preparing {bioname[ii]} simulation...")
    
    pflag = bioname[ii] == "phys"
    
    simdir = f"bgcmip_{bioname[ii]}"
    simname = f"bgcmip_{bioname[ii]}"
    
    # Set up sim folders
    
    fol = rc.simfolders(simdir, create=True)
    
    # Write accessory files
    
    bpar = os.path.join(fol['in'], f"{simname}_bpar.in") # bio
    ipar = os.path.join(fol['in'], f"{simname}_ipar.in") # ice
    spos = os.path.join(fol['in'], f"{simname}_spos.in") # stations

    if not pflag:
        rc.dict2standardin(bio[ii], compress=False, file=bpar)
    rc.dict2standardin(ice, compress=False, file=ipar)
    rc.dict2standardin(station, compress=False, file=spos)

    if not pflag:
        ocean['BPARNAM'] = bpar
    ocean['IPARNAM'] = ipar
    ocean['SPOSNAM'] = spos
    
    # Point to correct initialization and varinfo files
    
    if pflag:
        ocean['ININAME'] = os.path.join('.', 'Data', 'roms_ini', 'CFS_Bering10K-ocn_ini_19850101.nc')
    else:
        ocean['ININAME'] = os.path.join('.', 'Data', 'roms_ini', f"CFS-Glodap_Bering10K-{bioname[ii]}_ini_19850101.nc")
    
    ocean['VARNAME'] = os.path.join(appfol, f"varinfo_{bioname[ii]}.dat")
    ocean['NRREC'] = 0 # reset, in case changed by one of the other bio sims
    
    # Run
    
    romscmd = [mpiexec, romsexec[ii]]
    
    rc.runtodate(ocean, simdir, simname, enddate=enddate, romscmd=romscmd, 
                 dryrunflag=dryrun)
                 
                 