

! //this file should be included in F90 files that want to interface with C functions

! //The interface block to interface with C routines

interface

	subroutine gpu_init ( n ) bind ( c )
		use iso_c_binding
		integer ( c_int ), VALUE :: n
	end subroutine ben_init

	subroutine ben_timer_start ( ) bind ( c )
	  use iso_c_binding
	end subroutine ben_timer_start

	subroutine ben_timer_end ( string ) bind ( c )
	  use iso_c_binding
	  character ( kind = C_CHAR ) :: string ( * )
	end subroutine ben_timer_end

end interface
