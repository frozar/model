module tools
  implicit none
  public

contains
  REAL(kind=8) FUNCTION length2(x)
    IMPLICIT NONE
    REAL(kind=8),DIMENSION(2) :: x 

    length2 = dsqrt(DOT_PRODUCT(x,x))

  END FUNCTION length2

  ! Calcule la distance d'un point P a un segment (V1,V2)
  ! Input     vP  : coordonnees du point P
  !           v1  : coordonnees du debut du segment V1
  !           v2  : coordonnees de la fin du segment V2
  function perpendicularDistance(vP, v1, v2)
    implicit none
    REAL(kind=8)              :: perpendicularDistance
    REAL(kind=8),DIMENSION(2) :: vP,v1,v2,LI,LP

    LI = v2 - v1
    LP = vP - v1
    perpendicularDistance = abs( LI(1) * LP(2) - LP(1) * LI(2)) / length2(LI)

  end function perpendicularDistance

  ! simplification of a polygon based on Ramer–Douglas–Peucker algorithm
  ! Input    v(2,n)   : coordinates of the vertices
  !          i1       : indice of first vertex
  !          i2       : indice of last vertex
  !          eps      : distance of simplification
  ! Output   valid(n) : boolean to know if this vertex belongs or not to the simplified polygon
  recursive subroutine DouglasPeuckerRecursive(v, i1, i2, eps, valid)
    implicit none
    INTEGER,INTENT(IN)                   :: i1,i2
    REAL(kind=8),INTENT(IN)              :: eps
    REAL(kind=8),DIMENSION(:,:),POINTER  :: v
    LOGICAL,DIMENSION(:),POINTER         :: valid

    INTEGER                              :: ip,imax
    REAL(kind=8)                         :: dist,dmax

    dmax = 0.D0
    imax = 1
    ! regarde les points intermediaires
    ! et calcule la distance perpenticulaire maximale
    DO ip = i1+1,i2-1
      dist = perpendicularDistance(v(:,ip), v(:,i1), v(:,i2))
      IF ( dist > dmax ) THEN
        imax = ip
        dmax = dist
      ENDIF
    ENDDO

    ! si la distance perpenticulaire maximale est plus grande que eps, 
    ! simplifie les 2 branches [i1,imax] et [imax,i2]
    IF ( dmax > eps ) THEN
      call DouglasPeuckerRecursive(v, i1, imax, eps, valid)
      call DouglasPeuckerRecursive(v, imax, i2, eps, valid)

    ! sinon on vire tous les points entre i1 et i2   
    ELSE
      valid(i1+1:i2-1) = .FALSE.

    ENDIF

  end subroutine DouglasPeuckerRecursive

  real function log2(x)
    implicit none
    real, intent(in) :: x

    log2 = log(x) / log(2.)
  end function log2

  ! simplification of a polygon based on Ramer–Douglas–Peucker algorithm
  ! Input    v(2,n)   : coordinates of the vertices
  !          i1       : indice of first vertex
  !          i2       : indice of last vertex
  !          eps      : distance of simplification
  ! Output   valid(n) : boolean to know if this vertex belongs or not to the simplified polygon
  recursive subroutine DouglasPeuckerIteratif(v, eps, valid)
    implicit none
    REAL(kind=8),INTENT(IN)              :: eps
    REAL(kind=8),DIMENSION(:,:),POINTER  :: v
    LOGICAL,DIMENSION(:),POINTER         :: valid

    INTEGER                              :: i1,i2
    INTEGER                              :: ip,imax
    REAL(kind=8)                         :: dist,dmax

    integer, dimension(:,:), pointer     :: loop_index
    integer                              :: offset_loop_index
    integer                              :: next_loop_index
    integer                              :: nbp
    integer                              :: nb_sub_step

    nbp = size(valid)
    nb_sub_step = 2 ** ( ceiling( log2( real(nbp) - 1 ) ) + 1 )
    allocate( loop_index( 2, nb_sub_step ) )

    loop_index = -1
    
    loop_index( : , 1 ) = (/ 1, nbp /)

    ! print *, loop_index

    offset_loop_index = 1
    next_loop_index   = 1
    i1 = loop_index( 1, offset_loop_index )
    i2 = loop_index( 2, offset_loop_index )

    do while( (i1 /= -1) .and. (i2 /= -1) .and. (offset_loop_index <= nb_sub_step) )

      dmax = 0.D0
      imax = 1
      ! regarde les points intermediaires
      ! et calcule la distance perpenticulaire maximale
      DO ip = i1+1,i2-1
        dist = perpendicularDistance(v(:,ip), v(:,i1), v(:,i2))
        IF ( dist > dmax ) THEN
          imax = ip
          dmax = dist
        ENDIF
      ENDDO

      ! si la distance perpenticulaire maximale est plus grande que eps, 
      ! simplifie les 2 branches [i1,imax] et [imax,i2]
      IF ( dmax > eps ) THEN
        ! call DouglasPeuckerRecursive(v, i1, imax, eps, valid)
        next_loop_index = next_loop_index + 1
        loop_index( 1, next_loop_index ) = i1
        loop_index( 2, next_loop_index ) = imax

        ! call DouglasPeuckerRecursive(v, imax, i2, eps, valid)
        next_loop_index = next_loop_index + 1
        loop_index( 1, next_loop_index ) = imax
        loop_index( 2, next_loop_index ) = i2

      ! sinon on vire tous les points entre i1 et i2   
      ELSE
        valid(i1+1:i2-1) = .FALSE.
      ENDIF

      offset_loop_index = offset_loop_index + 1
      i1 = loop_index( 1, offset_loop_index )
      i2 = loop_index( 2, offset_loop_index )

    end do  ! END - do while

    deallocate(loop_index)

  end subroutine DouglasPeuckerIteratif

  !> ze test
  subroutine test()

    IMPLICIT NONE
    INTEGER                                 :: nbp
    REAL(kind=8),DIMENSION(:,:),POINTER     :: points
    LOGICAL,DIMENSION(:),POINTER            :: valid

    ! For the ramer-douglas-peucker algorithm, the last point must be different
    ! of the first point to avoid a division by 0 in 'perpendicularDistance()'

    ! nbp = 10
    ! allocate(points(2,nbp))
    nbp = 9
    allocate(points(2,nbp))

    points(:,1)  = (/ 0.0D0,   0.0D0   /)
    points(:,2)  = (/ 0.2D0,   0.0D0   /)
    points(:,3)  = (/ 0.6D0,   0.0D0   /)
    points(:,4)  = (/ 1.0D0,   0.0D0   /)
    points(:,5)  = (/ 1.0D0,   0.999D0 /)
    points(:,6)  = (/ 1.0D0,   1.0D0   /)
    points(:,7)  = (/ 0.996D0, 1.002D0 /)
    points(:,8)  = (/ 0.0D0,   1.0D0   /)
    points(:,9)  = (/-0.003D0, 0.5D0   /)
    ! points(:,10) = (/ 0.0D0,   0.0D0   /)

    allocate(valid(nbp))
    valid = .TRUE.
    call DouglasPeuckerRecursive(points, 1, nbp, 5.D-3, valid)
    print *, "recursive valid array: ", valid

    valid = .TRUE.
    call DouglasPeuckerIteratif(points, 5.D-3, valid)
    print *, "iterative valid array: ", valid

    deallocate( valid )
    deallocate( points )

  end subroutine test

end module tools

program main
  use tools
  implicit none

  call test()

end program main

! Command line to compile the code:
! gfortran -o ramer_douglas_peucker ramer_douglas_peucker.f90 && ./ramer_douglas_peucker
