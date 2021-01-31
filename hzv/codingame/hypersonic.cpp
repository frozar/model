#pragma GCC optimize "-O3,omit-frame-pointer,inline"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
// #include <cinttypes>

#include <cstdint>

#include <ctime>

// floor function
#include <cmath>

// chrono
#include <chrono>

// memset, memcpy function
#include <cstring>

using namespace std;

#define P_DBG( x ) cerr << #x << ": " << x << endl
#define P_DBG_8( x ) cerr << #x << ": " << ( int ) x << endl

#define now chrono::system_clock::now

#define NB_PARAM_ENTITY  6
#define WIDTH           13
#define HEIGHT          11

#define WALL_ID    -1
#define BOX_0_ID    0
#define BOX_1_ID    1
#define BOX_2_ID    2
#define FLOOR_ID    3
#define ME_ID       4
#define ENNEMY_ID   5
#define BOMB_ID     6
#define OBJECT_1_ID 7
#define OBJECT_2_ID 8

#define ENTITY_NULL  -1
#define ENTITY_BODY   0
#define ENTITY_BOMB   1
#define ENTITY_OBJECT 2

#define EXTRA_PORTE 1
#define EXTRA_BOMB  2

#define AG_STAY  0
#define AG_NORTH 1
#define AG_SOUTH 2
#define AG_EAST  3
#define AG_WEST  4
#define AG_BOMB  5

// #define AG_POPULATION  1024 * 2
// #define AG_DEEPNESS    12
#define AG_POPULATION  1024 * 3 + 128
#define AG_DEEPNESS    12

#define AG_MALUS_ACTION            -2
#define AG_MALUS_BOMB             -10
#define AG_MALUS_DESTROY_OBJECT    -1
#define AG_MALUS_STAY              -0.2
#define AG_MALUS_DEATH          -1000
#define AG_BONUS_BOX               18
#define AG_BONUS_BOX_OBJECT        20
#define AG_BONUS_KILL              30
#define AG_BONUS_EXTRA_PORTE        3
#define AG_BONUS_EXTRA_BOMB         4

#define NB_ALEA_INT 1019 * 11

int8_t tab_alea[ NB_ALEA_INT ] = { 0 } ;

void init_tab_alea( ) {
  
// for ( int i = 0 ; i < 4 ; ++i ) {
//   cerr << "Before " << tab_alea[ i ] << endl ;
  // }

  // Real alea
  // srand( time( 0 ) ) ;

  for ( int i = 0 ; i < NB_ALEA_INT ; ++i ) {
    tab_alea[ i ] = ( int8_t ) floor( rand() / float( RAND_MAX ) * 6 ) ;
    // if ( i < 1000 ) {
    //   cerr << "Create alea " << ( int ) tab_alea[ i ] << endl ;
    // }

  }

}

int ind_alea = 0 ;

int8_t get_alea( ) {
  int8_t res = tab_alea[ ind_alea ] ;
  ++ind_alea ;
  
  if ( ind_alea == NB_ALEA_INT ) 
    ind_alea = 0 ;

  return res ;
  
}

int8_t grid[ WIDTH ][ HEIGHT ] = { 0 } ;
int8_t wk_grid[ WIDTH ][ HEIGHT ] ;
int myId = -1 ;
int8_t nb_player = -1 ;
int8_t nb_boxes  = -1 ;
int cur_iter = 0;

int dbg_cpt = 0 ;

typedef struct {
  int8_t data[ AG_DEEPNESS ] ;
  float  score ;
} gene_t ;


gene_t genes[ AG_POPULATION ] ;

void print_grid( int8_t grid[ WIDTH ][ HEIGHT ] ) {
  for ( int y = 0 ; y < HEIGHT ; ++y ) {
    for ( int x = 0 ; x < WIDTH ; ++x ) {
      switch( grid[ x ][ y ] ) {
      case WALL_ID :
	fprintf( stderr, "X" ) ;
	break;
      case BOX_0_ID :
	fprintf( stderr, "0" ) ;
	break;
      case BOX_1_ID :
	fprintf( stderr, "1" ) ;
	break;
      case BOX_2_ID :
	fprintf( stderr, "2" ) ;
	break;
      case FLOOR_ID :
	fprintf( stderr, "." ) ;
	break;
      case ME_ID :
	fprintf( stderr, "M" ) ;
	break;
      case ENNEMY_ID :
	fprintf( stderr, "E" ) ;
	break;
      case BOMB_ID :
	fprintf( stderr, "B" ) ;
	break;
      case OBJECT_1_ID :
	fprintf( stderr, "O" ) ;
	break;
      case OBJECT_2_ID :
	fprintf( stderr, "O" ) ;
	break;
      }
    }
    fprintf( stderr, "\n" ) ;
  }
}


void print_bomb( int8_t * entity ) {
  cerr << "bomb: "
       << "[ owner:"      << ( int ) entity[ 1 ] << " ] " 
       << "[ x: "         << ( int ) entity[ 2 ] << " ] " 
       << "[ y: "         << ( int ) entity[ 3 ] << " ] "
       << "[ countdown: " << ( int ) entity[ 4 ] << " ] "
       << "[ porte: "     << ( int ) entity[ 5 ] << " ]" << endl ;
}


void print_entity( int8_t * entity ) {
  for ( int i = 0 ; i < NB_PARAM_ENTITY ; ++i ) {
    fprintf( stderr, "%2d, ", entity[ i ] ) ;
  }
  cerr << endl ;
}


void print_entities( int8_t * v_entities, int nb_entities ) {
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    // for ( int j = 0 ; j < NB_PARAM_ENTITY ; ++j ) {
    //   fprintf( stderr, "%2d, ", v_entities[ i * NB_PARAM_ENTITY + j ] ) ;
    // }
    // cerr << endl ;
    print_entity( &v_entities[ i * NB_PARAM_ENTITY ] ) ;
  }
  cerr << endl ;
}


string action_to_str( int8_t action ) {

  switch ( action ) {
  case ( AG_STAY ) :
    return "STAY" ;
    break;
  case ( AG_NORTH ) :
    return "NORTH" ;
    break;
  case ( AG_SOUTH ) :
    return "SOUTH" ;
    break;
  case ( AG_EAST ) :
    return "EAST" ;
    break;
  case ( AG_WEST ) :
    return "WEST" ;
    break;
  case ( AG_BOMB ) :
    return "BOMB" ;
    break;
  }

  return "" ;
  
}


int8_t get_my_entity( int8_t * v_entities, int nb_entities, int myId, 
		      int8_t & entityType, int8_t & owner, int8_t & x, int8_t & y, 
		      int8_t & param1, int8_t & param2 ) {

  int8_t my_index = -1 ;
  
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    if ( ( v_entities[ i * NB_PARAM_ENTITY + 0 ] == ENTITY_BODY ) &&	\
	 ( v_entities[ i * NB_PARAM_ENTITY + 1 ] == myId ) ) {
      entityType = v_entities[ i * NB_PARAM_ENTITY + 0 ] ;
      owner      = v_entities[ i * NB_PARAM_ENTITY + 1 ] ;
      x          = v_entities[ i * NB_PARAM_ENTITY + 2 ] ;
      y          = v_entities[ i * NB_PARAM_ENTITY + 3 ] ;
      param1     = v_entities[ i * NB_PARAM_ENTITY + 4 ] ;
      param2     = v_entities[ i * NB_PARAM_ENTITY + 5 ] ;

      my_index = i ;
      break ;
    }
  }

  return my_index ;
}


void put_my_entity( int8_t * v_entities, int nb_entities, int myId, 
		    int8_t entityType, int8_t owner, int8_t x, int8_t y, 
		    int8_t param1, int8_t param2 ) {

  for ( int i = 0 ; i < nb_entities ; ++i ) {
    if ( ( v_entities[ i * NB_PARAM_ENTITY + 0 ] == ENTITY_BODY ) &&	\
	 ( v_entities[ i * NB_PARAM_ENTITY + 1 ] == myId ) ) {
      v_entities[ i * NB_PARAM_ENTITY + 0 ] = entityType ;
      v_entities[ i * NB_PARAM_ENTITY + 1 ] = owner      ;
      v_entities[ i * NB_PARAM_ENTITY + 2 ] = x          ;
      v_entities[ i * NB_PARAM_ENTITY + 3 ] = y          ;
      v_entities[ i * NB_PARAM_ENTITY + 4 ] = param1     ;
      v_entities[ i * NB_PARAM_ENTITY + 5 ] = param2     ;
      break ;
    }
  }
}


void get_entity( int8_t * v_entities, int ind,
		 int8_t & entityType, int8_t & owner, int8_t & x, int8_t & y, 
		 int8_t & param1, int8_t & param2 ) {
  entityType = v_entities[ ind * NB_PARAM_ENTITY + 0 ] ;
  owner      = v_entities[ ind * NB_PARAM_ENTITY + 1 ] ;
  x          = v_entities[ ind * NB_PARAM_ENTITY + 2 ] ;
  y          = v_entities[ ind * NB_PARAM_ENTITY + 3 ] ;
  param1     = v_entities[ ind * NB_PARAM_ENTITY + 4 ] ;
  param2     = v_entities[ ind * NB_PARAM_ENTITY + 5 ] ;
}


void put_entity( int8_t * v_entities, int ind,
		 int8_t entityType, int8_t owner, int8_t x, int8_t y, 
		 int8_t param1, int8_t param2 ) {
  v_entities[ ind * NB_PARAM_ENTITY + 0 ] = entityType ;
  v_entities[ ind * NB_PARAM_ENTITY + 1 ] = owner      ;
  v_entities[ ind * NB_PARAM_ENTITY + 2 ] = x          ;
  v_entities[ ind * NB_PARAM_ENTITY + 3 ] = y          ;
  v_entities[ ind * NB_PARAM_ENTITY + 4 ] = param1     ;
  v_entities[ ind * NB_PARAM_ENTITY + 5 ] = param2     ;
}


void discard_entity( int8_t * v_entities, int ind ) {
  v_entities[ ind * NB_PARAM_ENTITY + 0 ] = ENTITY_NULL ;
}


void cp_grid( int8_t src_grid[ WIDTH ][ HEIGHT ], int8_t dest_grid[ WIDTH ][ HEIGHT ] ) {
  memcpy( dest_grid, src_grid, WIDTH * HEIGHT * sizeof( int8_t ) );
}


void cp_v_entities( int8_t * src_v_entities, int8_t * dest_v_entities, int nb_entities ) {
  memcpy( dest_v_entities, src_v_entities, nb_entities * NB_PARAM_ENTITY * sizeof( int8_t ) );
}


void pretty_print_grid( int8_t grid[ WIDTH ][ HEIGHT ], int8_t * v_entities, int nb_entities, int myId ) {

  int8_t to_print_grid[ WIDTH ][ HEIGHT ] ;

  cp_grid( grid, to_print_grid ) ;

  int8_t entityType, owner, x, y, param1, param2 ;

  for ( int ind = 0 ; ind < nb_entities ; ++ind ) {
    get_entity( v_entities, ind,
		entityType, owner, x, y, param1, param2 ) ;

    if ( entityType == ENTITY_BODY ) {
      if ( owner == myId ) {
	to_print_grid[ x ][ y ] = ME_ID ;
      } else {
	to_print_grid[ x ][ y ] = ENNEMY_ID ;
      }
    } else if ( entityType == ENTITY_BOMB ) {

      to_print_grid[ x ][ y ] = BOMB_ID ;

    } else if ( entityType == ENTITY_OBJECT ) {

      if ( param1 == 1 ) {
	to_print_grid[ x ][ y ] = OBJECT_1_ID ;
      } else {
	to_print_grid[ x ][ y ] = OBJECT_2_ID ;
      }
    }
  }

  print_grid( to_print_grid ) ;

  cerr << endl;
}


int isGridObstacle( int8_t grid[ WIDTH ][ HEIGHT ], int8_t x, int8_t y ) {

  return ( grid[ x ][ y ] == WALL_ID  ) ||	   \
    ( grid[ x ][ y ] == BOX_0_ID ) ||	   \
    ( grid[ x ][ y ] == BOX_1_ID ) ||	   \
    ( grid[ x ][ y ] == BOX_2_ID ) ;

}


int isNorthMoveImpossible( int8_t grid[ WIDTH ][ HEIGHT ], int8_t x, int8_t y, 
			 int8_t * v_entities, int nb_entities ) {

  int res = ( y == ( 0 ) ) || isGridObstacle( grid, x, y - 1 ) ;

  if ( res == 0 ) {
    int8_t entityType, owner, x0, y0, param1, param2 ;

    for ( int i = 0 ; i < nb_entities ; ++i ) {
      get_entity( v_entities, i,
		  entityType, owner, x0, y0, param1, param2 ) ;

      if ( ( entityType == ENTITY_BOMB ) && ( x0 == x ) && ( y0 == ( y - 1 ) ) ) {
	res = 1 ;
	break ;
      }
      
    }
  }

  return res ;  

}


int isSouthMoveImpossible( int8_t grid[ WIDTH ][ HEIGHT ], int8_t x, int8_t y, 
			 int8_t * v_entities, int nb_entities ) {

  int res = ( y == ( HEIGHT - 1 ) ) || isGridObstacle( grid, x, y + 1 ) ;

  if ( res == 0 ) {
    int8_t entityType, owner, x0, y0, param1, param2 ;

    for ( int i = 0 ; i < nb_entities ; ++i ) {
      get_entity( v_entities, i,
		  entityType, owner, x0, y0, param1, param2 ) ;

      if ( ( entityType == ENTITY_BOMB ) && ( x0 == x ) && ( y0 == ( y + 1 ) ) ) {
	res = 1 ;
	break ;
      }
      
    }
  }

  return res ;

}


int isEastMoveImpossible( int8_t grid[ WIDTH ][ HEIGHT ], int8_t x, int8_t y, 
			int8_t * v_entities, int nb_entities ) {

  int res = ( x == ( WIDTH - 1 ) ) || isGridObstacle( grid, x + 1, y ) ;

  if ( res == 0 ) {
    int8_t entityType, owner, x0, y0, param1, param2 ;

    for ( int i = 0 ; i < nb_entities ; ++i ) {
      get_entity( v_entities, i,
		  entityType, owner, x0, y0, param1, param2 ) ;

      if ( ( entityType == ENTITY_BOMB ) && ( x0 == ( x + 1 ) ) && ( y0 == ( y ) ) ) {
	res = 1 ;
	break ;
      }
      
    }
  }

  return res ;

}


int isWestMoveImpossible( int8_t grid[ WIDTH ][ HEIGHT ], int8_t x, int8_t y, 
			int8_t * v_entities, int nb_entities ) {
    
  int res = ( x == ( 0 ) ) || isGridObstacle( grid, x - 1, y ) ;

  if ( res == 0 ) {
    int8_t entityType, owner, x0, y0, param1, param2 ;

    for ( int i = 0 ; i < nb_entities ; ++i ) {
      get_entity( v_entities, i,
		  entityType, owner, x0, y0, param1, param2 ) ;

      if ( ( entityType == ENTITY_BOMB ) && ( x0 == ( x - 1 ) ) && ( y0 == ( y ) ) ) {
	res = 1 ;
	break ;
      }
      
    }
  }

  return res ;

}


int8_t get_nb_boxes( ) {

  int8_t res = 0 ;
  for ( int x = 0 ; x < WIDTH ; ++x ) {
    for ( int y = 0 ; y < HEIGHT ; ++y ) {
      if ( ( grid[ x ][ y ] == BOX_0_ID ) || \
	   ( grid[ x ][ y ] == BOX_1_ID ) || \
	   ( grid[ x ][ y ] == BOX_2_ID ) ) {
	++res ;
      }
    }
  }

  return res ;
}

int8_t * wk_v_entities ; // [ nb_entities * NB_PARAM_ENTITY * 2 + 1 ] ;
int8_t * bomb_to_explose ; // [ wk_nb_entities + 1 ] ;
int8_t * boxes_to_disappear ; // [ nb_boxes * 2 + 2 ] ;
int8_t * entity_to_discard ; // [ wk_nb_entities + 2 ] ;
float player_score[ 4 ] ;

void simulation( int8_t * v_entities, int nb_entities,
		 gene_t & gene, bool verbose = false ) {

  cp_grid( grid, wk_grid ) ;

  // int8_t wk_v_entities[ nb_entities * NB_PARAM_ENTITY * 2 + 1 ] ;
  // for ( int i = 0 ; i < ( nb_entities * NB_PARAM_ENTITY * 2 ) ; ++i ) {
  //   wk_v_entities[ i ] = 0 ;
  // }
  memset( wk_v_entities, 0, ( nb_entities * NB_PARAM_ENTITY * 2 + 1 ) * sizeof( int8_t ) ) ;

  cp_v_entities( v_entities, wk_v_entities, nb_entities ) ;

  int wk_nb_entities = nb_entities ;

  // int dbg_ctp = 0 ;

  float factor ;

  if ( verbose ) {
    cerr << "ORIGIN gene.score: " << gene.score << endl ;
    
    cerr << "ORIGIN Grid" << endl ;
    pretty_print_grid( wk_grid, wk_v_entities, wk_nb_entities, myId ) ;
  }
  
  // ***** LOOP OF SIMULATION FOR A GENE
  for ( int ind_gene = 0 ; ind_gene < AG_DEEPNESS ; ++ind_gene ) {

    // if ( to_debug ) {
    //   if ( ind_gene == 4 ) {
    // 	verbose = true ;
    //   }
    //   else {
    // 	verbose = false ;
    //   }
    // }
    
    factor = ( 1.0 / ( ind_gene + 1 ) ) ;

    // if ( verbose ) {
    //   cerr << "BEFORE ANYTHING list entities" << endl ;
    //   print_entities( wk_v_entities, wk_nb_entities ) ;
    // }


    // float player_score[ ( int ) nb_player + 2 ] ;
    // for ( int i = 0 ; i < nb_player ; ++i ) {
    //   player_score[ i ] = 0 ;
    // }
    memset( player_score, 0, 4 * sizeof( float ) ) ;

    // 0 *** UPDATE BOMB COUNTDOWN

    for ( int i = 0 ; i < wk_nb_entities ; ++i ) {
      if ( wk_v_entities[ i * NB_PARAM_ENTITY + 0 ] == ENTITY_BOMB ) {
	wk_v_entities[ i * NB_PARAM_ENTITY + 4 ] -= 1 ;
      }
    }

    // 0 *** END - UPDATE BOMB COUNTDOWN


    // 1 *** IDENTIFY CAREFULLY EVERY BOMB THAT SHOULD EXPLOSE

    // int8_t bomb_to_explose[ wk_nb_entities + 1 ] ;
    // for ( int i = 0 ; i < wk_nb_entities ; ++i ) {
    //   bomb_to_explose[ i ] = 0 ;
    // }
    memset( bomb_to_explose, 0, ( nb_entities + 1 ) * sizeof( int8_t ) ) ;

    // memset( bomb_to_explose, 0, sizeof bomb_to_explose ) ;
    int8_t nb_bomb_to_explose = 0 ;

    // int8_t boxes_to_disappear[ nb_boxes * 2 + 2 ] ;
    // for ( int i = 0 ; i < ( nb_boxes * 2 ) ; ++i ) {
    //   boxes_to_disappear[ i ] = 0 ;
    // }
    memset( boxes_to_disappear, 0, ( nb_boxes * 2 + 2 ) * sizeof( int8_t ) ) ;
    
    int8_t nb_boxes_to_disappear = 0 ;

    // for ( int i = 0 ; i < ( wk_nb_entities ) ; ++i ) {
    //   entity_to_discard[ i ] = 0 ;
    // }
    memset( entity_to_discard, 0, ( nb_entities + 2 ) * sizeof( int8_t ) ) ;
    int8_t nb_entity_to_discard = 0 ;

    int8_t haveChanged = 1 ;

    // TODO: take into account if a box is touch by many blast from different player

    // Put the "while" statement here
    while ( haveChanged ) {
      haveChanged        = 0 ;
      nb_bomb_to_explose = 0 ;

      // First pass for trigger bombs
      for ( int i = 0 ; i < wk_nb_entities ; ++i ) {
	if ( wk_v_entities[ i * NB_PARAM_ENTITY + 0 ] == ENTITY_BOMB ) {
	  if ( wk_v_entities[ i * NB_PARAM_ENTITY + 4 ] == 0 ) {
	    bomb_to_explose[ nb_bomb_to_explose++ ] = i ;
	  }
	}
      }  // END First pass for trigger bombs

      // if ( verbose ) {
      // 	print_entities( wk_v_entities, wk_nb_entities ) ;
      // }

      // Second pass for enchained bombs
      for ( int i = 0 ; i < nb_bomb_to_explose ; ++i ) {
	int ind_entity = bomb_to_explose[ i ] ;

	// if ( verbose ) {
	//   cerr << "Trigger Add bomb to explose " << endl ;
	//   print_bomb( &wk_v_entities[ ind_entity * NB_PARAM_ENTITY ] ) ;
	// }

	int8_t entityType_B0, owner_B0, x_B0, y_B0, param1_B0, param2_B0 ;
	get_entity( wk_v_entities, ind_entity,
		    entityType_B0, owner_B0, x_B0, y_B0, param1_B0, param2_B0 ) ;


	int8_t entityType_1, owner_1, x_1, y_1, param1_1, param2_1 ;
	for ( int j = 0 ; j < wk_nb_entities ; ++j ) {
	  get_entity( wk_v_entities, j,
		      entityType_1, owner_1, x_1, y_1, param1_1, param2_1 ) ;

	  // Dealing the North side
	  for ( int d_y = 0 ; d_y < param2_B0 ; ++d_y ) {

	    // Check grid obstacle
	    if ( ( ( y_B0 - d_y ) < 0 ) ||			\
		 ( wk_grid[ x_B0 ][ y_B0 - d_y ] == WALL_ID ) ) {

	      break ;

	    } 

	    else if ( ( wk_grid[ x_B0 ][ y_B0 - d_y ] == BOX_0_ID ) ||	\
		      ( wk_grid[ x_B0 ][ y_B0 - d_y ] == BOX_1_ID ) ||	\
		      ( wk_grid[ x_B0 ][ y_B0 - d_y ] == BOX_2_ID ) ) {

	      if ( wk_grid[ x_B0 ][ y_B0 - d_y ] == BOX_0_ID ) { 
		player_score[ owner_B0 ] += AG_BONUS_BOX ;
	      }
	    
	      else if ( ( wk_grid[ x_B0 ][ y_B0 - d_y ] == BOX_1_ID ) || \
			( wk_grid[ x_B0 ][ y_B0 - d_y ] == BOX_2_ID ) ) {
		player_score[ owner_B0 ] += AG_BONUS_BOX_OBJECT ;
	      }
	    
	      boxes_to_disappear[ 2 * nb_boxes_to_disappear     ] = x_B0 ;
	      boxes_to_disappear[ 2 * nb_boxes_to_disappear + 1 ] = y_B0 - d_y ;
	      ++nb_boxes_to_disappear ;
	      break ;

	    }
	    
	    else {

	      if ( ( ind_entity != j ) &&	      \
		   ( x_1 == x_B0 ) &&		      \
		   ( y_1 == ( y_B0 - d_y ) ) ) {

		// Trigger bomb on the path
		if ( ( entityType_1 == ENTITY_BOMB ) && \
		     ( param1_1 != 0 ) ) {
		  
		  wk_v_entities[ j * NB_PARAM_ENTITY + 4 ] = 0 ;
		  bomb_to_explose[ nb_bomb_to_explose++ ] = j ;
		  haveChanged = 1 ;
		  break ;

		}

		// Delete entities on the path
		else if ( entityType_1 == ENTITY_BODY ) {
		  player_score[ owner_1 ] += AG_BONUS_KILL ;
		  discard_entity( wk_v_entities, j ) ;		  
		}

		else if ( entityType_1 == ENTITY_OBJECT ) {
		  player_score[ owner_1 ] += AG_MALUS_DESTROY_OBJECT ;
		  entity_to_discard[ nb_entity_to_discard++ ] = j ;
		  break ;

		}

	      }
	    }

	  }  // for ( int d_y = 0 ; d_y < param2_B0 ; ++d_y ) {


	  // Dealing the South side
	  for ( int d_y = 0 ; d_y < param2_B0 ; ++d_y ) {

	    // Check grid obstacle
	    if ( ( ( HEIGHT - 1 ) < ( y_B0 + d_y ) ) ||		\
		 ( wk_grid[ x_B0 ][ y_B0 + d_y ] == WALL_ID ) ) {
	    
	      break ;

	    }

	    else if ( ( wk_grid[ x_B0 ][ y_B0 + d_y ] == BOX_0_ID ) ||	\
		      ( wk_grid[ x_B0 ][ y_B0 + d_y ] == BOX_1_ID ) ||	\
		      ( wk_grid[ x_B0 ][ y_B0 + d_y ] == BOX_2_ID ) ) {
	    
	      if ( wk_grid[ x_B0 ][ y_B0 + d_y ] == BOX_0_ID ) { 
		player_score[ owner_B0 ] += AG_BONUS_BOX ;
	      }
	    
	      else if ( ( wk_grid[ x_B0 ][ y_B0 + d_y ] == BOX_1_ID ) || \
			( wk_grid[ x_B0 ][ y_B0 + d_y ] == BOX_2_ID ) ) {
		player_score[ owner_B0 ] += AG_BONUS_BOX_OBJECT ;
	      }
	    
	      boxes_to_disappear[ 2 * nb_boxes_to_disappear     ] = x_B0 ;
	      boxes_to_disappear[ 2 * nb_boxes_to_disappear + 1 ] = y_B0 + d_y ;
	      ++nb_boxes_to_disappear ;
	      break ;
	    
	    }

	    else {

	      if ( ( ind_entity != j ) &&		\
		   ( x_1 == x_B0 ) &&			\
		   ( y_1 == ( y_B0 + d_y ) ) ) {

		// Trigger bomb on the path
		if ( ( entityType_1 == ENTITY_BOMB ) && \
		     ( param1_1 != 0 ) ) {
		  
		  wk_v_entities[ j * NB_PARAM_ENTITY + 4 ] = 0 ;
		  bomb_to_explose[ nb_bomb_to_explose++ ] = j ;
		  haveChanged = 1 ;
		  break ;

		}

		// Delete entities on the path
		else if ( entityType_1 == ENTITY_BODY ) {
		  player_score[ owner_1 ] += AG_BONUS_KILL ;
		  discard_entity( wk_v_entities, j ) ;		  
		}
		
		else if ( entityType_1 == ENTITY_OBJECT ) {
		  player_score[ owner_1 ] += AG_MALUS_DESTROY_OBJECT ;
		  entity_to_discard[ nb_entity_to_discard++ ] = j ;
		  break ;

		}
		
	      }	      
	    }

	  } // for ( int d_y = 0 ; d_y < param2_B0 ; ++d_y ) {


	  // Dealing the East side
	  for ( int d_x = 0 ; d_x < param2_B0 ; ++d_x ) {

	    // Check grid obstacle
	    if ( ( ( WIDTH - 1 ) < ( x_B0 + d_x ) ) ||		\
		 ( wk_grid[ x_B0 + d_x ][ y_B0 ] == WALL_ID ) ) {
	    
	      break ;
	      
	    }
	  
	    else if ( ( wk_grid[ x_B0 + d_x ][ y_B0 ] == BOX_0_ID ) ||	\
		      ( wk_grid[ x_B0 + d_x ][ y_B0 ] == BOX_1_ID ) ||	\
		      ( wk_grid[ x_B0 + d_x ][ y_B0 ] == BOX_2_ID ) ) {

	      if ( wk_grid[ x_B0 + d_x ][ y_B0 ] == BOX_0_ID ) { 
		player_score[ owner_B0 ] += AG_BONUS_BOX ;
	      }

	      else if ( ( wk_grid[ x_B0 + d_x ][ y_B0 ] == BOX_1_ID ) || \
			( wk_grid[ x_B0 + d_x ][ y_B0 ] == BOX_2_ID ) ) {
		player_score[ owner_B0 ] += AG_BONUS_BOX_OBJECT ;
	      }

	      boxes_to_disappear[ 2 * nb_boxes_to_disappear     ] = x_B0 + d_x ;
	      boxes_to_disappear[ 2 * nb_boxes_to_disappear + 1 ] = y_B0 ;
	      ++nb_boxes_to_disappear ;
	      break ;
	    }

	    else {

	      if ( ( ind_entity != j ) &&	      \
		   ( x_1 == ( x_B0 + d_x ) ) &&	      \
		   ( y_1 == y_B0 ) ) {

		// Trigger bomb on the path
		if ( ( entityType_1 == ENTITY_BOMB ) && \
		     ( param1_1 != 0 ) ) {

		  wk_v_entities[ j * NB_PARAM_ENTITY + 4 ] = 0 ;
		  bomb_to_explose[ nb_bomb_to_explose++ ] = j ;
		  haveChanged = 1 ;
		  break ;

		}

		// Delete entities on the path
		else if ( entityType_1 == ENTITY_BODY ) {
		  player_score[ owner_1 ] += AG_BONUS_KILL ;
		  discard_entity( wk_v_entities, j ) ;		  
		}

		else if ( entityType_1 == ENTITY_OBJECT ) {
		  player_score[ owner_1 ] += AG_MALUS_DESTROY_OBJECT ;
		  entity_to_discard[ nb_entity_to_discard++ ] = j ;
		  break ;
		
		}

	      }
	    }

	  } // END for ( int d_x = 0 ; d_x < param2_B0 ; ++d_x ) {


	  // Dealing the West side
	  for ( int d_x = 0 ; d_x < param2_B0 ; ++d_x ) {

	    // Check grid obstacle
	    if ( ( ( x_B0 - d_x ) < 0 ) ||				\
		 ( wk_grid[ x_B0 - d_x ][ y_B0 ] == WALL_ID ) ) {

	      break ;

	    }
	  
	    else if ( ( wk_grid[ x_B0 - d_x ][ y_B0 ] == BOX_0_ID ) || \
		      ( wk_grid[ x_B0 - d_x ][ y_B0 ] == BOX_1_ID ) ||	\
		      ( wk_grid[ x_B0 - d_x ][ y_B0 ] == BOX_2_ID ) ) {
	    
	      if ( wk_grid[ x_B0 - d_x ][ y_B0 ] == BOX_0_ID ) { 
		player_score[ owner_B0 ] += AG_BONUS_BOX ;
	      }

	      else if ( ( wk_grid[ x_B0 - d_x ][ y_B0 ] == BOX_1_ID ) ||	\
			( wk_grid[ x_B0 - d_x ][ y_B0 ] == BOX_2_ID ) ) {
		player_score[ owner_B0 ] += AG_BONUS_BOX_OBJECT ;
	      }

	      boxes_to_disappear[ 2 * nb_boxes_to_disappear     ] = x_B0 - d_x ;
	      boxes_to_disappear[ 2 * nb_boxes_to_disappear + 1 ] = y_B0 ;
	      ++nb_boxes_to_disappear ;
	      break ;
	    }

	    else {

	      if ( ( ind_entity != j ) &&		      \
		   ( x_1 == ( x_B0 - d_x ) ) &&	      \
		   ( y_1 == y_B0 ) ) {
	      
		// Trigger bomb on the path
		if ( ( entityType_1 == ENTITY_BOMB ) &&	\
		     ( param1_1 != 0 ) ) {

		  wk_v_entities[ j * NB_PARAM_ENTITY + 4 ] = 0 ;
		  bomb_to_explose[ nb_bomb_to_explose++ ] = j ;
		  haveChanged = 1 ;
		  break ;

		}

		// Delete entities on the path
		else if ( entityType_1 == ENTITY_BODY ) {
		  player_score[ owner_1 ] += AG_BONUS_KILL ;
		  discard_entity( wk_v_entities, j ) ;		  
		}

		else if ( entityType_1 == ENTITY_OBJECT ) {
		  player_score[ owner_1 ] += AG_MALUS_DESTROY_OBJECT ;
		  entity_to_discard[ nb_entity_to_discard++ ] = j ;
		  break ;

		}

	      }	      
	    }
	  } // for ( int d_x = 0 ; d_x < param2_B0 ; ++d_x ) {


	} // END for ( int j = 0 ; j < wk_nb_entities ; ++j ) {

      } // END Second pass for enchained bombs

      // if ( verbose ) {

      // 	cerr << "In while "  << endl ;
      // 	cerr << "nb_bomb_to_explose: " << ( int ) nb_bomb_to_explose << endl ;
      // 	cerr << "haveChanged: " << ( int ) haveChanged << endl ;

      // }

    } // END while ( haveChanged )


    for ( int i = 0 ; i < nb_entity_to_discard ; ++i ) {

      int ind_entity = entity_to_discard[ i ] ;
      discard_entity( wk_v_entities, ind_entity ) ;

    }



    // if ( verbose ) {
    //   cerr << "Bomb which will explose" << endl ;
    //   for ( int i = 0 ; i < nb_bomb_to_explose ; ++i ) {
    // 	int ind_entity = bomb_to_explose[ i ] ;
    // 	print_bomb( &wk_v_entities[ ind_entity * NB_PARAM_ENTITY ] ) ;
    //   }
    //   cerr << endl;
    // }

    //   if ( 0 < nb_bomb_to_explose ) {
    // 	cerr << "id bomb_to_explose: " ;
    // 	for ( int i = 0 ; i < wk_nb_entities ; ++i ) {
    // 	  if ( bomb_to_explose[ i ] != 0 ) {
    // 	    cerr << ( int ) bomb_to_explose[ i ] << ", " ;
    // 	  }
    // 	}
    // 	cerr << endl;
    //   }
    // }

    // 1 *** END - IDENTIFY CAREFULLY EVERY BOMB THAT SHOULD EXPLOSE


    // 2 *** UPDATE THE GRID AND THE ENTITIES ACCORDINGLY TO THE BOMB WHICH EXPLOSE

    // if ( verbose ) { 
    //   cerr << "Grid BEFORE explosion" << endl ;
    //   pretty_print_grid( wk_grid, wk_v_entities, wk_nb_entities, myId ) ;
    // }

    // if ( cur_iter == 132 ) {
    //   if ( ( 400 <= dbg_cpt ) && ( dbg_cpt < 1100 ) ) {
    // 	cerr << "nb_player " << ( int ) nb_player << endl;
    // 	cerr << "sizeof player_score: " << sizeof player_score << endl;
    //   }
    // }

    // if ( verbose ) {

    //   cerr << "player_score: " << player_score[ myId ]  << endl;
    //   cerr << "nb_boxes: " << ( int ) nb_boxes << endl ;

    // }

    // GENE SCORE
    if ( player_score[ myId ] != 0 ) { 
      // gene.score += player_score[ myId ] + ( 1.0 / ( ind_gene + 1 ) );
      gene.score += player_score[ myId ] * factor ;
      // gene.score += player_score[ myId ] ;
    }
    
    // if ( verbose ) {
    //   if ( 0 < nb_boxes_to_disappear ) {
    // 	cerr << "nb_boxes_to_disappear: " << ( int ) nb_boxes_to_disappear << endl;

    // 	int x_0 ;
    // 	int y_0 ;

    // 	for ( int i = 0 ; i < nb_boxes_to_disappear ; ++i ) {

    // 	  x_0 = boxes_to_disappear[ 2 * i     ] ;
    // 	  y_0 = boxes_to_disappear[ 2 * i + 1 ] ;

    // 	  cerr << "coor boxes to disappear: " << ( int ) x_0 << ", "	\
    // 	       << ( int ) y_0 << endl;
    // 	}
    // 	cerr << endl ;
    //   }
    //   // cerr << "BEFORE list entities" << endl ;
    //   // print_entities( wk_v_entities, wk_nb_entities ) ;
    // }

    // If some boxe contains an object, create it in the array of entities
    for ( int i = 0 ; i < nb_boxes_to_disappear ; ++i ) {

      int x_0 = boxes_to_disappear[ 2 * i     ] ;
      int y_0 = boxes_to_disappear[ 2 * i + 1 ] ;

      // if ( verbose ) 
      // 	cerr << "coor boxes to disappear: " << ( int ) x_0 << ", "
      // 	     << ( int ) y_0 << endl;
      
      if ( wk_grid[ x_0 ][ y_0 ] == BOX_0_ID ) {
	wk_grid[ x_0 ][ y_0 ] = FLOOR_ID ;
      }

      else if ( wk_grid[ x_0 ][ y_0 ] == BOX_1_ID ) { 
	wk_grid[ x_0 ][ y_0 ] = OBJECT_1_ID ;

	put_entity( wk_v_entities, wk_nb_entities,
		    ENTITY_OBJECT, 0, x_0, y_0, 
		    1, 0 ) ;

	++wk_nb_entities ;

      }

      else if ( wk_grid[ x_0 ][ y_0 ] == BOX_2_ID ) { 
	wk_grid[ x_0 ][ y_0 ] = OBJECT_2_ID ;

	put_entity( wk_v_entities, wk_nb_entities,
		    ENTITY_OBJECT, 0, x_0, y_0, 
		    2, 0 ) ;

	++wk_nb_entities ;

      }

    }

    // 2 *** END - UPDATE THE GRID AND THE ENTITIES ACCORDINGLY TO THE BOMB WHICH EXPLOSE


    // 3 *** CHECK IF I AM STILL ALIVE

    int8_t entityType, owner, x, y, param1, param2 ;
    int8_t my_index = get_my_entity( wk_v_entities, wk_nb_entities, myId, 
				     entityType, owner, x, y, param1, param2 ) ;

    if ( my_index == -1 ) {
      // GENE SCORE
      gene.score += AG_MALUS_DEATH ;
      break ;
    }

    // 3 *** END - CHECK IF I AM STILL ALIVE

    // if ( verbose ) {
    //   cerr << "Current v_entities: " << endl;
    //   print_entities( wk_v_entities, wk_nb_entities ) ;
    // }

    // 4 *** PLAY AN ACTION ON THE WORKING GRID

    int action = gene.data[ ind_gene ] ;

    if ( verbose ) { 
      cerr << "action: " << ind_gene << " | " << action_to_str( action ) << endl;
      // cerr << "cur_pos: "
      // 	   << "[ x: " << ( int ) x << " ] " 
      // 	   << "[ y: " << ( int ) y << " ]" << endl ;
    }

    switch( action ) {
    case ( AG_STAY ) :
      if ( verbose ) 
	cerr << "The possible STAY move" << endl ;

	// GENE SCORE
	gene.score += AG_MALUS_STAY * factor ;
	
      break ;
	
    case ( AG_NORTH ) :
      get_my_entity( wk_v_entities, wk_nb_entities, myId, 
		     entityType, owner, x, y, param1, param2 ) ;

      if ( isNorthMoveImpossible( wk_grid, x, y, wk_v_entities, wk_nb_entities ) ) {

	if ( verbose ) 
	  cerr << "Impossible North move" << endl ;

	// GENE SCORE
	gene.score += AG_MALUS_ACTION * factor ;
	
	break ;

      } else { 

	put_my_entity( wk_v_entities, wk_nb_entities, myId, 
		       entityType, owner, x, y - 1, param1, param2 ) ;

      }
      break ;

    case ( AG_SOUTH ) :
      get_my_entity( wk_v_entities, wk_nb_entities, myId, 
		     entityType, owner, x, y, param1, param2 ) ;

      if ( isSouthMoveImpossible( wk_grid, x, y, wk_v_entities, wk_nb_entities ) ) {
	  
	if ( verbose ) 
	  cerr << "Impossible South move" << endl ;

	// GENE SCORE
	gene.score += AG_MALUS_ACTION * factor ;
	
	break ;

      } else { 

	put_my_entity( wk_v_entities, wk_nb_entities, myId, 
		       entityType, owner, x, y + 1, param1, param2 ) ;

      }

      break ;

    case ( AG_EAST ) :
      get_my_entity( wk_v_entities, wk_nb_entities, myId, 
		     entityType, owner, x, y, param1, param2 ) ;

      if ( isEastMoveImpossible( wk_grid, x, y, wk_v_entities, wk_nb_entities ) ) {
	  
	if ( verbose ) 
	  cerr << "Impossible East move" << endl ;

	// GENE SCORE
	gene.score += AG_MALUS_ACTION * factor ;
	
	break ;

      } else { 

	put_my_entity( wk_v_entities, wk_nb_entities, myId, 
		       entityType, owner, x + 1, y, param1, param2 ) ;

      }
      break ;

    case ( AG_WEST ) :
      get_my_entity( wk_v_entities, wk_nb_entities, myId, 
		     entityType, owner, x, y, param1, param2 ) ;

      if ( isWestMoveImpossible( wk_grid, x, y, wk_v_entities, wk_nb_entities ) ) {

	if ( verbose ) {
	  cerr << "Impossible West move" << endl ;
	  cerr << "gene.score: " << gene.score << endl ;
	}
	
	// GENE SCORE
	gene.score += AG_MALUS_ACTION * factor ;
	
	break ;

      } else { 

	put_my_entity( wk_v_entities, wk_nb_entities, myId, 
		       entityType, owner, x - 1, y, param1, param2 ) ;

      }
      break ;

    case ( AG_BOMB ) :

      if ( param1 <= 0 ) {
	  
	if ( verbose ) 
	  cerr << "Impossible to drop bomb" << endl ;
	  
	// GENE SCORE
	gene.score += AG_MALUS_BOMB * factor;
	
      }

      else {
	bool already_bomb = false ;

	for ( int ind_entity_0 = 0 ;
	      ind_entity_0 < wk_nb_entities ;
	      ++ind_entity_0 ) {

	  
	  int8_t entityType_0, owner_0, x_0, y_0, param1_0, param2_0 ;
      
	  get_entity( wk_v_entities, ind_entity_0, 
		      entityType_0, owner_0, x_0, y_0, param1_0, param2_0 ) ;

	  if ( ( entityType_0 == ENTITY_BOMB ) &&
	       ( x == x_0 ) &&
	       ( y == y_0 ) ) {

	    if ( verbose ) 
	      cerr << "Already a bomb" << endl ;
	  
	    // GENE SCORE
	    gene.score += AG_MALUS_BOMB * factor;
	    already_bomb = true ;
	    break ;
	  }
	}

	if ( ! already_bomb ) {

	put_entity( wk_v_entities, wk_nb_entities,
		    ENTITY_BOMB, myId, x, y, 
		    8, param2 ) ;
	  
	++wk_nb_entities ;
	
	put_my_entity( wk_v_entities, wk_nb_entities, myId, 
		       entityType, owner, x, y, param1 - 1, param2 ) ;

	}

      }
      break ;
    }

    // 4 *** END - PLAY AN ACTION ON THE WORKING GRID

    if ( verbose ) {
      get_my_entity( wk_v_entities, wk_nb_entities, myId, 
		     entityType, owner, x, y, param1, param2 ) ;

      // cerr << "update_pos: "
      // 	   << "[ x: " << ( int ) x << " ] " 
      // 	   << "[ y: " << ( int ) y << " ]" << endl ;
      cerr << "gene.score: " << gene.score << endl ;
      cerr << endl ;
    }



    // 5 *** UPDATE THE ENTITIES IN CASE AN OBJECT WAS TAKEN

    // if ( cur_iter == 132 ) {

    //   if ( ( 400 <= dbg_cpt ) && ( dbg_cpt < 1100 ) ) {
    // 	cerr << "sizeof player_score: " << sizeof player_score << endl;
    //   }
      
    // }

    // for ( int i = 0 ; i < nb_player ; ++i ) {
    //   player_score[ i ] = 0 ;
    // }
    memset( player_score, 0, 4 * sizeof( float ) ) ;
    
    // for ( int i = 0 ; i < ( wk_nb_entities ) ; ++i ) {
    //   entity_to_discard[ i ] = 0 ;
    // }
    memset( entity_to_discard, 0, ( wk_nb_entities + 2 ) * sizeof( int8_t ) ) ;
    nb_entity_to_discard = 0 ;

    for ( int ind_entity_0 = 0 ; ind_entity_0 < wk_nb_entities ; ++ind_entity_0 ) {

      int8_t entityType_0, owner_0, x_0, y_0, param1_0, param2_0 ;
      
      get_entity( wk_v_entities, ind_entity_0, 
		  entityType_0, owner_0, x_0, y_0, param1_0, param2_0 ) ;

      if ( entityType_0 != ENTITY_BODY ) {
	continue ;
      }
            
      int8_t entityType_1, owner_1, x_1, y_1, param1_1, param2_1 ;

      for ( int ind_entity_1 = 0 ; ind_entity_1 < wk_nb_entities ; ++ind_entity_1 ) {

	get_entity( wk_v_entities, ind_entity_1,
		    entityType_1, owner_1, x_1, y_1, param1_1, param2_1 ) ;

	// if ( verbose ) {

	//   if ( ( entityType_0 == ENTITY_BODY ) &&
	//        ( onwer_0 == 0 ) &&
	//        ( x_0 == 2 ) &&
	//        ( y_0 == 5 ) ) {
	//     cerr << "Against entity: " << endl ;
	//     print_entity( &wk_v_entities[ j * NB_PARAM_ENTITY ] ) ;

	//   }
	// }
	
	if ( ( entityType_1 == ENTITY_OBJECT ) &&	\
	     ( x_0 == x_1 ) && ( y_0 == y_1 ) ) {

	  entity_to_discard[ nb_entity_to_discard++ ] = ind_entity_1 ;
	  
	  if ( param1_1 == EXTRA_PORTE ) {
	    put_entity( wk_v_entities, ind_entity_0, 
			entityType_0, owner_0, x_0, y_0, param1_0, param2_0 + 1 ) ;

	    player_score[ owner_0 ] += AG_BONUS_EXTRA_PORTE ;

	  }

	  else if ( param1_1 == EXTRA_BOMB ) {
	    put_entity( wk_v_entities, ind_entity_0, 
			entityType_0, owner_0, x_0, y_0, param1_0 + 1, param2_0 ) ;

	    player_score[ owner_0 ] += AG_BONUS_EXTRA_BOMB ;

	  }

	}
	
      }

    } // END - for ( int ind_entity = 0 ; ind_entity < wk_nb_entities ; ++ind_entity ) {

    if ( player_score[ myId ] != 0 ) {
      // GENE SCORE
      // gene.score += player_score[ myId ] + ( 1.0 / ( ind_gene + 1 ) );
      gene.score += player_score[ myId ] * factor ;
      // gene.score += player_score[ myId ] ;
    }

    for ( int i = 0 ; i < nb_entity_to_discard ; ++i ) {

      int ind_entity = entity_to_discard[ i ] ;
      discard_entity( wk_v_entities, ind_entity ) ;

    }
    
    // 5 *** END - UPDATE THE ENTITIES IN CASE AN OBJECT WAS TAKEN

    if ( verbose ) { 
      cerr << "Grid end loop" << endl ;
      pretty_print_grid( wk_grid, wk_v_entities, wk_nb_entities, myId ) ;
    }
      
    // if ( verbose ) { 
    //   if ( 8 <= dbg_ctp )
    // 	pretty_print_grid( wk_grid, wk_v_entities, wk_nb_entities, myId ) ;
    // }

    // ++dbg_ctp ;
      
  } // END for ( int ind_gene = 0 ; ind_gene < AG_DEEPNESS ; ++ind_gene ) {
  // ***** END - LOOP OF SIMULATION FOR A GENE

}


bool gene_compare( gene_t const & g_0, gene_t const & g_1 ) {

  return g_0.score > g_1.score ;
  
}


int main( ) {

  init_tab_alea() ;
  cur_iter = 0 ;

  memset( genes, 0, sizeof( genes ) ) ;


  int width;
  int height;
  cin >> width >> height >> myId; cin.ignore();

  using time_point = chrono::time_point< chrono::system_clock > ;
  using duration   = chrono::duration< float > ;

  time_point start ;
  duration d ;

  // game loop
  while (1) {

    start = now() ;

    dbg_cpt = 0 ;
    ++cur_iter ;

    // P_DBG( tab_alea[ 0 ] ) ;
    // printf( stderr, "%d toto\n", tab_alea[ 0 ] ) ;

    // cerr << "Alea " << ( int ) tab_alea[ 0 ] << endl ;

    for ( int i = 0 ; i < HEIGHT ; ++i ) {
      string row;
      cin >> row; cin.ignore();
      // cerr << row << endl;

      for ( int j = 0 ; j < WIDTH ; ++j ) {
      	switch( row[ j ] ) {
      	case 'X' : 
      	  grid[ j ][ i ] = WALL_ID ;
      	  break;
      	case '0' : 
      	  grid[ j ][ i ] = BOX_0_ID ;
      	  break;
      	case '1' : 
      	  grid[ j ][ i ] = BOX_1_ID ;
      	  break;
      	case '2' : 
      	  grid[ j ][ i ] = BOX_2_ID ;
      	  break;
      	case '.' : 
      	  grid[ j ][ i ] = FLOOR_ID ;
      	  break ;
      	}

      }
    }

    int nb_entities;
    cin >> nb_entities; cin.ignore();

    int8_t v_entities[ nb_entities * NB_PARAM_ENTITY ] ;
    // for ( int i = 0 ; i < ( nb_entities * NB_PARAM_ENTITY ) ; ++i ) {
    //   v_entities[ i ] = 0 ;
    // }
    memset( v_entities, 0, ( nb_entities * NB_PARAM_ENTITY ) * sizeof( int8_t ) ) ;
    // memset( v_entities, 0, sizeof v_entities ) ;

    nb_player = 0 ;

    for ( int i = 0 ; i < nb_entities ; ++i ) {
      int entityType;
      int owner;
      int x;
      int y;
      int param1;
      int param2;    
      cin >> entityType >> owner >> x >> y >> param1 >> param2; cin.ignore();

      v_entities[ i * NB_PARAM_ENTITY + 0 ] = entityType;
      v_entities[ i * NB_PARAM_ENTITY + 1 ] = owner;
      v_entities[ i * NB_PARAM_ENTITY + 2 ] = x;
      v_entities[ i * NB_PARAM_ENTITY + 3 ] = y;
      v_entities[ i * NB_PARAM_ENTITY + 4 ] = param1;
      v_entities[ i * NB_PARAM_ENTITY + 5 ] = param2;

      if ( entityType == ENTITY_BODY ) {
	++nb_player ;
      }

      // if ( entityType == ENTITY_BODY ) {
      // 	if ( owner == myId ) {
      // 	  grid[ x ][ y ] = ME_ID ;
      // 	} else {
      // 	  grid[ x ][ y ] = ENNEMY_ID ;
      // 	}
      // }

      // else if ( entityType == ENTITY_BOMB ) {

      // 	grid[ x ][ y ] = BOMB_ID ;

      // } else if ( entityType == ENTITY_OBJECT ) {

      // 	if ( param1 == 1 ) {
      // 	  grid[ x ][ y ] = OBJECT_1_ID ;
      // 	} else {
      // 	  grid[ x ][ y ] = OBJECT_2_ID ;
      // 	}
      // }

    }

    nb_boxes = get_nb_boxes( ) ;
    
    cerr << "cur_iter: " << cur_iter << endl;
    cerr << "myId: " << myId << endl;

    // pretty_print_grid( grid, v_entities, nb_entities, myId ) ;

    // print_entities( v_entities, nb_entities ) ;

  // for ( int i = 0 ; i < nb_entities ; ++i ) {
  //     for ( int j = 0 ; j < NB_PARAM_ENTITY ; ++j ) {
  // 	fprintf( stderr, "%2d, ", v_entities[ i * NB_PARAM_ENTITY + j ] ) ;
  //     }
  //     cerr << endl ;
  //   }
  //   cerr << endl ;

    // gene_t gene ;

    // int8_t gene[ AG_DEEPNESS ];

    for ( int i = 0 ; i < AG_POPULATION ; ++i ) {

      if ( 10 < genes[ i ].score ) {
	
	for ( int j = 0 ; j < ( AG_DEEPNESS - 1 ) ; ++j ) {
	  genes[ i ].data[ j ] = genes[ i ].data[ j + 1 ] ;
	}
	genes[ i ].data[ AG_DEEPNESS - 1 ] = get_alea( ) ;

      }
      
      else { 

	for ( int j = 0 ; j < AG_DEEPNESS ; ++j ) {
	  genes[ i ].data[ j ] = get_alea( ) ;
	}

      }

      genes[ i ].score = 0 ;
      
    }

    // Allocate every temporary array only once
    // wk_v_entities = ( int8_t * ) malloc( sizeof( int8_t )  * ( nb_entities * NB_PARAM_ENTITY * 2 + 1 ) ) ;
    // bomb_to_explose = ( int8_t * ) malloc( sizeof( int8_t ) * ( 2 * nb_entities + 1 ) ) ;
    
    wk_v_entities = ( int8_t * ) malloc( sizeof( int8_t ) * 
    					 ( nb_entities * NB_PARAM_ENTITY * 2 + 1 ) * 
    					 ( 2 * nb_entities + 1 ) * 
    					 ( nb_boxes * 2 + 2 ) * 
    					 ( nb_entities + 2 ) ) ;

    bomb_to_explose    = &wk_v_entities[ nb_entities * NB_PARAM_ENTITY * 2 + 1 ] ;
    boxes_to_disappear = &wk_v_entities[ ( nb_entities * NB_PARAM_ENTITY * 2 + 1 ) *
					 ( 2 * nb_entities + 1 ) ] ;
    entity_to_discard  = &wk_v_entities[ ( nb_entities * NB_PARAM_ENTITY * 2 + 1 ) *
					 ( 2 * nb_entities + 1 ) * 
					 ( nb_boxes * 2 + 2 ) ] ;

    if ( nb_boxes == 0 ) {
      for ( int i = 0 ; i < ( AG_POPULATION * 0.2 ) ; ++i ) {

	++dbg_cpt ;

	simulation( v_entities, nb_entities, genes[ i ], false );
      }
      sort( genes, genes + ( int ) ( AG_POPULATION * 0.2 ), gene_compare );

    }

    else {
      for ( int i = 0 ; i < AG_POPULATION ; ++i ) {

	++dbg_cpt ;

	simulation( v_entities, nb_entities, genes[ i ], false );
      }
      sort( genes, genes + AG_POPULATION, gene_compare );
    
    }

    // Deallocation every temporary array
    // free( bomb_to_explose ) ;
    // free( wk_v_entities ) ;

    free( wk_v_entities ) ;

    // cerr << "genes[ " << 0 << " ].score: " << genes[ 0 ].score << endl ;

    for ( int i = 0 ; i < 1 ; ++i ) {
      cerr << "genes[ " << i << " ].score: " << genes[ i ].score << endl ;

      cerr << "actions: " << endl ;
      for ( int j = 0 ; j < AG_DEEPNESS ; ++j ) {
	cerr << action_to_str( genes[ i ].data[ j ] ) << " | ";
	if ( j == 5 ) {
	  cerr << endl ;
	}
      }
      cerr << endl;
      
    }


    // DBG
    // if ( ( 78 <= cur_iter ) && ( cur_iter <= 79 ) ) {
    //   genes[ 0 ].score = 0 ;
    //   simulation( v_entities, nb_entities, genes[ 0 ], true );
    // }

    d = now() - start ;

    cerr << "Total time: " << d.count( ) * 1000 << " ms" << endl ;
    
    int8_t entityType, owner, x, y, param1, param2 ;

    get_my_entity( v_entities, nb_entities, myId, 
		   entityType, owner, x, y, param1, param2 ) ;

    switch( genes[ 0 ].data[ 0 ] ) {
    case ( AG_STAY ) :
      cout << "MOVE " << ( int ) x << " " << ( int ) y << endl ;
      break;
    case ( AG_NORTH ) :
      cout << "MOVE " << ( int ) x << " " << ( int ) ( y - 1 ) << endl ;
      break;
    case ( AG_SOUTH ) :
      cout << "MOVE " << ( int ) x << " " << ( int ) ( y + 1 ) << endl ;
      break;
    case ( AG_EAST ) :
      cout << "MOVE " << ( int ) ( x + 1 ) << " " << ( int ) y << endl ;
      break;
    case ( AG_WEST ) :
      cout << "MOVE " << ( int ) ( x - 1 ) << " " << ( int ) y << endl ;
      break;
    case ( AG_BOMB ) :
      cout << "BOMB " << ( int ) x << " " << ( int ) y << endl ;
      break;
    }
    
  }  // end while (1) {


}
