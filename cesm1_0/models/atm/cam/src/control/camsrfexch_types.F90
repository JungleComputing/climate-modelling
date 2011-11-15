module camsrfexch_types
!-----------------------------------------------------------------------
!
! Module to handle data that is exchanged between the CAM atmosphere
! model and the surface models (land, sea-ice, and ocean).
!
!-----------------------------------------------------------------------
!
! USES:
!
  use shr_kind_mod,  only: r8 => shr_kind_r8, r4 => shr_kind_r4
  use constituents,  only: pcnst
  use ppgrid,        only: pcols, begchunk, endchunk
  use phys_grid,     only: get_ncols_p, phys_grid_initialized
  use abortutils,    only: endrun
  use infnan,        only: inf
  use cam_logfile,   only: iulog

  implicit none

!----------------------------------------------------------------------- 
! PRIVATE: Make default data and interfaces private
!----------------------------------------------------------------------- 
  private     ! By default all data is private to this module
!
! Public interfaces
!
  public atm2hub_alloc              ! Atmosphere to surface data allocation method
  public hub2atm_alloc              ! Merged hub surface to atmosphere data allocation method
  public hub2atm_setopts            ! Set options to allocate optional parts of data type
!
! Public data types
!
  public surface_state              ! Data from atmosphere
  public srfflx_state               ! Merged surface data

!---------------------------------------------------------------------------
! This is the data that is sent from the atmosphere to the surface models
!---------------------------------------------------------------------------

  type surface_state
     integer  :: lchnk               ! chunk index
     integer  :: ncol                ! number of columns in chunk
     real(r8) :: tbot(pcols)         ! bot level temperature
     real(r8) :: zbot(pcols)         ! bot level height above surface
     real(r8) :: ubot(pcols)         ! bot level u wind
     real(r8) :: vbot(pcols)         ! bot level v wind
     real(r8) :: qbot(pcols,pcnst)   ! bot level specific humidity
     real(r8) :: pbot(pcols)         ! bot level pressure
     real(r8) :: rho(pcols)          ! bot level density	
     real(r8) :: netsw(pcols)        !	
     real(r8) :: flwds(pcols)        ! 
     real(r8) :: precsc(pcols)       !
     real(r8) :: precsl(pcols)       !
     real(r8) :: precc(pcols)        ! 
     real(r8) :: precl(pcols)        ! 
     real(r8) :: soll(pcols)         ! 
     real(r8) :: sols(pcols)         ! 
     real(r8) :: solld(pcols)        !
     real(r8) :: solsd(pcols)        !
     real(r8) :: srfrad(pcols)       !
     real(r8) :: thbot(pcols)        ! 
     real(r8) :: co2prog(pcols)      ! prognostic co2
     real(r8) :: co2diag(pcols)      ! diagnostic co2
     real(r8) :: psl(pcols)
     real(r8) :: bcphiwet(pcols)     ! wet deposition of hydrophilic black carbon
     real(r8) :: bcphidry(pcols)     ! dry deposition of hydrophilic black carbon
     real(r8) :: bcphodry(pcols)     ! dry deposition of hydrophobic black carbon
     real(r8) :: ocphiwet(pcols)     ! wet deposition of hydrophilic organic carbon
     real(r8) :: ocphidry(pcols)     ! dry deposition of hydrophilic organic carbon
     real(r8) :: ocphodry(pcols)     ! dry deposition of hydrophobic organic carbon
     real(r8) :: dstwet1(pcols)      ! wet deposition of dust (bin1)
     real(r8) :: dstdry1(pcols)      ! dry deposition of dust (bin1)
     real(r8) :: dstwet2(pcols)      ! wet deposition of dust (bin2)
     real(r8) :: dstdry2(pcols)      ! dry deposition of dust (bin2)
     real(r8) :: dstwet3(pcols)      ! wet deposition of dust (bin3)
     real(r8) :: dstdry3(pcols)      ! dry deposition of dust (bin3)
     real(r8) :: dstwet4(pcols)      ! wet deposition of dust (bin4)
     real(r8) :: dstdry4(pcols)      ! dry deposition of dust (bin4)
  end type surface_state

!---------------------------------------------------------------------------
! This is the merged state of sea-ice, land and ocean surface parameterizations
!---------------------------------------------------------------------------

  type srfflx_state
     integer  :: lchnk                   ! chunk index
     integer  :: ncol                    ! number of active columns
     real(r8) :: asdir(pcols)            ! albedo: shortwave, direct
     real(r8) :: asdif(pcols)            ! albedo: shortwave, diffuse
     real(r8) :: aldir(pcols)            ! albedo: longwave, direct
     real(r8) :: aldif(pcols)            ! albedo: longwave, diffuse
     real(r8) :: lwup(pcols)             ! longwave up radiative flux
     real(r8) :: lhf(pcols)              ! latent heat flux
     real(r8) :: shf(pcols)              ! sensible heat flux
     real(r8) :: wsx(pcols)              ! surface u-stress (N)
     real(r8) :: wsy(pcols)              ! surface v-stress (N)
     real(r8) :: tref(pcols)             ! ref height surface air temp
     real(r8) :: qref(pcols)             ! ref height specific humidity 
     real(r8) :: ts(pcols)               ! merged surface temp 
     real(r8) :: sst(pcols)              ! sea surface temp
     real(r8) :: snowhland(pcols)        ! snow depth (liquid water) over land
     real(r8) :: snowhice(pcols)         ! snow depth (liquid water) over ice
     real(r8) :: fco2_lnd(pcols)         ! co2 flux from lnd
     real(r8) :: fco2_ocn(pcols)         ! co2 flux from ocn
     real(r8) :: fdms(pcols)             ! dms flux
     real(r8) :: landfrac(pcols)         ! land area fraction
     real(r8) :: icefrac(pcols)          ! sea-ice areal fraction
     real(r8) :: ocnfrac(pcols)          ! ocean areal fraction
     real(r8), pointer, dimension(:) :: ram1  !aerodynamical resistance (s/m) (pcols)
     real(r8), pointer, dimension(:) :: fv    !friction velocity (m/s) (pcols)
     real(r8) :: cflx(pcols,pcnst)      ! constituent flux (evap)
     real(r8) :: ustar(pcols)            ! atm/ocn saved version of ustar
     real(r8) :: re(pcols)               ! atm/ocn saved version of re
     real(r8) :: ssq(pcols)              ! atm/ocn saved version of ssq
     real(r8), pointer, dimension(:,:) :: depvel ! deposition velocities
  end type srfflx_state

  logical :: dust = .false.     ! .true. => aerosol dust package is being used

!===============================================================================
CONTAINS
!===============================================================================

!----------------------------------------------------------------------- 
! 
! BOP
!
! !IROUTINE: hub2atm_alloc
!
! !DESCRIPTION:
!
!   Allocate space for the surface to atmosphere data type. And initialize
!   the values.
! 
!-----------------------------------------------------------------------
!
! !INTERFACE
!
  subroutine hub2atm_alloc( srfflx_state2d )
    use seq_drydep_mod, only : lnd_drydep, n_drydep
!
!!ARGUMENTS:
!
   type(srfflx_state), pointer ::  srfflx_state2d(:)     ! Merged surface state
!
!!LOCAL VARIABLES:
!
    integer :: c        ! chunk index
    integer :: ierror   ! Error code
!----------------------------------------------------------------------- 
! 
! EOP
!
    if ( .not. phys_grid_initialized() ) call endrun( "HUB2ATM_ALLOC error: phys_grid not called yet" )
    allocate (srfflx_state2d(begchunk:endchunk), stat=ierror)
    if ( ierror /= 0 )then
      write(iulog,*) 'Allocation error: ', ierror
      call endrun('HUB2ATM_ALLOC error: allocation error')
    end if

    do c = begchunk,endchunk
       nullify(srfflx_state2d(c)%ram1)
       nullify(srfflx_state2d(c)%fv)
    enddo  
    if ( dust ) then
       do c = begchunk,endchunk 
          allocate (srfflx_state2d(c)%ram1(pcols), stat=ierror)
          if ( ierror /= 0 ) call endrun('HUB2ATM_ALLOC error: allocation error ram1')
          allocate (srfflx_state2d(c)%fv(pcols), stat=ierror)
          if ( ierror /= 0 ) call endrun('HUB2ATM_ALLOC error: allocation error fv')
       end do
    endif  !dust

    if (lnd_drydep .and. n_drydep>0) then
       do c = begchunk,endchunk 
          allocate (srfflx_state2d(c)%depvel(pcols,n_drydep), stat=ierror)
          if ( ierror /= 0 ) call endrun('HUB2ATM_ALLOC error: allocation error depvel')
       end do
    endif
          
    do c = begchunk,endchunk
       srfflx_state2d(c)%lchnk = c
       srfflx_state2d(c)%ncol  = get_ncols_p(c)
       srfflx_state2d(c)%asdir    (:) = 0._r8
       srfflx_state2d(c)%asdif    (:) = 0._r8
       srfflx_state2d(c)%aldir    (:) = 0._r8
       srfflx_state2d(c)%aldif    (:) = 0._r8
       srfflx_state2d(c)%lwup     (:) = 0._r8
       srfflx_state2d(c)%lhf      (:) = 0._r8
       srfflx_state2d(c)%shf      (:) = 0._r8
       srfflx_state2d(c)%wsx      (:) = 0._r8
       srfflx_state2d(c)%wsy      (:) = 0._r8
       srfflx_state2d(c)%tref     (:) = 0._r8
       srfflx_state2d(c)%qref     (:) = 0._r8
       srfflx_state2d(c)%ts       (:) = 0._r8
       srfflx_state2d(c)%sst      (:) = 0._r8
       srfflx_state2d(c)%snowhland(:) = 0._r8
       srfflx_state2d(c)%snowhice (:) = 0._r8
       srfflx_state2d(c)%fco2_lnd (:) = 0._r8
       srfflx_state2d(c)%fco2_ocn (:) = 0._r8
       srfflx_state2d(c)%fdms     (:) = 0._r8
       srfflx_state2d(c)%landfrac (:) = inf
       srfflx_state2d(c)%icefrac  (:) = inf
       srfflx_state2d(c)%ocnfrac  (:) = inf
       if ( dust ) then
          srfflx_state2d(c)%ram1  (:) = 0.1_r8
          srfflx_state2d(c)%fv    (:) = 0.1_r8
       endif
       srfflx_state2d(c)%cflx   (:,:) = 0._r8
       srfflx_state2d(c)%ustar    (:) = 0._r8
       srfflx_state2d(c)%re       (:) = 0._r8
       srfflx_state2d(c)%ssq      (:) = 0._r8
       if (lnd_drydep .and. n_drydep>0) then
          srfflx_state2d(c)%depvel (:,:) = 0._r8
       endif
    end do

  end subroutine hub2atm_alloc

!
!===============================================================================
!

!----------------------------------------------------------------------- 
! 
! BOP
!
! !IROUTINE: atm2hub_alloc
!
! !DESCRIPTION:
!
!   Allocate space for the atmosphere to surface data type. And initialize
!   the values.
! 
!-----------------------------------------------------------------------
!
! !INTERFACE
!
  subroutine atm2hub_alloc( surface_state2d )
!
!!USES:
!
!
!!ARGUMENTS:
!
   type(surface_state), pointer :: surface_state2d(:)    ! Atmosphere to surface input
!
!!LOCAL VARIABLES:
!
    integer :: c            ! chunk index
    integer :: ierror       ! Error code
    !----------------------------------------------------------------------- 

    if ( .not. phys_grid_initialized() ) call endrun( "ATM2HUB_ALLOC error: phys_grid not called yet" )
    allocate (surface_state2d(begchunk:endchunk), stat=ierror)
    if ( ierror /= 0 )then
      write(iulog,*) 'Allocation error: ', ierror
      call endrun('ATM2HUB_ALLOC error: allocation error')
    end if

    do c = begchunk,endchunk
       surface_state2d(c)%lchnk       = c
       surface_state2d(c)%ncol        = get_ncols_p(c)
       surface_state2d(c)%tbot(:)     = 0._r8
       surface_state2d(c)%zbot(:)     = 0._r8
       surface_state2d(c)%ubot(:)     = 0._r8
       surface_state2d(c)%vbot(:)     = 0._r8
       surface_state2d(c)%qbot(:,:)   = 0._r8
       surface_state2d(c)%pbot(:)     = 0._r8
       surface_state2d(c)%rho(:)      = 0._r8
       surface_state2d(c)%netsw(:)    = 0._r8
       surface_state2d(c)%flwds(:)    = 0._r8
       surface_state2d(c)%precsc(:)   = 0._r8
       surface_state2d(c)%precsl(:)   = 0._r8
       surface_state2d(c)%precc(:)    = 0._r8
       surface_state2d(c)%precl(:)    = 0._r8
       surface_state2d(c)%soll(:)     = 0._r8
       surface_state2d(c)%sols(:)     = 0._r8
       surface_state2d(c)%solld(:)    = 0._r8
       surface_state2d(c)%solsd(:)    = 0._r8
       surface_state2d(c)%srfrad(:)   = 0._r8
       surface_state2d(c)%thbot(:)    = 0._r8
       surface_state2d(c)%co2prog(:)  = 0._r8
       surface_state2d(c)%co2diag(:)  = 0._r8
       surface_state2d(c)%psl(:)      = 0._r8
       surface_state2d(c)%bcphidry(:) = 0._r8
       surface_state2d(c)%bcphodry(:) = 0._r8
       surface_state2d(c)%bcphiwet(:) = 0._r8
       surface_state2d(c)%ocphidry(:) = 0._r8
       surface_state2d(c)%ocphodry(:) = 0._r8
       surface_state2d(c)%ocphiwet(:) = 0._r8
       surface_state2d(c)%dstdry1(:)  = 0._r8
       surface_state2d(c)%dstwet1(:)  = 0._r8
       surface_state2d(c)%dstdry2(:)  = 0._r8
       surface_state2d(c)%dstwet2(:)  = 0._r8
       surface_state2d(c)%dstdry3(:)  = 0._r8
       surface_state2d(c)%dstwet3(:)  = 0._r8
       surface_state2d(c)%dstdry4(:)  = 0._r8
       surface_state2d(c)%dstwet4(:)  = 0._r8
    end do

  end subroutine atm2hub_alloc


!======================================================================
! 
! BOP
!
! !IROUTINE: hub2atm_setopts
!
! !DESCRIPTION:
!
!   Method for outside packages to influence what is allocated
!   (For now, just aerosol dust controls if fv & ram1 arrays are allocated
! 
!-----------------------------------------------------------------------
!
! !INTERFACE
!
  subroutine hub2atm_setopts( aero_dust_in )
!
!!USES:
!
!
!!ARGUMENTS:
!
    logical, intent(in),optional :: aero_dust_in


!----------------------------------------------------------------------- 
! 
! EOP
!

    if ( present (aero_dust_in ) ) then
       dust = aero_dust_in
    endif

end subroutine hub2atm_setopts

end module camsrfexch_types
