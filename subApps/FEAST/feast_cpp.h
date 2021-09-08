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