!=======================================================================
!   Parameters for BEST_NPZ Biological Model
!      Georgina Gibson, Kelly Kearney, Darren Pilcher
!=======================================================================

        integer, pointer :: idbio(:)  ! Biological tracers
        integer :: iNO3               ! Nitrate
        integer :: iNH4               ! Ammonium
        integer :: iPhS               ! Small Phytoplankton
        integer :: iPhL               ! Large Phytoplankton
        integer :: iMZS               ! Small Microzooplankton
        integer :: iMZL               ! Large Microzooplankton
        integer :: iCop               ! Small Copepods
        integer :: iNCaS              ! On-shelf large copepods
        integer :: iEupS              ! On-shelf Euphausiids
        integer :: iNCaO              ! Off-shelf large copepods
        integer :: iEupO              ! Off-shelf Euphausiids
# ifdef JELLY
        integer :: iJel               ! Jellfish
# endif
        integer :: iDet               ! Slow-sinking Detritus
        integer :: iDetF              ! Fast Sinking Detritus
# ifdef BIOFLUX
        integer :: iBF
# endif
# ifdef CLIM_ICE_1D
       integer :: i1CI
# endif
# ifdef AKT_3D
        integer ::iAKt3
# endif
# ifdef IRON_LIMIT
        integer :: iFe                ! Iron
# endif
# ifdef CARBON
        integer :: iTIC_              ! Total inorganic carbon
        integer :: iTAlk              ! Total alkalinity
# endif
# ifdef OXYGEN
        integer :: iOxyg              ! Dissolved oxygen concentration
# endif
# ifdef BENTHIC
         integer, pointer :: idben(:) ! Benthic tracers
         integer :: iBen
         integer :: iDetBen
# endif
# ifdef ICE_BIO
         integer, pointer :: idice(:) ! Ice tracers
#  ifdef CLIM_ICE_1D
         integer, pointer :: idiceLog(:)  ! Ice tracers
#  endif
         integer :: iIcePhL
         integer :: iIceNO3
         integer :: iIceNH4
         integer :: iIceZ
         integer :: iIceLog
# endif

!**************************
! TODO: start changes here

# ifdef STATIONARY
        integer, pointer :: idbio3(:)
#  include <bestnpz_mod_scalars_i3stat1.h>
# endif
# ifdef STATIONARY2
        integer, pointer :: idbio2(:)
        integer :: i2Stat1
        integer :: i2Stat2
        integer :: i2Stat3
# endif
# ifdef PROD2
        integer, pointer :: idbioP2(:)
        integer :: iIAPrd
        integer :: iBenPrd
  integer :: iXPrd
# endif
# ifdef PROD3
        integer, pointer :: idbioP3(:)

        integer :: iPhSprd            ! Small Phytoplankton Production
        integer :: iPhLprd            ! Large Phytoplankton Production
        integer :: iMZSprd            ! Small Microzooplankton Production
        integer :: iMZLprd            ! Large Microzooplankton Production
        integer :: iCopPrd            ! Copepod production
        integer :: iNCaPrd            ! Neocalanus production
        integer :: iEupPrd            ! Euphausiid production
#  ifdef JELLY
        integer :: iJelPrd            ! Jellyfish  production
#  endif
#  ifdef FEAST
        integer :: iFCopMort
        integer :: iFNCaSMort
        integer :: iFEupSMort
        integer :: iFNCaOMort
        integer :: iFEupOMort
        integer :: iQCopMort
        integer :: iQNCaSMort
        integer :: iQNCaOMort
        integer :: iQEupSMort
        integer :: iQEupOMort
        integer :: iFishOne
        integer :: iFishTwo
        integer :: iFishThree
        integer :: iFishFour
        integer :: iFishFive
        integer :: iFishSix
        integer :: iFishSeven
        integer :: iFishEight
#  endif
# endif