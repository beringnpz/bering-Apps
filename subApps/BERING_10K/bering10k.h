/*
** svn $Id: basin.h 8 2007-02-06 19:00:29Z arango $
*******************************************************************************
** Copyright (c) 2002-2009 The ROMS/TOMS Group
**
**   Licensed under a MIT/X style license
**
**   See License_ROMS.txt
**
*******************************************************************************
**
**  Options for BERING_10K simulation.  
**
**  This file originated from the nep5.h header file, and was then modified for
**  the smaller BERING_10K domain. 
**  
**  By default this will run a physics-only variant.  To add the BESTNPZ 
**  biogeochmistry, add "-DBESTNPZ" to MY_CPP_FLAGS; to add the full 
**  FEAST food web, add "-DBESTNPZ -DFEAST"
*/


#define  BERING_10K     /* Application flag */
                        /* Usually, this would be set via the ROMS_APPLICATION environment 
                           variable within a build script or makefile.  BERING_10K is a 
                           special case in that it relies on two application flags: NEP5 
                           and BERING_10K.  When used this way, this header file should be
                           renamed nep5.h. */

#undef NETCDF4              // use classic netCDF 
#undef PARALLEL_IO          // no parallel input/output
#undef OFFLINE_FLOATS       // could be used for floats in offline version... but not now

/* general */

#define CURVGRID            // use curvilinear coordinates
#define MASKING             // use land/sea masking
#define NONLIN_EOS          // nonlinear equation of state
#define SOLVE3D             // 3D primitive equations
#define SALINITY            // have salinity
#ifdef SOLVE3D               
# undef SPLINES             // turn off option for parabolic splines reconstruction of vertical derivatives
#endif                      
#undef FLOATS               // toggle on/off floats
#define STATIONS            // toggle on/off stations output
#undef WET_DRY              // no wetting/drying of grid cells
                          
#undef T_PASSIVE            // no passive tracers
#ifdef T_PASSIVE            
# define ANA_PASSIVE        // ... but if on, use analytical initial conditions for them
#endif                 
                
/* salinity nudging */ 
                
#define SCORRECTION         // freshwater flux correction

/* ice */

#ifdef SOLVE3D
# define  ICE_MODEL         // Turn on default ice model with...
# ifdef ICE_MODEL           
#  define  ICE_THERMO       // ... ice thermodynamic component
#  define  ICE_MK           // ... Mellor-Kantha thermodynamics
#  undef   ICE_ALB_EC92
#  undef   ICE_SMOOTH
#  define  ICE_MOMENTUM     // ... ice momentum component
#  define  ICE_MOM_BULK     // ... something related to ice-water stress computation
#  define  ICE_EVP          // ... elastic-viscous-plastic rheology
#  define  ICE_ADVECT       // ... advect ice tracers
#  define  ICE_SMOLAR       // ... MPDATA advection scheme
#  define  ICE_UPWIND       // ... upwind advection scheme
#  define  ICE_BULK_FLUXES  // ... ice in bulk flux computation
#  define  ANA_AIOBC        // ... analytical aice boundary conditions (defaults to 0)
#  define  ANA_HIOBC        // ... analytical hice boundary conditions (defaults to 0)
#  define  ANA_HSNOBC       // ... analytical snow boundary conditions (defaults to 0)
# endif
#endif

/* output stuff */
 
#define NO_WRITE_GRID       // Don't write grid arrays
#undef OUT_DOUBLE           // Don't force double precision
#define RST_SINGLE          // Use single precision for restart files
#define AVERAGES            // Write out averages output
#undef AVERAGES2            // No secondary averages output
#ifdef SOLVE3D
# undef AVERAGES_DETIDE     // Don't apply detiding filter
# define AVERAGES_AKT       // add AKt output to averages file
# define AVERAGES_AKS       // add AKs output to averages file
# define AVERAGES_AKV       // add AKv output to averages file
# define AVERAGES_FLUXES    // add a bunch of variables related to surface fluxes to averages file
# undef AVERAGES_QUADRATIC  // Donj't add quadratic terms to averages file
# undef DIAGNOSTICS_TS      // Don't add tracer diagnostics to averages file
#endif
#undef DIAGNOSTICS_UV       // Don't add momentum diagnostics to averages file
 
/* advection, dissipation, pressure grad, etc. */
 
#ifdef SOLVE3D
# define DJ_GRADPS          // use splines density Jacobian (Shchepetkin, 2000) in pressure graident term
#endif
 
#define UV_ADV              // turn on advection terms
#define UV_COR              // turn on Coriolis terms
#define UV_SADVECTION       // turn on splines vertical advection
 
#ifdef SOLVE3D
# define TS_C4HADVECTION    // use 4th-order centered horizontal advection
# define TS_C4VADVECTION    // use 4th-order centered vertical advection
# undef TS_MPDATA           // Don't use recursive MPDATA 3D advection
#endif
 
#define UV_VIS2             // turn on harmonic horizontal mixing, momentum 
#define UV_SMAGORINSKY      // turn on Smagorinky-like viscosity 
#define VISC_3DCOEF         // turn on time-invarant horizontal viscosity at rho-points
#define MIX_S_UV            // mixing along constant S-surfaces 
#define VISC_GRID           // scale viscosity coefficient by grid size
#define SPONGE              // allows for enhanced viscosity/diffusion areas

#ifdef SOLVE3D
# define TS_DIF2            // turn on harmonic horizontal mixing, tracers 
# define MIX_GEO_TS         // mix along geopotential (constant z) surfaces
# define DIFF_GRID          // scales diffusion coefficients by grid size
#endif
 
 
/* vertical mixing */
 
#ifdef SOLVE3D
# define SOLAR_SOURCE       // solar radiation source term
 
# define LMD_MIXING         // Use Large et al. (1994) interior closure with ...
# ifdef LMD_MIXING
#  define LMD_RIMIX         // ... diffusivity due to shear instability
#  define LMD_CONVEC        // ... convective mixing due to shear instability
#  define LMD_SKPP          // ... surface boundary layer KPP mixing
#  undef LMD_BKPP           // ... no bottom boundary KPP mixing
#  define LMD_NONLOCAL      // ... nonlocal transport
#  define LMD_SHAPIRO       // ... Shapiro filtering boundary layer depth
#  undef LMD_DDMIX          // ... no double-diffusive mixing
# endif
 
# undef GLS_MIXING          // Don't use alternative mixing schemes
# undef MY25_MIXING

# if defined GLS_MIXING || defined MY25_MIXING
#  define KANTHA_CLAYSON
#  define N2S2_HORAVG
# endif
#endif
 
/* surface forcing */
 
#ifdef SOLVE3D
# define CORE_FORCING       // input humidity is specific humidity, not relative
# define BULK_FLUXES        // use bulk fluxes computation...
# define CCSM_FLUXES        // ... specifically, the CCSM version of bulk fluxes computation
# if defined BULK_FLUXES || defined CCSM_FLUXES
#  define LONGWAVE_OUT      // compute outgoing longwave radiation (with downward provided as input)
#  define DIURNAL_SRFLUX    // add diurnal cycle to daily-averaged shortwave input
#  define EMINUSP           // compute evap minus precip
#  undef ANA_SRFLUX         // no analytical surface fluxes
#  undef ALBEDO             // Don't calculate shortwave using global albedo equation (use input plus diurnal instead)
#  define ALBEDO_CURVE      // albedo function of lat from Large and Yeager
#  undef LONGWAVE           // Not using net longwave
# endif
#endif
 
/* surface and side corrections */
 
#ifdef SOLVE3D
# undef SRELAXATION         // No salinity relaxation
# undef QCORRECTION         // No net heat flux correction
#endif
 
#ifdef SOLVE3D
# undef TCLIMATOLOGY        // No tracer climatology
# undef TCLM_NUDGING        // No tracer nudging
#endif
 
/* point sources (rivers, line sources) */
/* Using Runoff instead now             */

#ifdef SOLVE3D
# define RUNOFF             // Add runoff as an additional rain field
# define UV_PSOURCE         // Include momentum point sources (but not for rivers, for Bering Strait)
# define ANA_PSOURCE        // Use analytical point sources
# undef TS_PSOURCE          // No tracer point sources
#endif
 
/* tides */
 
#define LTIDES              // Turn on tides (Not a ROMS CPP option, just used here to turn some stuff on/off in bulk)
#ifdef LTIDES
# undef FILTERED            // don't turn on filters... may need on eventually  KAK: what filters, exactly?
# define SSH_TIDES          // impose tidal elevation
# define UV_TIDES           // impose tidal currents
# define ADD_FSOBC          // add tidal elevation to processed OBC data
# define ADD_M2OBC          // add tidal currents  to processed OBC data
# undef RAMP_TIDES
# define TIDES_ASTRO
# define POT_TIDES          // impose potential tides
# define UV_LDRAG           // turn on linear bottom friction
# define RDRG_GRID          // read bottom drag coefficients from grid file
# define DRAG_LIMITER       // quadratic bottom stress  KAK: customization?
# undef UV_QDRAG
#else
# define UV_QDRAG           // quadratic bottom stress
#endif
 
/* Boundary conditions...careful with grid orientation */
/* BERING_10K: north = Russia (northwest-ish), closed
               south = Gulf of Alaska (southeast-ish), open
               east = Bering Strait, closed but with momentum point source (see ana_psource)
               west = North Pacific (south of Aleutians), open */
 
#define EASTERN_WALL        // closed eastern
#define NORTHERN_WALL       // closed northern
#undef WESTERN_WALL
#undef SOUTHERN_WALL
 
#define RADIATION_2D
 
#ifndef NORTHERN_WALL
# define NORTH_FSCHAPMAN
# define NORTH_M2FLATHER
# ifdef SOLVE3D
#  define NORTH_M3RADIATION
#  define NORTH_M3NUDGING
#  define NORTH_TRADIATION
#  define NORTH_TNUDGING
#  define NORTH_MIGRADIENT
# endif
#endif
 
#ifndef WESTERN_WALL        // western boundary conditions
# define WEST_FSCHAPMAN     // ... free surface Chapman
# define WEST_M2FLATHER     // ... 2D momentum Flather
# ifdef SOLVE3D
#  define WEST_M3RADIATION  // ... 3D momentum radiation+nudging
#  define WEST_M3NUDGING
#  define WEST_TRADIATION   // ... 3D tracers radiation+nudging
#  define WEST_TNUDGING     
#  define WEST_MIGRADIENT   // ... Ice tracers gradient
# endif
#endif
 
#ifndef SOUTHERN_WALL       // southern boundary conditions
# define SOUTH_FSCHAPMAN    // ... free surface Chapman
# define SOUTH_M2FLATHER    // ... 2D momentum Flather
# ifdef SOLVE3D
#  define SOUTH_M3RADIATION // ... 3D momentum radiation+nudging
#  define SOUTH_M3NUDGING
#  define SOUTH_TRADIATION  // ... 3D tracers radiation+nudging
#  define SOUTH_TNUDGING
#  define SOUTH_MIGRADIENT  // ... Ice tracers gradient
# endif
#endif
 
#ifndef EASTERN_WALL
# define EAST_FSCHAPMAN
# define EAST_M2FLATHER
# ifdef SOLVE3D
#  define EAST_M3RADIATION
#  define EAST_M3NUDGING
#  define EAST_TRADIATION
#  define EAST_TNUDGING
#  define EAST_MIGRADIENT
# endif
#endif
 
/* roms quirks */
 
#ifdef SOLVE3D
# define ANA_BSFLUX
# define ANA_BTFLUX
#else
# define ANA_SMFLUX
#endif

/*
**  Biological model options.
*/

/* In order to allow use of this header file with different biology, I'm 
   leaving out any explicit define/undef options for BESTNPZ and FEAST.  
   These will be defined externally via MY_CPP_FLAGS.
*/ 

#undef NEMURO
#undef BIO_GOANPZ        /* Sarah Hinckley's 11 box model */

#if defined BEST_NPZ || defined BIO_GOANPZ || defined PASSIVE_TRACERS
# undef  BIOFLUX           /* sum Nitrogen fluxes between boxes */
# define ANA_BIOLOGY       /* analytical biology initial conditions */
# define ANA_BPFLUX        /* analytical bottom passive tracers fluxes */
# define ANA_SPFLUX        /* analytical surface passive tracers fluxes */
# define DIAPAUSE          /* Enable large copepod seasonal vertical migration */
# undef FLOAT_VWALK
#endif

#if defined NEMURO
# undef ANA_BIOLOGY        /* analytical biology initial conditions */
# define ANA_BPFLUX        /* analytical bottom passive tracers fluxes */
# define ANA_SPFLUX        /* analytical surface passive tracers fluxes */
# define IRON_LIMIT        /* Add iron as passive 11th tracer */
# define IRON_RELAX
# undef  IRON_RSIN
# define BIO_SEDIMENT
# define HOLLING_GRAZING
# undef  IVLEV_EXPLICIT
# undef  ANA_BIOSWRAD
# undef  DIAGNOSTICS_BIO
# undef  BIO_SEDIMENT
#endif

/* TODO: Need to clean this up, and determine which options are needed for 
   H16 compilation vs K20 compilation, and which are thoroughly deprecated */

#ifdef BEST_NPZ

# define BIO_NUDGE_ONLY    /* Fe nudging */

# undef LIMIT_BIO_AKT
# if !defined(NEWSHADE) && !defined(NEWSHADESHALLOW) && !defined(COKELET)
#   define COKELET         /* Only relevant in H16 code, removed from K20  */
# endif
# undef KODIAK_IRAD        /* Generate irradiance with curve matching Kodiak data 
                              Else use Sarah Hinckly original code   */
# define JELLY
# define IRON_LIMIT        /* Add iron  */
# define BENTHIC           /*FENNEL or BENTHIC or TRAP*/
# define ICE_BIO
# undef CLIM_ICE_1D
# define TCLM_NUDGING      /* Nudging of tracer climatology for iron */
# define ANA_TCLIMA        /* analytical tracers climatology for iron */
# define TCLIMATOLOGY      /* Processing of tracer climatology for iron */
# if defined CARBON
#  define CARBON_FLUX      /* For river fluxes of DIC,TA */
#  define OXYGEN           /* For oxygen cycling */
#  define STATIONARY2
# endif
# define STATIONARY
# define GPPMID            /* gross primary production at mid-point of layer */
# define PI_CONSTANT       /* PI curve: use constant-alpha version */
# undef PROD3 
# undef PROD2
# undef SINKVAR            /* for variable sinking rate*/
# undef DENMAN
# undef CORRECT_TEMP_BIAS  /* corrects ROMS temp for biology only */
#endif

/*
#undef  OFFLINE_BIOLOGY     define if offline simulation of bio tracers 
#if defined OFFLINE_BIOLOGY
# define AKSCLIMATOLOGY     Processing of AKS climatology 
# undef ANA_AKSCLIMA        Processing of AKS climatology 
#endif
*/

/* Here's the new stuff for feast*/

#ifdef FEAST
# ifdef PROD3
#  define FEAST_DAT        /* only use this if using 60-layer model */
# endif
# undef FEAST_DAT          /* so undef in the 10-layer model */
# define FEAST_FORCING
# define T_PASSIVE
# define ANA_PASSIVE
# undef ANA_PASSIVE
# undef FLOATS
# define ANA_BPFLUX
# define ANA_SPFLUX
# undef FEAST_NOEXCHANGE
#endif



