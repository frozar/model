#pragma GCC optimize "-O3,omit-frame-pointer,inline"
// #pragma GCC optimize "-O1"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <queue>          // std::queue

#include <cstdio>

// Statement of int*_t and uint*_t type
#include <stdint.h>

// Specific format for printf and uint*_t argument
#include <inttypes.h>

// memset, memcpy function
#include <cstring>

// chrono
#include <chrono>

using namespace std;

#define DEBUG 0

#define now chrono::system_clock::now

#define NB_LINE    12
#define NB_COL      6
#define NB_PIECES   8

#define NB_PRED   8
#define NB_COLOR  5
#define NB_ROT    4

#define NB_POSSIBILITY 22

#define TIME_LIMIT_MC_FIRST 0.940
#define TIME_LIMIT_MC_WHILE 0.094

#define CHUNK_SIZE_FIRST   5000
#define CHUNK_SIZE_WHILE    250

#define NB_ALEA    10 * 1024 * 1024

#define REDUCE_GRID_VERSION  4


typedef int8_t tint ;

struct answer_t {
  tint pos ;
  tint rot ;  
} ;

void ind_2_pos_rot( tint index, tint & pos, tint & rot ) {

  pos = -1 ;
  rot = -1 ;
  
  switch( index ) {
  case 0 :
    pos = 0 ;
    rot = 0 ;
    break ;
  case 1 :
    pos = 0 ;
    rot = 1 ;
    break ;
  case 2 :
    pos = 0 ;
    rot = 3 ;
    break ;
  case 19 :
    pos = 5 ;
    rot = 1 ;
    break ;
  case 20 :
    pos = 5 ;
    rot = 2 ;
    break ;
  case 21 :
    pos = 5 ;
    rot = 3 ;
    break ;
  }

  if ( ( pos == -1 ) && ( rot == -1 ) ) {
    pos = ( index + 1 ) / NB_ROT ;
    rot = ( index + 1 ) % NB_ROT ;
  }

}

struct answer_t tab_alea[ NB_ALEA ] ;

void init_tab_alea( ) {
  float f_rand ;
  tint index ;
  for ( int i = 0 ; i < NB_ALEA ; ++i ) {
    f_rand = rand( ) / float( RAND_MAX ) ;
    index  = f_rand * NB_POSSIBILITY ;
    ind_2_pos_rot( index, tab_alea[ i ].pos, tab_alea[ i ].rot ) ;
  }
}

int ind_alea = 0 ;

struct answer_t get_answer( ) {

  struct answer_t res = tab_alea[ ind_alea ] ;
  ++ind_alea ;

  if ( ind_alea == NB_ALEA ) {
    ind_alea = 0 ;
  }

  return res ;
}


tint my_grid [ NB_LINE ][ NB_COL ] ;
tint his_grid[ NB_LINE ][ NB_COL ] ;

int my_score ;
int his_score ;
int his_previous_score ;

int ind_iter = 0 ;

void print_grid( tint grid[ NB_LINE ][ NB_COL ] ){

  for ( tint i = 0; i < NB_LINE ; ++i ) {
    for ( tint j = 0; j < NB_COL ; ++j ) {
      if ( grid[ i ][ j ] == -1 ) {
	fprintf( stderr, " . " );
      }
      else {
	fprintf( stderr, "%2d ", grid[ i ][ j ] );
      }
    }
    fprintf( stderr, "\n" );
  }   
  fprintf( stderr, "\n" );

}

void print_pieces( tint pieces[NB_PIECES][2] ){

  cerr << "Pieces: " << endl ;
  for ( tint i = 0; i < NB_PIECES ; ++i ) {
    for ( tint j = 0; j < 2 ; ++j ) {
      cerr << ( int ) pieces[ i ][ j ] << " " ;
    }
    cerr << endl ;
  }   

}

bool test_fallen_piece_aux( tint grid[ NB_LINE ][ NB_COL ], 
			    const tint pos, const tint color ) {

  tint i_empty = NB_LINE - 1 ;
  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    if ( grid[ i ][ pos ] != ( tint ) -1 ) {
      i_empty = i - 1 ;
      break;
    }
  }

  if ( 0 <= i_empty ) {
    grid[ i_empty ][ pos ] = color ;
    return true ;
  }
  else {    
    return false ;
  }

}

tint test_fallen_piece( tint grid[ NB_LINE ][ NB_COL ],
			const tint piece[ 2 ], const tint pos, const tint rot ) {

  // Deal with the colorA and colorB independantly
  // and according with the rot parameter
  
  tint color_a = piece[ 0 ];
  tint color_b = piece[ 1 ];
  tint color_first, color_second ;
  tint pos_first  , pos_second   ;

  switch( rot ){
  case 0:
    color_first  = color_a ;
    color_second = color_b ;
    pos_first    = pos ;
    pos_second   = pos + 1 ;
    break ;

  case 1:
    color_first  = color_a ;
    color_second = color_b ;
    pos_first    = pos ;
    pos_second   = pos ;
    break ;

  case 2:
    color_first  = color_b ;
    color_second = color_a ;
    pos_first    = pos - 1 ;
    pos_second   = pos ;
    break ;

  case 3:
    color_first  = color_b ;
    color_second = color_a ;
    pos_first    = pos  ;
    pos_second   = pos ;
    break ;

  default :
    color_first  = -1 ;
    color_second = -1 ;
    pos_first    = -1 ;
    pos_second   = -1 ;
    break ;
  }

  if ( ( pos_first  < 0 ) || ( 5 < pos_first  ) ||
       ( pos_second < 0 ) || ( 5 < pos_second ) ) {
    return 0 ;
  }

  return ( test_fallen_piece_aux( grid, pos_first , color_first  ) && \
	   test_fallen_piece_aux( grid, pos_second, color_second ) ) ;
  
}


struct submission_t {
  struct answer_t answer   [ NB_PRED ] ;
  tint            len_chain[ NB_PRED ] ;
  int             score    [ NB_PRED ] ;
} ;


void get_submission( struct submission_t & submission, bool verbose = false ) {

  for ( tint i = 0 ; i < NB_PRED ; ++i ){

    submission.answer[ i ] = get_answer( ) ;
    
    // if ( verbose ){
    //   cerr << "ind_submission: " << ( int ) ind_submission  << endl ;
    //   cerr << "pos: " << ( int ) submission.answer[ i ].pos << endl ;
    //   cerr << "rot: " << ( int ) submission.answer[ i ].rot << endl ;
    // }
  }

}


void apply_gravity( tint grid[NB_LINE][NB_COL], bool verbose = false ){

  if ( verbose ) {

    fprintf( stderr, "INPUT apply_gravity\n" );
    print_grid( grid );

  }
  
  tint tmp_col[ NB_LINE ];
  tint tmp_ind;
  
  for ( tint j = 0 ; j < NB_COL ; ++j ){
    tmp_ind = 0 ;

    for ( tint i = 0 ; i < NB_LINE ; ++i ){ 
      if ( grid[ i ][ j ] != -1 ) {
	tmp_col[ tmp_ind ] = grid[ i ][ j ];
	tmp_ind++;
      }  
    }

    if ( tmp_ind ) {
      for ( tint i = 0 ; i < NB_LINE - tmp_ind ; ++i ) {
	grid[ i ][ j ] = -1 ;
      }
    
      for ( tint i = NB_LINE - tmp_ind ; i < NB_LINE ; ++i ) {
	grid[ i ][ j ] = tmp_col[ i - ( NB_LINE - tmp_ind ) ];
      }
    }
  }

  if ( verbose ) {

    fprintf( stderr, "OUTPUT apply_gravity\n" );
    print_grid( grid );

  }
  
}




int get_nb_non_empty_cell( tint grid[ NB_LINE ][ NB_COL ] ) {
  int nb_non_empty_cell = 0 ;
  
  for ( int i = 0 ; i < NB_LINE ; ++i ) {
    for ( int j = 0 ; j < NB_COL ; ++j ) {
      // cerr << "i: " << i << endl ;
      // cerr << "j: " << j << endl ;
      // cerr << "grid: " << ( int ) grid[ i ][ j ] << endl ;
      if ( 0 <= grid[ i ][ j ] ) {
	++nb_non_empty_cell ;
      }
    }
  }
  return nb_non_empty_cell ;
}


struct cell_t {
  tint color ;
  // Status:
  // 0 : never visited
  // 1 : next focus
  // 2 : already processed
  // 3 : to explose
  tint status ;
  tint i ;
  tint j ;
} ;


bool get_next_cell_to_process( struct cell_t rich_grid[ NB_LINE ][ NB_COL ],
			       tint & coord_i, tint & coord_j ) {
  for ( int i = NB_LINE - 1 ; -1 < i ; --i ) {
    for ( int j = NB_COL - 1 ; -1 < j ; --j ) {
      if ( ( 1 <= rich_grid[ i ][ j ].color ) &&
	   ( rich_grid[ i ][ j ].status == 0 ) ) {
	rich_grid[ i ][ j ].status = 1 ;
	coord_i = i ;
	coord_j = j ;
	return true ;
      }
    }
  }

  return false ;
}


void get_neighbourhood( const tint coord_i, const tint coord_j,
			bool tab_neighbour[ 4 ] ) {
			// bool & east, bool & north, bool & west, bool & south ) {

  bool east  = false ;
  bool north = false ;
  bool west  = false ;
  bool south = false ;
  
  if ( coord_j < NB_COL - 1 ) {
    east = true ;
  }
  if ( 0 < coord_i ) {
    north = true ;
  }
  if ( 0 < coord_j ) {
    west = true ;
  }
  if ( coord_i < NB_LINE - 1 ) {
    south = true ;
  }

  tab_neighbour[ 0 ] = east ;
  tab_neighbour[ 1 ] = north ;
  tab_neighbour[ 2 ] = west ;
  tab_neighbour[ 3 ] = south ;
}


bool reduce_grid_atomic_2( tint grid[ NB_LINE ][ NB_COL ],
			   int & nb_to_explose,
			   tint bonus_color[ NB_COLOR ],
			   bool verbose = false ) {

  memset( bonus_color, ( tint ) 0, NB_COLOR ) ;

  struct cell_t rich_grid[ NB_LINE ][ NB_COL ] ;

  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    for ( tint j = 0 ; j < NB_COL ; ++j ) {
      rich_grid[ i ][ j ].color  = grid[ i ][ j ] ;
      rich_grid[ i ][ j ].status = 0 ;
      rich_grid[ i ][ j ].i      = i ;
      rich_grid[ i ][ j ].j      = j ;
    }
  }

  // tint cell_to_explose[ NB_LINE * 3 * 2 ] ;
  // memset( cell_to_explose, 0, sizeof( tint ) * NB_LINE * 3 * 2 ) ;

  // std::list<int> mylist (2,200);         // list with 2 elements
  std::queue<struct cell_t> next_cell_to_process ; 

  std::queue<struct cell_t> cur_group ; 
  
  int head_next_cell = 0 ;
  
  int nb_non_empty_cell = get_nb_non_empty_cell( grid ) ;
  int it_non_empty_cell ;

  tint coord_i, coord_j ;
  bool east, north, west, south ;
  int i_cell = 0 ;
  int group_size = 0 ;

  nb_to_explose = 0 ;
  bool has_change = false ;
  bool tab_neighbour[ 4 ] ; // { east, north, west, south } ;

#if DEBUG
  cerr << "DBG nb_non_empty_cell: " << nb_non_empty_cell << endl ;
#endif
  
  it_non_empty_cell = 0 ;
  while ( it_non_empty_cell < nb_non_empty_cell ) {
    
    get_next_cell_to_process( rich_grid, coord_i, coord_j ) ;
    rich_grid[ coord_i ][ coord_j ].status = 1 ;
    next_cell_to_process.push( rich_grid[ coord_i ][ coord_j ] ) ;
    ++it_non_empty_cell ;

#if DEBUG
    cerr << "DBG it_non_empty_cell: " << it_non_empty_cell << endl ;
#endif

    // clean the group
    while ( ! cur_group.empty( ) ) {
      cur_group.pop( ) ;
    }
      
    // while ( get_next_cell_to_process( rich_grid, coord_i, coord_j ) ) {
    // Try to catch a group
    while ( ! next_cell_to_process.empty( ) ) {

      // cerr << "cur_group: " << cur_group << endl ;
      
      struct cell_t cur_cell = next_cell_to_process.front( ) ;
      next_cell_to_process.pop( ) ;
      cur_group.push( cur_cell ) ;
  
      // tab_neighbour : { east, north, west, south } ;
      get_neighbourhood( cur_cell.i, cur_cell.j, tab_neighbour ) ;

      for ( int i = 0 ; i < 4 ; ++i ) {

	bool tmp_b = tab_neighbour[ i ] ;
	int tmp_i, tmp_j ;
	if ( tmp_b ) {
	  switch( i ) {
	  case 0 :
	    tmp_i = cur_cell.i ;
	    tmp_j = cur_cell.j + 1 ;
	    break ;
	  case 1 :
	    tmp_i = cur_cell.i - 1 ;
	    tmp_j = cur_cell.j ;
	    break ;
	  case 2 :
	    tmp_i = cur_cell.i ;
	    tmp_j = cur_cell.j - 1 ;
	    break ;
	  case 3 :
	    tmp_i = cur_cell.i + 1 ;
	    tmp_j = cur_cell.j ;
	    break ;
	  }
	
	  if ( ( rich_grid[ tmp_i ][ tmp_j ].status == 0 ) &&
	       ( rich_grid[ tmp_i ][ tmp_j ].color  == cur_cell.color ) ) {
	    rich_grid[ tmp_i ][ tmp_j ].status = 1 ;
	    next_cell_to_process.push( rich_grid[ tmp_i ][ tmp_j ] ) ;
	    ++it_non_empty_cell ;
	  }
	}
      }

#if DEBUG
      std::queue<struct cell_t> to_print_cur_group( cur_group ) ;

      cerr << "[ ";
      while ( ! to_print_cur_group.empty( ) ) {
	struct cell_t tmp_cell = to_print_cur_group.front( ) ;
	cerr << " " << ( int ) tmp_cell.i << " " << ( int ) tmp_cell.j << ", " ;
	to_print_cur_group.pop( ) ;
      }
      cerr << " ]" << endl ;
#endif

      rich_grid[ cur_cell.i ][ cur_cell.j ].status = 2 ;
    }  // END - while ( ! next_cell_to_process.empty( ) ) {

    if ( 4 <= cur_group.size( ) ) {

      struct cell_t cur_cell = cur_group.front( ) ;
      
      bonus_color[ rich_grid[ cur_cell.i ][ cur_cell.j ].color - 1 ] += cur_group.size( ) ;

      while ( ! cur_group.empty( ) ) {
	cur_cell = cur_group.front( ) ;
	
#if DEBUG
	cerr << "to_explose i: " << ( int ) cur_cell.i << endl ;
	cerr << "to_explose j: " << ( int ) cur_cell.j << endl ;
#endif
	rich_grid[ cur_cell.i ][ cur_cell.j ].status = 3 ;
	++nb_to_explose ;
	
	cur_group.pop( ) ;
      }
      // bonus_color[ rich_grid[ cur_cell.i ][ cur_cell.j ].color - 1 ] += cur_group.size( ) ;

    }

  }  // END - while ( it_non_empty_cell < nb_non_empty_cell ) {


  if ( nb_to_explose ) {
    has_change = true ;

#if DEBUG
    cerr << "nb_to_explose: " << nb_to_explose << endl ;
#endif
    
    for ( int i = 0 ; i < NB_LINE ; ++i ) {
      for ( int j = 0 ; j < NB_COL ; ++j ) {
	if ( rich_grid[ i ][ j ].status == 3 ) {
#if DEBUG
	  cerr << "i: " << i << endl ;
	  cerr << "j: " << j << endl ;
	  cerr << "grid: " << ( int ) grid[ i ][ j ] << endl ;
#endif
	  grid[ i ][ j ] = -1 ;

	  tint i_min = max( i - 1, 0 );
	  tint i_max = min( i + 1, NB_LINE - 1 );
	  tint j_min = max( j - 1, 0 );
	  tint j_max = min( j + 1, NB_COL - 1 );

	  if ( grid[ i ][ j_min ] == 0 )
	    grid[ i ][ j_min ] = -1 ;
	  if ( grid[ i ][ j_max ] == 0 )
	    grid[ i ][ j_max ] = -1 ;
	  if ( grid[ i_min ][ j ] == 0 )
	    grid[ i_min ][ j ] = -1 ;
	  if ( grid[ i_max ][ j ] == 0 )
	    grid[ i_max ][ j ] = -1 ;
	}
      }
    }
    
    apply_gravity( grid ) ;

    // if ( verbose ) {
    //   print_grid( grid ) ;
    // }

  }

#if DEBUG
  cerr << "bonus color: " ;
  for ( int i = 0 ; i < NB_COLOR ; ++i ) {
    cerr << ( int ) bonus_color[ i ] << ", " ;
  }
  cerr << endl ;

  print_grid( grid ) ;
#endif
  
  return has_change ;
}


tint grid_filter[ NB_COLOR ][ NB_LINE * NB_COL * 2 / 5 ] ;
tint nb_colors[ NB_COLOR ] ;

struct pt_t {
  tint i ;
  tint j ;
} ;

  

bool reduce_grid_atomic( tint grid[ NB_LINE ][ NB_COL ], 
			 int & nb_to_explose,
			 tint bonus_color[ NB_COLOR ],
			 bool verbose = false ) {

  // if ( verbose ) {
  //   fprintf( stderr, "INPUT reduce_grid_atomic\n" );
  //   print_grid( grid );
  // }
  
  memset( nb_colors, 0, sizeof( tint ) * NB_COLOR ) ;
  memset( grid_filter, -1, sizeof( tint ) * NB_COLOR *  
	  ( sizeof ( tint ) * NB_LINE * NB_COL * 2 / 5 ) ) ;

  // if ( verbose ) {
  //   cerr << "sizeof( grid_filter ): " << sizeof( grid_filter ) << endl ;
  //   cerr << "asked size: " << sizeof( tint ) * NB_COLOR *  
  //     ( sizeof ( tint ) * NB_LINE * NB_COL * 2 / 5 ) << endl ;
  // }
  
  tint cur_color ;
  tint ind_color ;
  tint ind_grid_filter ;

  memset( bonus_color, ( tint ) 0, NB_COLOR ) ;

  // ### 0 - SEPARATE the grid between the different colors
  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    for ( tint j = 0 ; j < NB_COL ; ++j ) {
      if ( 1 <= grid[ i ][ j ] ) {
	cur_color = grid[ i ][ j ] ;
	ind_color = cur_color - 1 ;
	ind_grid_filter   = nb_colors[ ind_color ] ;
	
	grid_filter[ ind_color ][ 2 * ind_grid_filter     ] = i ;
	grid_filter[ ind_color ][ 2 * ind_grid_filter + 1 ] = j ;

	nb_colors[ ind_color ] += 1 ;

      }
    }
  }

  // if( verbose ) {
  //   tint i = 0 ;
  //   while ( grid_filter[ 0 ][ 2 * i ] != 0 ){
  //     fprintf( stderr, "grid_filter i: %d\n" , grid_filter[ 0 ][ 2 * i     ] );
  //     fprintf( stderr, "grid_filter j: %d\n" , grid_filter[ 0 ][ 2 * i + 1 ] );
  //     ++i ;
  //   }
  // }
  
  tint to_explose[ NB_LINE * NB_COL * 2 / 2 ] = { -1, -1 } ;
  tint cur_group[ NB_LINE * NB_COL * 2 / 2 ] ;
  // if ( verbose ) {
  //   memset( cur_group, ( tint ) -1, NB_LINE * NB_COL * 2 / 2 ) ;
  // }

  nb_to_explose = 0 ;
  tint one_color_pos[ 2 ] = { 0, 0 } ;
  bool has_change = false ;

  tint gr_size ;
  tint ind_cur_group ;
  bool new_bloc ;

  // ### 1 - DEAL with each color separately
  for ( tint i_color = 0 ; i_color < NB_COLOR ; ++i_color ) {

    // if ( verbose ) {
    //   cerr << "## I_COLOR: " << ( int ) i_color 
    // 	   << " nb_color: " << ( int ) nb_colors[ i_color ] << endl ;
    // }

    ind_color = 0 ;
    ind_cur_group  = 0 ;
    gr_size   = 0 ;

    // WHILE we don't see every color position
    while( ind_color < nb_colors[ i_color ] ) {

      // if ( verbose ) {
      // 	int i = 0;
      // 	while ( cur_group[ 2 * i ] != -1 ){
      // 	  cerr << "cur_group i: " << ( int ) cur_group[ 2 * i ]
      // 	       << " j: " << ( int ) cur_group[ 2 * i + 1 ] << endl ;
      // 	  ++i;
      // 	}
      // 	fprintf( stderr, "gr_size: %d\n", gr_size );
      // }
      
      if ( gr_size == 0 ) {
	one_color_pos[ 0 ] = grid_filter[ i_color ][ 2 * ind_color ] ;
	one_color_pos[ 1 ] = grid_filter[ i_color ][ 2 * ind_color + 1 ] ;
	++ind_color;
	/* fprintf( stderr, "ind_color: %d\n", ind_color ); */

	if ( ( one_color_pos[ 0 ] == -1 ) && ( one_color_pos[ 1 ] == -1 ) ) {
	  continue;
	}

	cur_group[ gr_size * 2     ] = one_color_pos[ 0 ] ;
	cur_group[ gr_size * 2 + 1 ] = one_color_pos[ 1 ] ;
	++gr_size ;
      }
      else {
	one_color_pos[ 0 ] = cur_group[ 2 * ind_cur_group     ] ;
	one_color_pos[ 1 ] = cur_group[ 2 * ind_cur_group + 1 ] ;

	// cerr << "2 * ind_cur_group: " << 2 * ind_cur_group << endl ;
	// cerr << "2 * ind_cur_group + 1: " << 2 * ind_cur_group + 1 << endl ;
      }

      /* fprintf( stderr, "one_color_pos[ 0 ]: %d\n", one_color_pos[ 0 ] ); */
      /* fprintf( stderr, "one_color_pos[ 1 ]: %d\n", one_color_pos[ 1 ] ); */
      
      new_bloc = false ;
      
      for ( tint i = ind_color ; i < nb_colors[ i_color ] ; ++i ) {
	if ( ( ( grid_filter[ i_color ][ 2 * i     ] == one_color_pos[ 0 ] + 1 ) &&
	       ( grid_filter[ i_color ][ 2 * i + 1 ] == one_color_pos[ 1 ]     ) ) ||
	     ( ( grid_filter[ i_color ][ 2 * i     ] == one_color_pos[ 0 ] - 1 ) &&
	       ( grid_filter[ i_color ][ 2 * i + 1 ] == one_color_pos[ 1 ]     ) ) ||
	     ( ( grid_filter[ i_color ][ 2 * i     ] == one_color_pos[ 0 ]     ) &&
	       ( grid_filter[ i_color ][ 2 * i + 1 ] == one_color_pos[ 1 ] + 1 ) ) ||
	     ( ( grid_filter[ i_color ][ 2 * i     ] == one_color_pos[ 0 ]     ) &&
	       ( grid_filter[ i_color ][ 2 * i + 1 ] == one_color_pos[ 1 ] - 1 ) ) ) {

	  cur_group[ gr_size * 2     ] = grid_filter[ i_color ][ 2 * i     ];
	  cur_group[ gr_size * 2 + 1 ] = grid_filter[ i_color ][ 2 * i + 1 ];

	  grid_filter[ i_color ][ 2 * i     ] = -1 ;
	  grid_filter[ i_color ][ 2 * i + 1 ] = -1 ;

	  ++gr_size ;
	  new_bloc = true ;
	}
      }

      /* fprintf( stderr, "found: %d\n", found ); */
      /* fprintf( stderr, "ind_cur_group: %d\n", ind_cur_group); */
      /* fprintf( stderr, "gr_size: %d\n", gr_size); */
	
      if ( ( !new_bloc ) && ( ind_cur_group == gr_size - 1 ) ) {

	if ( 4 <= gr_size ) {
	  for ( tint i = 0 ; i < gr_size ; ++i ) {
	    to_explose[ 2 * nb_to_explose     ] = cur_group[ 2 * i     ];
	    to_explose[ 2 * nb_to_explose + 1 ] = cur_group[ 2 * i + 1 ];
	    ++nb_to_explose ;
	    has_change = true ;
	  }
	  bonus_color[ i_color ] += gr_size ;
	}
	
	gr_size  = 0 ;
	ind_cur_group = 0 ;
      }

      if ( gr_size != 0 ) {

	++ind_cur_group ;
	
      }
    } // while( ind_color < nb_colors[ i_color ] )

  } // for ( tint i_color = 0 ; i_color < NB_COLOR ; ++i_color )

  // if ( verbose ) { 
  //   fprintf( stderr, "nb_to_explose: %d\n", nb_to_explose );
  // }

  if ( nb_to_explose ) {
    for ( tint ind = 0 ; ind < nb_to_explose ; ++ind ) {
      tint i = to_explose[ 2 * ind     ] ;
      tint j = to_explose[ 2 * ind + 1 ] ;

      grid[ i ][ j ] = -1 ;

      tint i_min = max( i - 1, 0 );
      tint i_max = min( i + 1, NB_LINE - 1 );
      tint j_min = max( j - 1, 0 );
      tint j_max = min( j + 1, NB_COL - 1 );

      if ( grid[ i ][ j_min ] == 0 )
	grid[ i ][ j_min ] = -1 ;
      if ( grid[ i ][ j_max ] == 0 )
	grid[ i ][ j_max ] = -1 ;
      if ( grid[ i_min ][ j ] == 0 )
	grid[ i_min ][ j ] = -1 ;
      if ( grid[ i_max ][ j ] == 0 )
	grid[ i_max ][ j ] = -1 ;

    }

    apply_gravity( grid ) ;

    // if ( verbose ) {
    //   print_grid( grid ) ;
    // }

  }

  return has_change ;
}

// struct pt_t grid_filter_4[ NB_COLOR ][ NB_LINE * NB_COL / 5 ] ;
struct pt_t grid_filter_4[ NB_COLOR ][ 14 ] ;

bool reduce_grid_atomic_4( tint grid[ NB_LINE ][ NB_COL ], 
			   int & nb_to_explose,
			   tint bonus_color[ NB_COLOR ],
			   bool verbose = false ) {

  // if ( verbose ) {
  //   fprintf( stderr, "INPUT reduce_grid_atomic\n" );
  //   print_grid( grid );
  // }
  
  memset( nb_colors, 0, sizeof( tint ) * NB_COLOR ) ;
  // memset( grid_filter_4, -1, sizeof( struct pt_t ) * NB_COLOR *  
  // 	  ( NB_LINE * NB_COL / 5 ) ) ;
  // memset( grid_filter_4, -1, sizeof( struct pt_t ) * NB_COLOR * 10 ) ;

  // if ( verbose ) {
  //   cerr << "sizeof( grid_filter ): " << sizeof( grid_filter ) << endl ;
  //   cerr << "asked size: " << sizeof( tint ) * NB_COLOR *  
  //     ( sizeof ( tint ) * NB_LINE * NB_COL * 2 / 5 ) << endl ;
  // }
  
  tint cur_color ;
  tint ind_color ;
  tint ind_grid_filter ;

  memset( bonus_color, ( tint ) 0, NB_COLOR ) ;

  // ### 0 - SEPARATE the grid between the different colors
  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    for ( tint j = 0 ; j < NB_COL ; ++j ) {
      if ( 1 <= grid[ i ][ j ] ) {
	cur_color = grid[ i ][ j ] ;
	ind_color = cur_color - 1 ;
	ind_grid_filter = nb_colors[ ind_color ] ;
	
	grid_filter_4[ ind_color ][ ind_grid_filter ].i = i ;
	grid_filter_4[ ind_color ][ ind_grid_filter ].j = j ;

	nb_colors[ ind_color ] += 1 ;
      }
    }
  }

  struct pt_t to_explose[ 16 ] ;
  struct pt_t cur_group[ 8 ] ;

  nb_to_explose = 0 ;
  struct pt_t one_color_pos ;
  bool has_change = false ;

  tint gr_size ;
  tint ind_cur_group ;
  bool new_bloc ;

  // ### 1 - DEAL with each color separately
  for ( tint i_color = 0 ; i_color < NB_COLOR ; ++i_color ) {

    ind_color = 0 ;
    ind_cur_group  = 0 ;
    gr_size   = 0 ;

    // WHILE we don't see every color position
    while( ind_color < nb_colors[ i_color ] ) {
      
      // PICK up a starting point
      if ( gr_size == 0 ) {
	one_color_pos.i = grid_filter_4[ i_color ][ ind_color ].i ;
	one_color_pos.j = grid_filter_4[ i_color ][ ind_color ].j ;

	grid_filter_4[ i_color ][ ind_color ].i = -1 ;
	grid_filter_4[ i_color ][ ind_color ].j = -1 ;
	
	++ind_color;

	if ( ( one_color_pos.i == -1 ) && ( one_color_pos.j == -1 ) ) {
	  continue ;
	}

	cur_group[ gr_size ].i = one_color_pos.i ;
	cur_group[ gr_size ].j = one_color_pos.j ;
	
	++gr_size ;
      }
      else {
	one_color_pos.i = cur_group[ ind_cur_group ].i ;
	one_color_pos.j = cur_group[ ind_cur_group ].j ;
      }
      

      new_bloc = false ;
      // SEARCH neighbourhood amoung every position of the same color
      for ( tint ind = ind_color ; ind < nb_colors[ i_color ] ; ++ind ) {
	if ( ( ( grid_filter_4[ i_color ][ ind ].i == one_color_pos.i + 1 ) &&
	       ( grid_filter_4[ i_color ][ ind ].j == one_color_pos.j     ) ) ||
	     ( ( grid_filter_4[ i_color ][ ind ].i == one_color_pos.i - 1 ) &&
	       ( grid_filter_4[ i_color ][ ind ].j == one_color_pos.j     ) ) ||
	     ( ( grid_filter_4[ i_color ][ ind ].i == one_color_pos.i     ) &&
	       ( grid_filter_4[ i_color ][ ind ].j == one_color_pos.j + 1 ) ) ||
	     ( ( grid_filter_4[ i_color ][ ind ].i == one_color_pos.i     ) &&
	       ( grid_filter_4[ i_color ][ ind ].j == one_color_pos.j - 1 ) ) ) {

	  cur_group[ gr_size ].i = grid_filter_4[ i_color ][ ind ].i ;
	  cur_group[ gr_size ].j = grid_filter_4[ i_color ][ ind ].j ;

	  grid_filter_4[ i_color ][ ind ].i = -1 ;
	  grid_filter_4[ i_color ][ ind ].j = -1 ;

	  ++gr_size ;
	  new_bloc = true ;
	}
      }

      // IF during this pass no new bloc was discover and
      // we are on the last position of the group
      if ( ( !new_bloc ) && ( ind_cur_group == gr_size - 1 ) ) {

	// IF the group is large enough, delete it
	if ( 4 <= gr_size ) {

	  // CUMULATE in the buffer to_explose
	  for ( tint i = 0 ; i < gr_size ; ++i ) {
	    to_explose[ nb_to_explose ].i = cur_group[ i ].i ;
	    to_explose[ nb_to_explose ].j = cur_group[ i ].j ;
	    ++nb_to_explose ;
	  }
	  
	  has_change = true ;
	  bonus_color[ i_color ] += gr_size ;
	  
	  // nb_to_explose += gr_size ;	  

	  // // DELETE the blocs of the current group from the grid
	  // for ( tint ind = 0 ; ind < gr_size ; ++ind ) {
	  //   tint i = cur_group[ ind ].i ;
	  //   tint j = cur_group[ ind ].j ;

	  //   grid[ i ][ j ] = -1 ;

	  //   tint i_min = max( i - 1, 0 );
	  //   tint i_max = min( i + 1, NB_LINE - 1 );
	  //   tint j_min = max( j - 1, 0 );
	  //   tint j_max = min( j + 1, NB_COL - 1 );

	  //   if ( grid[ i ][ j_min ] == 0 )
	  //     grid[ i ][ j_min ] = -1 ;
	  //   if ( grid[ i ][ j_max ] == 0 )
	  //     grid[ i ][ j_max ] = -1 ;
	  //   if ( grid[ i_min ][ j ] == 0 )
	  //     grid[ i_min ][ j ] = -1 ;
	  //   if ( grid[ i_max ][ j ] == 0 )
	  //     grid[ i_max ][ j ] = -1 ;
	  // }
	}

	// ANYWAY erase the current group
	gr_size       = 0 ;
	ind_cur_group = 0 ;
      }
      else {
      	++ind_cur_group ;
      }

    } // while( ind_color < nb_colors[ i_color ] )

  } // for ( tint i_color = 0 ; i_color < NB_COLOR ; ++i_color )

  if ( nb_to_explose ) {
    for ( tint ind = 0 ; ind < nb_to_explose ; ++ind ) {
      tint i = to_explose[ ind ].i ;
      tint j = to_explose[ ind ].j ;
      
      grid[ i ][ j ] = -1 ;
      
      tint i_min = max( i - 1, 0 );
      tint i_max = min( i + 1, NB_LINE - 1 );
      tint j_min = max( j - 1, 0 );
      tint j_max = min( j + 1, NB_COL - 1 );

      if ( grid[ i ][ j_min ] == 0 )
	grid[ i ][ j_min ] = -1 ;
      if ( grid[ i ][ j_max ] == 0 )
	grid[ i ][ j_max ] = -1 ;
      if ( grid[ i_min ][ j ] == 0 )
	grid[ i_min ][ j ] = -1 ;
      if ( grid[ i_max ][ j ] == 0 )
	grid[ i_max ][ j ] = -1 ;
    }

    apply_gravity( grid ) ;
  }

  // if ( verbose ) { 
  //   fprintf( stderr, "nb_to_explose: %d\n", nb_to_explose );
  // }

  // if ( verbose ) {
  //   print_grid( grid ) ;
  // }

  return has_change ;
}


bool get_next_starting_point( tint & offset_starting_point,
			      tint grid[ NB_LINE ][ NB_COL ],
			      tint & cur_i, tint & cur_j ) {

  bool res = false ;

  tint cpt = 0 ;

  for ( int i = NB_LINE - 1 ; -1 < i ; --i ) {
    for ( int j = 0 ; j < NB_COL ; ++j ) {
      ++cpt ;

      if ( offset_starting_point < cpt ) {
	if ( 1 <= grid[ i ][ j ] ) {
	  res = true ;
	  cur_i = i ;
	  cur_j = j ;
	  offset_starting_point = cpt ;
	  return res ;
	}
      }
    }
  }
  
  return res ;
}


bool reduce_grid_atomic_3( tint grid[ NB_LINE ][ NB_COL ], 
			   int & nb_to_explose,
			   tint bonus_color[ NB_COLOR ],
			   bool verbose = false ) {

  // if ( verbose ) {
  //   fprintf( stderr, "INPUT reduce_grid_atomic_3\n" );
  //   print_grid( grid );
  // }

  nb_to_explose = 0 ;
  
  for ( tint i = 0 ; i < NB_COLOR ; ++i ) {
    nb_colors[ i ]   = 0 ;
    bonus_color[ i ] = 0 ;    
  }

  // tint cur_group[ NB_LINE * NB_COL * 2 / 2 ] ;
  // tint cur_group[ 11 * 2 ] ;
  // tint cur_group[ 8 * 2 ] ;
  struct pt_t cur_group[ 8 ] ;

  bool has_change = false ;

  tint gr_size ;
  tint previous_gr_size ;
  tint ind_cur_group ;
  bool new_bloc ;

  bool east, north, west, south ;

  tint cur_i ;
  tint cur_j ;

  tint offset_starting_point = 0 ;
  bool tab_neighbour[ 4 ] ;

  // ### 0 - Take a starting point in the grid which is a color
  while( get_next_starting_point( offset_starting_point, grid, cur_i, cur_j ) ) {

    // cerr << "DBG cur_i: " << ( int ) cur_i << endl ;
    // cerr << "DBG cur_j: " << ( int ) cur_j << endl ;

    gr_size = 0 ;

    cur_group[ gr_size ].i = cur_i ;
    cur_group[ gr_size ].j = cur_j ;
    ++gr_size ;
	
    new_bloc = true ;
    previous_gr_size = gr_size ;
    ind_cur_group  = 0 ;

    // ### 1 - BUILD the group of points linked to the cur point
    while ( ( new_bloc ) || ( ind_cur_group <= gr_size - 1 ) ) {

      cur_i = cur_group[ ind_cur_group ].i ;
      cur_j = cur_group[ ind_cur_group ].j ;

      // tab_neighbour : { east, north, west, south } ;
      get_neighbourhood( cur_i, cur_j, tab_neighbour ) ;

      // Scan among the neighbourhood
      for ( int i = 0 ; i < 4 ; ++i ) {
	if ( ! tab_neighbour[ i ] ) {
	  continue ;
	}
	int tmp_i, tmp_j ;
	switch( i ) {
	case 0 :
	  tmp_i = cur_i ;
	  tmp_j = cur_j + 1 ;
	  break ;
	case 1 :
	  tmp_i = cur_i - 1 ;
	  tmp_j = cur_j ;
	  break ;
	case 2 :
	  tmp_i = cur_i ;
	  tmp_j = cur_j - 1 ;
	  break ;
	case 3 :
	  tmp_i = cur_i + 1 ;
	  tmp_j = cur_j ;
	  break ;
	}

	// If neighbour is of the same color
	if ( grid[ tmp_i ][ tmp_j ] == grid[ cur_i ][ cur_j ] ) {
	  bool already_in_group = false ;
	  for ( int ii = 0 ; ii < gr_size ; ++ii ) {
	    if ( ( cur_group[ ii ].i == tmp_i ) &&
		 ( cur_group[ ii ].j == tmp_j ) ) {
	      already_in_group = true ;
	      break ;
	    }
	  }
	  if ( already_in_group ) {
	    continue ;
	  }

	  cur_group[ gr_size ].i = tmp_i ;
	  cur_group[ gr_size ].j = tmp_j ;
	  ++gr_size ;
	  new_bloc = true ;
	}

      }  // END - for ( int i = 0 ; i < 4 ; ++i ) {

      ++ind_cur_group ;

      if ( previous_gr_size == gr_size ) {
	new_bloc = false ;
      }
      previous_gr_size = gr_size ;

    }  // END - while ( new_bloc ) {

    // for ( int i = 0 ; i < gr_size ; ++i ) {
    // 	cerr << "DBG i: cur_group[" << 2 * i << "]: " << ( int ) cur_group[ 2 * i ] << endl ;
    // 	cerr << "DBG j: cur_group[" << 2 * i + 1 << "]: " << ( int ) cur_group[ 2 * i + 1 ] << endl ;
    // }
    // cerr << "DBG END NEW BLOCK" << endl ;
    // cerr << endl ;
      
    // ### 2 - IF the current group is big enough, delete it
    if ( 4 <= gr_size ) {
      has_change = true ;
      for ( tint ind = 0 ; ind < gr_size ; ++ind ) {
	tint i = cur_group[ ind ].i ;
	tint j = cur_group[ ind ].j ;

	grid[ i ][ j ] = -1 ;

	tint i_min = max( i - 1, 0 ) ;
	tint i_max = min( i + 1, NB_LINE - 1 ) ;
	tint j_min = max( j - 1, 0 ) ;
	tint j_max = min( j + 1, NB_COL - 1 ) ;

	if ( grid[ i ][ j_min ] == 0 )
	  grid[ i ][ j_min ] = -1 ;
	if ( grid[ i ][ j_max ] == 0 )
	  grid[ i ][ j_max ] = -1 ;
	if ( grid[ i_min ][ j ] == 0 )
	  grid[ i_min ][ j ] = -1 ;
	if ( grid[ i_max ][ j ] == 0 )
	  grid[ i_max ][ j ] = -1 ;
      }

      nb_to_explose += gr_size ;
    }

  }  // END - while( get_next_starting_point( rich_grid, cur_i, cur_j ) ) {

  apply_gravity( grid ) ;
  // print_grid( grid ) ;

  return has_change ;
}


int get_score( tint len_chain, int nb_to_explose, tint bonus_color[ NB_COLOR ],
	       bool verbose = false ) {

  // CP: chain power
  // CB: color bonus
  // GB: global bonus
  int CP = 0 ;
  int CB = 0 ;
  int GB = 0 ;

  int nb_bonus_color ;
  int cpt_explose = 0 ;
  int sub_total   = 0 ;

  int score = 0 ;

  cpt_explose = nb_to_explose ;

  if ( len_chain < 2 ) {
    CP = 0 ;
  }
  else if ( 2 <= len_chain ) {
    CP = 8 ;
    for ( int i = 2 ; i < len_chain ; ++i ){
      CP *= 2 ;
    }
  }

  nb_bonus_color = 0 ;
  for ( tint i = 0 ; i < NB_COLOR ; ++i ){
    if ( bonus_color[ i ] != 0 ){
      ++nb_bonus_color;
    }
  }

  switch( nb_bonus_color ) {
  case 0 :
    CB = 0;
    break;
  case 1 :
    CB = 0;
    break;
  case 2 :
    CB = 2;
    break;
  case 3 :
    CB = 4;
    break;
  case 4 :
    CB = 8;
    break;
  case 5 :
    CB = 16;
    break;
  }

  GB = 0 ;
  for ( tint i = 0 ; i < NB_COLOR ; ++i ) {
    if ( bonus_color[ i ] && ( 4 <= bonus_color[ i ] ) ){
      GB += bonus_color[ i ] - 4 ;
    }
    if ( 11 <= bonus_color[ i ] ) {
      GB += 8 ;
    }
  }
  
  sub_total = min( CP + CB + GB, 999 );
  sub_total = max( sub_total, 1 );

  score = ( 10 * cpt_explose ) * sub_total ;

  if ( verbose ) {
    cerr << "GET score score: " << score << endl ;
    cerr << "cpt_explose: " << cpt_explose << endl ;
    cerr << "sub_total: " << sub_total << endl ;
    cerr << "CP: " << CP << endl ;
    cerr << "CB: " << CB << endl ;
    cerr << "GB: " << GB << endl ;
  }

  return score ;

}


void reduce_grid( tint grid[ NB_LINE ][ NB_COL ],
		  int & score, tint & len_chain,
		  bool verbose = false ) {

  int nb_to_explose = 0 ;
  tint bonus_color[ NB_COLOR ];

  score     = 0 ;
  len_chain = 0 ;

  bool ( * p_reduce_grid_atomic ) ( tint grid[ NB_LINE ][ NB_COL ],
				    int & nb_to_explose,
				    tint bonus_color[ NB_COLOR ],
				    bool verbose ) ;

  switch( REDUCE_GRID_VERSION ) {
  case 1 :
    p_reduce_grid_atomic = reduce_grid_atomic ;
    break ;
  case 2 :
    p_reduce_grid_atomic = reduce_grid_atomic_2 ;
    break ;
  case 3 :
    p_reduce_grid_atomic = reduce_grid_atomic_3 ;
    break ;
  case 4 :
    p_reduce_grid_atomic = reduce_grid_atomic_4 ;
    break ;
  }
  
  // HEAD code
  bool has_change = p_reduce_grid_atomic( grid,
					  nb_to_explose,
					  bonus_color, verbose ) ;
  // bool has_change = reduce_grid_atomic_2( grid,
  // 					  nb_to_explose,
  // 					  bonus_color, verbose ) ;

  if ( verbose ) {
    print_grid( grid ) ;
    cerr << "DBG has_change: " << has_change << endl ;
  }

  if ( has_change ) {
    len_chain += 1 ;
    score += get_score( len_chain, nb_to_explose, bonus_color, verbose ) ;
  }

  while ( has_change ){
    has_change = p_reduce_grid_atomic( grid,
				       nb_to_explose,
				       bonus_color, verbose ) ;
    // has_change = reduce_grid_atomic_2( grid,
    // 				       nb_to_explose,
    // 				       bonus_color, verbose ) ;

    if ( has_change ) {
      len_chain += 1 ;
      if ( verbose ) {
	cerr << "DURING reduce len_chain: " << ( int ) len_chain << endl ;
	print_grid( grid ) ;
      }
    
      score += get_score( len_chain, nb_to_explose, bonus_color, verbose ) ;
    }
  }

  if ( verbose ) {
    if ( score ) {
      cerr << "REDUCE grid score: " << score << endl ;
    }
  }
  
}


void simulation( const tint grid[ NB_LINE ][ NB_COL ], 
		 const tint pieces[ NB_PIECES ][ 2 ], 
		 tint wk_grid[ NB_LINE ][ NB_COL ], 
		 struct submission_t & submission, bool verbose = false ){

  // Copy the original grid
  memcpy( ( void * ) wk_grid,
	  ( void * ) grid,
	  sizeof( tint ) * NB_LINE * NB_COL ) ;

  if ( verbose ) {
    cerr << "Original grid" << endl ;
    print_grid( wk_grid ) ;
  }

  for ( tint i = 0 ; i < NB_PRED ; ++i ) {

    if ( verbose ) {
      cerr << " piece: " << ( int )pieces[ i ][ 0 ] << " "
	   << ( int ) pieces[ i ][ 1 ] << endl ;
      cerr << " pos: " << ( int ) submission.answer[ i ].pos
	   << " rot: " << ( int ) submission.answer[ i ].rot
	   << endl ;
      // cerr << " len_chain: " << ( int ) submission.len_chain[ i ] << endl ;
    }

    if ( test_fallen_piece( wk_grid, pieces[ i ],
			    submission.answer[ i ].pos, submission.answer[ i ].rot ) ) {

      if ( verbose ) {
	cerr << "AFTER test_fallen_piece ( i: " << ( int ) i << " )" << endl ;
	print_grid( wk_grid ) ;
      }

      reduce_grid( wk_grid, submission.score[ i ], submission.len_chain[ i ], verbose ) ;

    }
    else {

      for ( tint j = i ; j < NB_PRED ; ++j ) {
	submission.score[ j ]     = -1 ;
	submission.len_chain[ j ] =  0 ;
      }

      break ;
    }
  }  // END - for ( tint i = 0 ; i < NB_PRED ; ++i ) {

}


float get_filling_rate( tint grid[ NB_LINE ][ NB_COL ] ) {

  int nb_non_empty_cell = get_nb_non_empty_cell( grid ) ;
  
  return ( float ) nb_non_empty_cell / ( NB_LINE * NB_COL ) ;
  
}


void print_sub_score( const struct submission_t & sub ) {
  cerr << "sub score: " ;
  for ( tint i = 0 ; i < NB_PRED ; ++i ) {
    cerr << "( " << ( int ) sub.score    [ i ]
	 << " "  << ( int ) sub.len_chain[ i ]
	 << " ) " ;
  }
  cerr << endl ;
}


void  get_better_submission( tint grid[ NB_LINE ][ NB_COL ],
			           struct submission_t & best_sub,
			     const struct submission_t & wk_sub ) {


  // for ( tint i = 0 ; i < NB_PRED ; ++i ) {
  //   if ( best_sub.score[ i ] < wk_sub.score[ i ] ) {
  //     return wk_sub ;
  //   }
  //   else if ( best_sub.score[ i ] > wk_sub.score[ i ] ) {
  //     return best_sub ;
  //   }
  // }

  // return best_sub ;


  float my_filling_rate = get_filling_rate( grid ) ;
  float his_filling_rate = get_filling_rate( his_grid ) ;

  int len_thresold ;
  
  if ( his_filling_rate < ( 5.0 * NB_COL ) / ( NB_LINE * NB_COL ) ) {
    // if ( my_filling_rate < ( 5.0 * NB_COL ) / ( NB_LINE * NB_COL ) )
    //   len_thresold = 4 ;
    // else if ( my_filling_rate < ( 7.0 * NB_COL ) / ( NB_LINE * NB_COL ) )
    //   len_thresold = 3 ;
    // else
    //   len_thresold = 1 ;

    len_thresold = 4 ;
  }
  else {
    if ( my_filling_rate < ( 5.0 * NB_COL ) / ( NB_LINE * NB_COL ) )
      len_thresold = 3 ;
    else if ( my_filling_rate < ( 7.0 * NB_COL ) / ( NB_LINE * NB_COL ) )
      len_thresold = 3 ;
    else
      len_thresold = 1 ;
  }

  // len_thresold = 4 ;

  // else if ( my_filling_rate < ( float )( 10 * NB_COL ) / ( NB_LINE * NB_COL ) )
  //   len_thresold = 2 ;
  // else 
  //   len_thresold = 1 ;

  tint cpt_best_len  = 0 ;
  tint cpt_wk_len    = 0 ;
  int cpt_best_score = 0 ;
  int cpt_wk_score   = 0 ;

  tint max_best_len = 0 ;

  // If the best move of the current best submission is already strong enough
  // try to replace it with a better one
  for ( tint i = 0 ; i < NB_PRED ; ++i ) {

    max_best_len = max( max_best_len, best_sub.len_chain[ i ] ) ;

    cpt_best_len += best_sub.len_chain[ i ] ;
    cpt_wk_len   += wk_sub.len_chain[ i ] ;

    cpt_best_score += best_sub.score[ i ] ;
    cpt_wk_score   += wk_sub.score[ i ] ;

  }

  if ( len_thresold <= max_best_len ) {

    for ( int i = 0 ; i < NB_PRED ; ++i ) {
      if ( best_sub.len_chain[ i ] != max_best_len ) {

	if ( max_best_len <= wk_sub.len_chain[ i ] ) {
	  best_sub = wk_sub ;
	  return ;
	}
	
      }
      else {
	return ;
      }
    }
    
  }

  for ( tint i = 0 ; i < NB_PRED ; ++i ) {

    if ( len_thresold <= wk_sub.len_chain[ i ] ) {

      if ( best_sub.len_chain[ i ] < wk_sub.len_chain[ i ] ) {
	best_sub = wk_sub ;
	return ;
      }
      else if ( best_sub.len_chain[ i ] > wk_sub.len_chain[ i ] ) {
	return ;
      }
      else {
	if ( cpt_best_len < cpt_wk_len ) {
	  best_sub = wk_sub ;
	  return ;
	}
	else if ( cpt_best_len > cpt_wk_len ) {
	  return ;
	}
	
      }

      // if ( best_sub.len_chain[ i ] <= wk_sub.len_chain[ i ] ) {
      // 	   // ( best_sub.score[ i ] < wk_sub.score[ i ] ) ) {
      // 	if ( cpt_best_len < cpt_wk_len ) {
      // 	  best_sub = wk_sub ;
      // 	  return ;
      // 	}
      // }
    }

    else if ( 2 <= wk_sub.len_chain[ i ] ) {
    // else {

      if ( ( best_sub.len_chain[ i ] < wk_sub.len_chain[ i ] ) &&
    	   ( wk_sub.len_chain[ i ] != 1 ) ) {
    	if ( ( cpt_best_len < cpt_wk_len ) &&
    	     ( cpt_best_score < cpt_wk_score ) ) {

    	  if ( ( 2 <= ind_iter ) && ( ind_iter <= 5 ) ) {
    	    cerr << "ELSE deepness: " << ( int ) i << endl ;
    	    cerr << " best sub: " ;
    	    print_sub_score( best_sub ) ;
    	    cerr << " wk   sub: " ;
    	    print_sub_score( wk_sub ) ;
    	    cerr << endl ;
    	  }
	  
    	  best_sub = wk_sub ;
    	  return ;
    	}
      }
      else if ( best_sub.len_chain[ i ] > wk_sub.len_chain[ i ] ) {
    	return ;
      }
    }
    
  }
    
  // int score0 = 0 , score1 = 0 ;
  // for ( tint i = 0 ; i < NB_PRED ; ++i ) {
  //   score0 += best_sub.score[ i ] ;
  //   score1 += wk_sub.score[ i ] ;
  // }

  // if ( score0 < score1 ) {
  //   // return wk_sub ;
  //   best_sub = wk_sub ;
  //   return ;

  // }
  // // else if ( score0 > score1 ) {
  // //   return best_sub ;
  // // }
  // // else {
  // //   return best_sub ;
  // // }
  
}


void read_input( tint pieces[ 8 ][ 2 ], bool verbose = false ) {

  for ( tint i = 0 ; i < 8 ; ++i ) {
    int colorA ; // color of the first block
    int colorB ; // color of the attached block
    // scanf("%d%d", &colorA, &colorB);

    cin >> colorA >> colorB ; cin.ignore( ) ;

    if ( verbose ) {
      cerr << "IN:" << colorA << " " << colorB << endl ;
    }

    pieces[ i ][ 0 ] = ( tint )colorA ;
    pieces[ i ][ 1 ] = ( tint )colorB ;
  }

  cin >> my_score ; cin.ignore( ) ;

  if ( verbose ) {
    cerr << "IN:" << my_score << endl ;
  }
    
  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    char row[ 7 ] ; // One line of the map ( '.' = empty,
                    //                       '0' = skull block,
                    //                       '1' to '5' = colored block )

    cin >> row ; cin.ignore( ) ;

    if ( verbose ) {
      cerr << "IN:" << row << endl ;
    }

    for ( tint j = 0 ; j < NB_COL ; ++j ) {
      if ( row[ j ] == '.' ) {
	my_grid[ i ][ j ] = ( tint )-1 ;
      }
      else {
	my_grid[ i ][ j ] = ( tint )( row[ j ] - '0' ) ;
      }
    }
  }

  cin >> his_score ; cin.ignore( ) ;

  if ( verbose ) {
    cerr << "IN:" << his_score << endl ;
  }

  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    char row[ 7 ] ; // One line of the map ( '.' = empty,
                    //                       '0' = skull block,
                    //                       '1' to '5' = colored block )

    cin >> row ; cin.ignore( ) ;

    if ( verbose ) {
      cerr << "IN:" << row << endl ;
    }

    for ( tint j = 0 ; j < NB_COL ; ++j ) {
      if ( row[ j ] == '.' ) {
	his_grid[ i ][ j ] = ( tint )-1 ;
      }
      else {
	his_grid[ i ][ j ] = ( tint )( row[ j ] - '0' ) ;
      }
    }
  }
}


void update_best_submission( struct submission_t & best_submission ) {

  for ( tint i = 0 ; i < NB_PRED - 1 ; ++i ) {
    best_submission.answer   [ i ] = best_submission.answer   [ i + 1 ] ;
    best_submission.score    [ i ] = best_submission.score    [ i + 1 ] ;
    best_submission.len_chain[ i ] = best_submission.len_chain[ i + 1 ] ;
  }

  best_submission.answer[ NB_PRED - 1 ] = get_answer( ) ;
  
  best_submission.score    [ NB_PRED - 1 ] = 0 ;
  best_submission.len_chain[ NB_PRED - 1 ] = 0 ;

}


void get_best_gene_MC( const float time_limit,
		       const int chunk_size,
		       const tint pieces[ 8 ][ 2 ],
		       struct submission_t & best_submission,
		       chrono::duration< float > & d ) {

  tint wk_grid[ NB_LINE ][ NB_COL ] ;
  struct submission_t wk_submission ;
  int nb_simu = 0 ;

  chrono::time_point< chrono::system_clock > start = now( ) ;

  while ( d.count( ) < ( time_limit ) ) {
    
    for ( int i_chunk = 0 ; i_chunk < chunk_size ; ++i_chunk ) {

      get_submission( wk_submission ) ;
      simulation( my_grid, pieces, wk_grid, wk_submission ) ;
      ++nb_simu ;

      get_better_submission( my_grid, best_submission, wk_submission ) ;

    }

    d = now( ) - start ;
  }

  cerr << "  ### MC NB SIMULATION: " << nb_simu << endl ;
  print_sub_score( best_submission ) ;
  cerr << endl ;
}


void print_output( const struct submission_t & best_submission ) {
  cerr << "pos: " << ( int ) best_submission.answer[ 0 ].pos << endl ;
  cerr << "rot: " << ( int ) best_submission.answer[ 0 ].rot << endl ;
  cout << ( int ) best_submission.answer[ 0 ].pos << " "
       << ( int ) best_submission.answer[ 0 ].rot << " "
       << ( get_filling_rate( my_grid ) < ( ( 5.0 * NB_COL ) / ( NB_LINE * NB_COL ) ) ) << " "
       << ( get_filling_rate( my_grid ) < ( ( 7.0 * NB_COL ) / ( NB_LINE * NB_COL ) ) ) << " "
       << endl ;
}


void reduce_unit_test( ) {

  cerr << "FROZAR" << endl ;

  tint grid_test[ NB_LINE ][ NB_COL ] = { -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1, -1, -1, -1, -1, -1,	\
					  -1,  2, -1, -1, -1, -1,	\
					  -1,  1, -1, -1, -1, -1,	\
					   1,  1,  1, -1, -1, -1,	\
					   2,  2,  2, -1, -1, -1, } ;


  int  dbg_score ;
  tint dbg_len_chain ;

  cerr << "Original grid" << endl ;
  print_grid( grid_test ) ;

  reduce_grid( grid_test, dbg_score, dbg_len_chain, true ) ;
  
  cerr << "After reduce_grid" << endl ;
  print_grid( grid_test ) ;

  exit( 0 ) ;

}


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement. */
int main( )
{
  // reduce_unit_test( ) ;
  
  chrono::duration< float > d ;
  chrono::time_point< chrono::system_clock > start = now( ) ;

  tint pieces[ 8 ][ 2 ];

  // struct submission_t wk_submission ;
  struct submission_t best_submission ;
  memset( &best_submission, 0, sizeof( struct submission_t ) ) ;

  // ### 0 - Initialise all the random stuff
  init_tab_alea( ) ;
  
  // ### 1 - FIRST iteration outside the game loop
  read_input( pieces ) ;
  // read_input( pieces, true ) ;
  // exit( 0 ) ;
  
  ++ind_iter ;
  cerr << "iter: " << ind_iter << endl ;

  d = now( ) - start ;
  
  get_best_gene_MC( TIME_LIMIT_MC_FIRST - d.count( ), 
		    CHUNK_SIZE_FIRST,
		    pieces,
		    best_submission,
		    d ) ;

  print_output( best_submission ) ;

  // UPDATE score and len_chain in best_submission
  update_best_submission( best_submission ) ;

  // exit( 0 ) ;

  // ### 0 - GAME loop
  tint wk_grid[ NB_LINE ][ NB_COL ] ;
  while ( 1 ) {

    his_previous_score = his_score ;
    read_input( pieces ) ;
    // read_input( pieces, true ) ;

    ++ind_iter ;
    cerr << "iter: " << ind_iter << endl ;

    // IF no attack has been done
    if ( his_previous_score == his_score ) {
      // UPDATE score and len_chain in best_submission
      simulation( my_grid, pieces, wk_grid, best_submission ) ;
    }
    else {
      get_submission( best_submission ) ;
    }
    
    start = now( ) ;
    d = now( ) - start ;
    
    get_best_gene_MC( TIME_LIMIT_MC_WHILE, 
		      CHUNK_SIZE_WHILE,
		      pieces,
		      best_submission,
		      d ) ;
  
    cerr << "best_submission.score: " ;
    for ( tint i = 0 ; i < NB_PRED ; ++i ) {
      cerr << "( " << ( int ) best_submission.score    [ i ]
	   << " "  << ( int ) best_submission.len_chain[ i ]
	   << " ) " ;
    }
    cerr << endl ;

    int ind_dbg_min = 5 ;
    int ind_dbg_max = 5 ;
    
    if ( ( ind_dbg_min <= ind_iter ) && ( ind_iter <= ind_dbg_max ) ) {
      simulation( my_grid, pieces, wk_grid, best_submission, true ) ;
    }
    
    print_output( best_submission ) ;

    // UPDATE score and len_chain in best_submission
    update_best_submission( best_submission ) ;

  }

  return 0;
}
