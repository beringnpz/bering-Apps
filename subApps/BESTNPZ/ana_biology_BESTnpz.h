!
!=======================================================================
!                                                                      !
!  This routine sets initial conditions for biological tracer fields   !
!  using analytical expressions for the BEST NPZ model .                                       !
!                                                                      !
!=======================================================================
!



#ifdef SPINUPBIO

!
!-----------------------------------------------------------------------
! This set of initial conditions starts with an empty Bering Sea shelf; 
! NO3 is only present at depth, with no NH4 or detritus anywhere.  
! Living critters are seeded with only a tiny amount to allow for future 
! growth.  This start condition is intended to allow the model domain to 
! move towards its own internally-regulated nutrient/benthos steady-
! state
!-----------------------------------------------------------------------
!

      DO i=IstrR,IendR
        DO j=JstrR,JendR
          
# ifdef IRON_LIMIT
          ! Iron top/bottom values set based on bottom (matches nudging)
          FeSurf = CalcLinearCapped(Feinh, Feinlo, Feoffh, Feofflo, GRID(ng)%h(i,j))
          FeDeep = CalcLinearCapped(Feinh, Feinhi, Feoffh, Feoffhi, GRID(ng)%h(i,j))
# endif
          
          DO k=1,N(ng)
# ifdef IRON_LIMIT

            ! Iron: depth-dependant linear-capped profile (same as nudging)
            
            t(i,j,k,1,iFe) = CalcLinearCapped(-50.0_r8, FeSurf, -300.0_r8, FeDeep, GRID(ng)%z_r(i,j,k))
# endif
            ! Nitrate: depth-dependant linear-capped profile (0 above 100, 40 below 300
            ! based very roughly on World Ocean Atlas 2013 January climatological values 
            ! at bottom depth in the Bering Sea) 
            
            t(i,j,k,1,iNO3) = CalcLinearCapped(-100.0_r8, 0.0_r8, -300.0_r8, 40.0_r8, GRID(ng)%z_r(i,j,k))
            
            ! NH4: 0
            
            t(i,j,k,1,iNH4) = 0.0_r8
            
            ! Phytoplankton and zooplankton: constant seed
            
            t(i,j,k,1,iPhS)  = eps
            t(i,j,k,1,iPhL)  = eps
            t(i,j,k,1,iMZS)  = 0.0_r8
            t(i,j,k,1,iMZL)  = eps
            t(i,j,k,1,iCop)  = eps
            t(i,j,k,1,iNCaS) = eps
            t(i,j,k,1,iNCaO) = eps
            t(i,j,k,1,iEupS) = eps
            t(i,j,k,1,iEupO) = eps
# ifdef JELLY
            t(i,j,k,1,iJel) = eps
# endif

            ! Detritus: nothing
            
            t(i,j,k,1,iDet) =  0.0_r8
	          t(i,j,k,1,iDetF) =  0.0_r8

          END DO
# ifdef BENTHIC
          DO k = 1,NBL(ng)
            
            ! Benthos
            
            bt(i,j,k,1,iBen) = eps
            
            ! Benthic detritus
            
		        bt(i,j,k,1,iDetBen) = 0.0_r8
            
          END DO
# endif        
# ifdef ICE_BIO

          ! Ice: start with no-ice conditions
          
#  ifdef CLIM_ICE_1D
          it(i,j,1,iIcePhL)  =  0.0_r8      
          it(i,j,1,iIceNO3)  =  0.0_r8      
          it(i,j,1,iIceNH4)  =  0.0_r8      
		      itL(i,j,1,iIceLog) = -1.0_r8    
#  elif defined BERING_10K
          IcePhL(i,j,1) =  0.0_r8
          IceNO3(i,j,1) =  0.0_r8
          IceNH4(i,j,1) =  0.0_r8
          IceLog(i,j,1) = -1.0_r8
#  endif
# endif

        END DO
      END DO
      

#else
!
!-----------------------------------------------------------------------
! This set of initial conditions reflects those used in the original 
! Bering10K runs.  
!-----------------------------------------------------------------------
!
      
      DO i=IstrR,IendR
        DO j=JstrR,JendR
          
# ifdef IRON_LIMIT
          ! Iron top/bottom values set based on bottom (matches nudging)
          FeSurf = CalcLinearCapped(Feinh, Feinlo, Feoffh, Feofflo, GRID(ng)%h(i,j))
          FeDeep = CalcLinearCapped(Feinh, Feinhi, Feoffh, Feoffhi, GRID(ng)%h(i,j))
# endif
          
          DO k=1,N(ng)
# ifdef IRON_LIMIT

            ! Iron: depth-dependant linear-capped profile (same as nudging)
            
            t(i,j,k,1,iFe) = CalcLinearCapped(-50.0_r8, FeSurf, -300.0_r8, FeDeep, GRID(ng)%z_r(i,j,k))
# endif
            ! Nitrate: depth-dependant linear-capped profile
            
            t(i,j,k,1,iNO3) = CalcLinearCapped(-100.0_r8, 18.0_r8, -300.0_r8, 30.0_r8, GRID(ng)%z_r(i,j,k))
            
            ! NH4: 0
            
            t(i,j,k,1,iNH4) = 0.0_r8
            
            ! Phytoplankton and small zooplankton: constant profiles
            
            t(i,j,k,1,iPhS) = 5.0_r8
            t(i,j,k,1,iPhL) = 1.0_r8
            t(i,j,k,1,iMZS) = 0.0_r8
            t(i,j,k,1,iMZL) = 1.0_r8
            t(i,j,k,1,iCop) = 0.1_r8
            
            ! Large copepods: Offshore Neocalanus are only added to grid cells deeper than 
            ! 400m, with a concentration at mid-depth under the assumption that we're 
            ! starting in January and they would be in diapause at that time.  Onshore 
            ! Calanus marshallae are added only to grid cells shallower than 200m, and 
            ! placed in the top 5 layers (regardless of model resolution).

	     		  if (z_r(i,j,1) .le. -400.0_r8) then 
	            if (z_r(i,j,k).le.-400.and.z_r(i,j,k).ge.-800) THEN
                t(i,j,k,1,iNCaO) = 0.1_r8
	            endif
	     		  elseif (z_r(i,j,1) .ge. -200.0_r8) then 
  	          if(k.le.5)THEN
                t(i,j,k,1,iNCaS) =  8.0_r8
	            endif
	          endif

            ! Euphausiids: Constant concentration, split into onshore (<100m) and offshore 
            ! (<100m) groups.
            
	          if (-z_r(i,j,1) .gt. 100.0_r8 ) then 
              t(i,j,k,1,iEupO) =  0.1_r8
	            t(i,j,k,1,iEupS) =  0_r8
	          else
              t(i,j,k,1,iEupS) =  0.1_r8
	            t(i,j,k,1,iEupO) =  0_r8
	          endif
            
# ifdef JELLY
            ! Jellyfish: depth-dependant linear-capped profile
            
            t(i,j,k,1,iJel) = CalcLinearCapped(-100.0_r8, 0.1_r8, -300.0_r8, eps, GRID(ng)%z_r(i,j,k))
# endif

            ! Detritus: nearly nothing
            
            t(i,j,k,1,iDet) =  eps
	          t(i,j,k,1,iDetF) =  eps
            

          END DO
# ifdef BENTHIC
          DO k = 1,NBL(ng)
            
            ! Benthos: start high (note: due to a missing check for analytical conditions, 
            ! these were overrided by input file initial conditions in most earlier 
            ! Bering10K runs)
            
            bt(i,j,k,1,iBen) = 8000.0_r8 
            
            ! Benthic detritus
            
		        bt(i,j,k,1,iDetBen) = 500.0_r8
            
          END DO
# endif        
# ifdef ICE_BIO

          ! Ice: start with no-ice conditions
          
#  ifdef CLIM_ICE_1D
          it(i,j,1,iIcePhL)  =  0.0_r8      
          it(i,j,1,iIceNO3)  =  0.0_r8      
          it(i,j,1,iIceNH4)  =  0.0_r8      
		      itL(i,j,1,iIceLog) = -1.0_r8    
#  elif defined BERING_10K
          IcePhL(i,j,1) =  0.0_r8
          IceNO3(i,j,1) =  0.0_r8
          IceNH4(i,j,1) =  0.0_r8
          IceLog(i,j,1) = -1.0_r8
#  endif
# endif

        END DO
      END DO

#endif

!
!-----------------------------------------------------------------------
! Additional setup, applicable to all compilation options above
!-----------------------------------------------------------------------
!

      ! Set other time index, and add a minimum eps value for all 3D variables

      DO i=IstrR,IendR
        DO j=JstrR,JendR
          DO k=1,N(ng)
            DO is=1,NBT
              itrc=idbio(is)
!               t(i,j,k,1,itrc) = MAX(t(i,j,k,1,itrc),eps)
              t(i,j,k,2,itrc) = t(i,j,k,1,itrc)
            END DO
          END DO
#ifdef BENTHIC
          DO k=1,NBL(ng)
		        bt(i,j,k,2,iBen)    = bt(i,j,k,1,iBen)
		        bt(i,j,k,2,iDetBen) = bt(i,j,k,1,iDetBen)
          END DO
#endif
#ifdef ICE_BIO
# ifdef CLIM_ICE_1D
          it(i,j,2,iIcePhL)  = it(i,j,1,iIcePhL)    
          it(i,j,2,iIceNO3)  = it(i,j,1,iIceNO3)    
          it(i,j,2,iIceNH4)  = it(i,j,1,iIceNH4)    
		      itL(i,j,2,iIceLog) = itL(i,j,1,iIceLog) 
# elif defined BERING_10K
          IcePhL(i,j,2) = IcePhL(i,j,1)
          IceNO3(i,j,2) = IceNO3(i,j,1)
          IceNH4(i,j,2) = IceNH4(i,j,1)
          IceLog(i,j,2) = IceLog(i,j,1)
# endif
#endif
        END DO
      END DO
      