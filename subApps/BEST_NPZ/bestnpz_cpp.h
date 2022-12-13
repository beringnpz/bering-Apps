#ifdef BEST_NPZ
# define JELLY             /* Add jellyfish */
# define IRON_LIMIT        /* Add iron  */
# define BENTHIC           /* Add benthos (infauna and detritus) */
# define ICE_BIO           /* Add ice bio (PhL, NO3, NH4) */
# undef CLIM_ICE_1D
# define DIAPAUSE          /* turn on seasonal vertical migration for large copepods */
# define OPTIC_MANIZZA     /* Manizza light attenuation... */
# define COASTAL_ATTEN     /* ... with additional coastal attenuation */
# if defined CARBON
#  define CARBON_FLUX      /* For river fluxes of DIC,TA */
#  define OXYGEN           /* For oxygen cycling */
# endif
# define DIAGNOSTICS_BIO   /* diagnostics on */
# define ANA_ICEBIOBC      /* Analytical ice bio boundary conditions */
# undef ANA_BIOLOGY
# define TCLM_NUDGING      /* Nudging of tracer climatology for iron */
# define ANA_TCLIMA        /* analytical tracers climatology for iron */
# define TCLIMATOLOGY      /* Processing of tracer climatology for iron */
# define ANA_BPFLUX        /* analytical bottom passive tracers fluxes */
# define ANA_SPFLUX        /* analytical surface passive tracers fluxes */
#endif

