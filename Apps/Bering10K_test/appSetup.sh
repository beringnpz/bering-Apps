# This script builds the primary Application-related files necessary to compile and run 
# the Bering10K + Budgell ice + varied bio version of new-ROMS.
#*****************************************************************************************

# Adjust to match local location of bering-Apps clone

berapps=../..

#-----------------------
# ROMS source code stuff
#-----------------------

# The header file combines the defaults for the Bering_10K application along with all 
# biological module options.  The bio-dependent flags are wrapped in a conditional such 
# that the specific module to be compiled is turned on by the compilation script 
# (build_roms.bash).  This allows the same header file to be used for all app variants.

cp ${berapps}/subApps/BERING_10K_new/bering_10k.h bering_10k.h 
cat ${berapps}/subApps/COBALT/cobalt_cpp.h >> bering_10k.h
cat ${berapps}/subApps/BESTNPZ/bestnpz_cpp.h >> bering_10k.h

# Point sources: Used to set Bering Strait flow, specific to the grid

cp ${berapps}/subApps/BERING_10K_new/ana_psource.h ana_psource.h

#-----------------------
# varinfo
#-----------------------

# Physics-only version: combines ocean defaults with Budgell ice variables

cp ${berapps}/subApps/ocean/varinfo.defaults.dat varinfo.dat      # Start with physics
cat ${berapps}/subApps/IceBudgell/varinfo.ice.dat >> varinfo.dat  # Add ice
sed -i.bu "s/'rdrag'/'rdrg_grid'/" varinfo.dat                    # Rename rdrag
rm varinfo.dat.bu

# The biology versions add biological variables specific to each bio model to the main 
# ocean+ice varinfo.dat we just created.  However, bio state variables need to preceed 
# any variable that include (itrc) in their ID (e.g. tracer2).  I've added text markers in
# the subApp varinfo components to assist with this
#
# Cheat sheet: 
# sed -n '/regexp/,$p'       <-- prints section of file from regex to end
# sed -n '/mark1/,/mark2/p'  <-- prints section of file between two markers

# COBALT

sed -n '/!  ROMS\/TOMS IO NetCDF variables./,/ADD_STATE_VARIABLES_HERE/p' ${berapps}/subApps/ocean/varinfo.defaults.dat > varinfo_cobalt.dat # First part of physics
sed -n '/BEGIN_STATE_VARIABLES/,/END_STATE_VARIABLES/p' ${berapps}/subApps/COBALT/varinfo.cobalt.dat >> varinfo_cobalt.dat # Add COBALT state variables
sed -n '/ADD_STATE_VARIABLES_HERE/,$p' ${berapps}/subApps/ocean/varinfo.defaults.dat >> varinfo_cobalt.dat # Add the rest of the physics
cat ${berapps}/subApps/IceBudgell/varinfo.ice.dat >> varinfo_cobalt.dat # Add ice
sed -n '/END_STATE_VARIABLES/,$p' ${berapps}/subApps/COBALT/varinfo.cobalt.dat >> varinfo_cobalt.dat # Add the rest of the COBALT variables
sed -i.bu "s/'rdrag'/'rdrg_grid'/" varinfo_cobalt.dat # Rename rdrag
rm varinfo_cobalt.dat.bu

# BESTNPZ

sed -n '/!  ROMS\/TOMS IO NetCDF variables./,/ADD_STATE_VARIABLES_HERE/p' ${berapps}/subApps/ocean/varinfo.defaults.dat > varinfo_bestnpz.dat # First part of physics
sed -n '/BEGIN_STATE_VARIABLES/,/END_STATE_VARIABLES/p' ${berapps}/subApps/BESTNPZ/varinfo.bestnpz.dat >> varinfo_bestnpz.dat # Add BESTNPZ state variables
sed -n '/ADD_STATE_VARIABLES_HERE/,$p' ${berapps}/subApps/ocean/varinfo.defaults.dat >> varinfo_bestnpz.dat # Add the rest of the physics
cat ${berapps}/subApps/IceBudgell/varinfo.ice.dat >> varinfo_bestnpz.dat # Add ice
sed -n '/END_STATE_VARIABLES/,$p' ${berapps}/subApps/BESTNPZ/varinfo.bestnpz.dat >> varinfo_bestnpz.dat # Add the rest of the BESTNPZ variables
sed -i.bu "s/'rdrag'/'rdrg_grid'/" varinfo_bestnpz.dat # Rename rdrag
rm varinfo_bestnpz.dat.bu


#-----------------------
# .in files
#-----------------------

# These were copied and then edited manually.  Users should modify their own files on a 
# simulation-specific basis; these are included for reference only.

# ocean_phys.in:   copy from ${berapps}/subApps/BERING_10K_new/ocean_bering10k_new.in with
#                  customized time variables, archiving variables, and input .nc
#                  files
#
# ocean_cobalt.in: copy of ocean_phys.in w/ I/O files modified for COBALT
#
# ice_budgell.in:  copy from ${berapps}/subApps/IceBudgell/ice_budgell.in
#
# bio_cobalt.in:   copy of ${berapps}/subApps/COBALT/bio_Cobalt.in.full.hincasthauri2020
#                  with tracer nudging, climatology, and point sources switched to false.












