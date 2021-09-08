#ifdef BIO_COBALT
# undef TS_MPDATA
# define TS_HSIMT
#elif defined SOLVE3D
# define TS_U3HADVECTION
# define TS_C4VADVECTION
#endif

/* #define DEBUG_COBALT */
/*#define COBALT_CONSERVATION_TEST */
/*#define COBALT_NOSOURCE */
/*#define COBALT_DO_NOTHING  */

#if defined BIO_COBALT
# undef FILTERED
# undef AVERAGES2
# define OPTIC_MANIZZA
# define COBALT_MINERALS
# undef COBALT_PHOSPHORUS
# define COBALT_IRON
# define NO_IRON_COAST
# define COBALT_CARBON
# define DIAGNOSTICS_BIO
/*# define BENTHIC  */
/*# define TIMESERIES */
# undef ANA_ALK_DIC
# undef ANA_BIOLOGY        /* analytical biology initial conditions */
# define ANA_BPFLUX        /* analytical bottom passive tracers fluxes */
# define ANA_SPFLUX        /* analytical surface passive tracers fluxes */
# define COASTDIAT
#endif