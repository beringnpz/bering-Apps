!
!  Set biological tracer indices.
!
      DO i=1,NBT
        idbio(i)=ic+i
      END DO
      iNO3=ic+1
      iNH4=ic+2
      iPhS=ic+3
      iPhL=ic+4
      iMZS=ic+5
      iMZL=ic+6
      iCop=ic+7
      iNCaS=ic+8
      iEupS=ic+9
      iNCaO=ic+10
      iEupO=ic+11
      iDet=ic+12
      iDetF=ic+13
# ifdef JELLY
      iJel=ic+14
      ic=ic+14
# else
      ic=ic+13
# endif

# ifdef IRON_LIMIT
      iFe=ic+1
      ic=ic+1
# endif
# ifdef CARBON
      iTIC_=ic+1
      iTAlk=ic+2
      ic=ic+2
# endif
# ifdef OXYGEN
      iOxyg=ic+1
      ic=ic+1
# endif 
# ifdef CLIM_ICE_1D
      i1CI=ic+1
      ic=ic+1
# endif
# ifdef AKT_3D
      iAKt3=ic+1
      ic=ic+1
# endif

# ifdef BIOFLUX
      iBF=1
# endif

# ifdef STATIONARY
      DO i=1,NBTS
        idbio3(i)=i
      END DO
#  include <bestnpz_mod_scalars_i3stat2.h>
# endif
# ifdef STATIONARY2
      DO i=1,NBTS2
        idbio2(i)=i
      END DO
      i2Stat1 = 1
      i2Stat2 = 2
      i2Stat3 = 3
# endif
# ifdef PROD3

      DO i=1,NBPT3
        idbioP3(i)=ic+i
      END DO

      iPhSprd = 1
      iPhLprd = 2
      iMZSprd = 3
      iMZLprd = 4
      iCopPrd = 5
      iNCaPrd = 6
      iEupPrd = 7
#  ifdef JELLY
      iJelPrd = 8
#  endif
#  ifdef FEAST
      iFCopMort  = 9
      iFNCaSMort  = 10
      iFEupSMort  = 11
      iQCopMort  = 12
      iQNCaSMort = 13
      iQNCaOMort = 14
      iQEupSMort = 15
      iQEupOMort = 16
      iFishOne   = 17
      iFishTwo   = 18
      iFishThree = 19
      iFishFour  = 20
      iFishFive  = 21
      iFishSix   = 22
      iFishSeven = 23
      iFishEight = 24
      iFNCaOMort = 25
      iFEupOMort = 26
#  endif
# endif
# ifdef PROD2
      DO i=1,NBPT2
        idbioP2(i)=ic+i
      END DO
      iBenPrd = 1
      iIAPrd = 2
      iXPrd =3
# endif

# ifdef BENTHIC
      DO i=1,NBEN
        idben(i)=i
      END DO
      iBen=1
      iDetBen=2
# endif
# ifdef ICE_BIO
      DO i=1,NIB
        idice(i)=i
      END DO
#  ifdef CLIM_ICE_1D
      iIceLog=1

      iIcePhL=1
      iIceNO3=2
      iIceNH4=3
      iIceZ=4
#  else
      iIcePhL=1
      iIceNO3=2
      iIceNH4=3
#  endif
# endif