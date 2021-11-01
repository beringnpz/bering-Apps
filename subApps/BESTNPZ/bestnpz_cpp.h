#ifdef BEST_NPZ
# define JELLY             /* Add jellyfish */
# define IRON_LIMIT        /* Add iron  */
# define BENTHIC           /* Add benthos (infauna and detritus) */
# define ICE_BIO           /* Add ice bio (PhL, NO3, NH4) */
# undef CLIM_ICE_1D
# define TCLM_NUDGING      /* Nudging of tracer climatology for iron */
# define ANA_TCLIMA        /* analytical tracers climatology for iron */
# define TCLIMATOLOGY      /* Processing of tracer climatology for iron */
# if defined CARBON
#  define CARBON_FLUX      /* For river fluxes of DIC,TA */
#  define OXYGEN           /* For oxygen cycling */
# endif
# define DIAGNOSTICS_BIO   /* diagnostics on */
# define GPPMID            /* use layer midpoint (instead of layer integral) for GPP */
# define PI_CONSTANT       /* constant alpha in PI curve */
#endif

