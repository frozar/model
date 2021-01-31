#pragma GCC optimize "-O3,omit-frame-pointer,inline"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// memset, memcpy function
#include <cstring>

// printf, fprintf
#include <cstdio>

// strcpy
#include <cstring>

#define FACTORY_ID 1
#define TROOP_ID   2
#define BOMB_ID    3

#define NB_MAX_FACTORY 15

using namespace std;

int factoryCount ; // the number of factories
int linkCount ;    // the number of links between factories

int entityCount; // the number of entities (e.g. factories and troops)

int my_factories[ NB_MAX_FACTORY ] ;
int opponent_factories[ NB_MAX_FACTORY ] ;
int neutral_factories[ NB_MAX_FACTORY ] ;

int * dist_factories ;

inline int semi_square( int n ) {
  return n * ( n + 1 ) / 2 ;
}

inline void set_dist_factories( int * dist_factories, 
				int i, int j, int dist ) {
  int n_1 = factoryCount - 1 ;
  int n_2 = n_1 - i ;
  int ind = semi_square( n_1 ) - semi_square( n_2 ) + j - ( i + 1 ) ;
  dist_factories[ ind ] = dist ;
}

inline int get_dist_factories( int * dist_factories, 
			       int i, int j ) {
  int n_1 = factoryCount - 1 ;
  int n_2 = n_1 - i ;
  int ind = semi_square( n_1 ) - semi_square( n_2 ) + j - ( i + 1 ) ;
  return dist_factories[ ind ] ;
}

typedef struct {
  int id ;
  int type ;
  int arg1 ;
  int arg2 ;
  int arg3 ;
  int arg4 ;
  int arg5 ;
} entity_t ;

entity_t * l_entities ;

void print_entity( entity_t e ) {
  char entity_type[ 10 ] ;
  memset( entity_type, 0, 10 * sizeof( char ) ) ;

  if ( e.type == FACTORY_ID ) {
    strcpy( entity_type, "FACTORY" ) ;
  }
  else if ( e.type == TROOP_ID ) {
    strcpy( entity_type, "TROOP" ) ;
  }
  else if ( e.type == BOMB_ID ) {
    strcpy( entity_type, "BOMB" ) ;
  }
  
  fprintf( stderr, "%2d : %s %2d %2d %2d %2d %2d\n",
	   e.id, entity_type,
	   e.arg1, e.arg2, e.arg3, e.arg4, e.arg5 ) ;
}

void read_input( ) {
  cin >> entityCount; cin.ignore();

  l_entities = ( entity_t * )malloc( sizeof( entity_t ) * entityCount ) ;
    
  // int my_factories[ factoryCount ] ;
  // int opponent_factories[ factoryCount ] ;

  // for ( int i = 0 ; i < factoryCount ; ++i ) {
  //   my_factories[ i ]    = -1 ;
  //   opponent_factories[ i ] = -1 ;
  // }
  // cerr << endl ;

  int idx_l_entities    = 0 ;
  for ( int i = 0 ; i < entityCount ; ++i ) {
    int entityId;
    string entityType;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    cin >> entityId >> entityType >> arg1 >> arg2 >> arg3 >> arg4 >> arg5; cin.ignore();
    // cerr << entityId <<
    //   " " << entityType <<
    //   " " << arg1 <<
    //   " " << arg2 <<
    //   " " << arg3 <<
    //   " " << arg4 <<
    //   " " << arg5 << endl ;

    int entity_type ;
    if ( entityType.compare( "FACTORY" ) == 0 ) {
      entity_type = FACTORY_ID ;
    }
    else {
      entity_type = TROOP_ID ;
    }

    l_entities[ idx_l_entities++ ] = { entityId, entity_type,
				       arg1, arg2, arg3, arg4, arg5 } ;

  }  // END - for ( int i = 0 ; i < entityCount ; ++i ) {
}


void print_dist_to_other_factory( int id_factory_src ) {

  for ( int i = 0 ; i < factoryCount ; ++i ) {
    if ( l_entities[ i ].type != FACTORY_ID ) {
      continue ;
    }
    if ( l_entities[ i ].id == id_factory_src ) {
      continue ;
    }
    if ( l_entities[ i ].arg1 == 1 ) {
      continue ;
    }

    int ii = id_factory_src ;
    int jj = l_entities[ i ].id ;
    if ( jj < ii ) {
      int tmp = jj ;
      jj = ii ;
      ii = tmp ;
    }

    // cerr << "i: " << id_factory_src <<
    //   " j: " << l_entities[ i ].id <<
    //   " own: " << l_entities[ i ].arg1 <<
    //   " dist: " << get_dist_factories( dist_factories, ii, jj ) << 
    //   " prod: " << l_entities[ i ].arg3 << 
    //   " cybo: " << l_entities[ i ].arg2 <<
    //   endl ;
    
    fprintf( stderr, "( %2d, %2d ) own: %2d dist: %2d prod: %2d cybo: %2d \n", 
	     id_factory_src, l_entities[ i ].id, l_entities[ i ].arg1, 
	     get_dist_factories( dist_factories, ii, jj ), l_entities[ i ].arg3, 
	     l_entities[ i ].arg2 ) ;

  }

}

void get_other_factories( int ** other_factories, int & nb_other_factories ) {

  nb_other_factories = 0 ;
  for ( int i = 0 ; i < entityCount ; ++i ) {
    if ( l_entities[ i ].type != FACTORY_ID ) {
      continue ;
    }
    if ( l_entities[ i ].arg1 == 1 ) {
      continue ;
    }
    nb_other_factories++ ;
  }

  if ( nb_other_factories == 0 ) {
    *other_factories = NULL ;
    return ;
  }

  *other_factories = ( int * )malloc( nb_other_factories * sizeof( int ) ) ;

  int idx_other_factories = 0 ;
  for ( int i = 0 ; i < entityCount ; ++i ) {
    if ( l_entities[ i ].type != FACTORY_ID ) {
      continue ;
    }
    if ( l_entities[ i ].arg1 == 1 ) {
      continue ;
    }
    ( *other_factories )[ idx_other_factories++ ] = i ;
  }

  return ;
}


void print_tab_sub_disivion( float * tab_sub_division, int nb_sub_division ) {
  cerr << "tab_sub_division: " ;
  for ( int i = 0 ; i < nb_sub_division ; ++i ) {
    cerr << tab_sub_division[ i ] << " " ;
  }
  cerr << endl ;
}


// TODO: mettre une priorite par rapport au nombre de cyborg present dans les
//       factory neutres
//       mettre une priorite sur les factory neutre par rapport aux factory adverses
int get_dst_factories_2( int nb_neutral_factories,
			 int i, float alea, bool verbose = false ) {
  float sub_division ;
  float sum ;

  int dst_factory = -1 ;

  int * other_factories ;
  int nb_other_factories ;

  get_other_factories( &other_factories, nb_other_factories ) ;

  // cerr << "other_factories: " << other_factories << " " << ( other_factories == NULL ) << endl ;
  // cerr << "nb_other_factories: " << nb_other_factories << endl ;
  
  if ( other_factories == NULL ) {
    return dst_factory ;
  }

  sub_division = 1.0 / nb_other_factories ;

  float tab_sub_division[ nb_other_factories ] ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] = sub_division ;
  }

  // print_tab_sub_disivion( tab_sub_division, nb_other_factories ) ;

  //  0 - OWNER PART

  float tot_owner_weight = 0 ;
  // Rescale accordingly to the owner (neutral/opponent)
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    
    float owner_weight ;
    int owner = l_entities[ other_factories[ j ] ].arg1 ;
    if ( owner == 0 ) {
      owner_weight = 1.0 ;
    }
    else {
      owner_weight = 0.1 ;
    }

    tot_owner_weight += owner_weight ;
    // cerr << "tot_owner_weight: " << tot_owner_weight << endl ;
  }

  float owner_proportion[ nb_other_factories ] ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    
    float owner_weight ;
    int owner = l_entities[ other_factories[ j ] ].arg1 ;
    if ( owner == 0 ) {
      owner_weight = 1.0 ;
    }
    else {
      owner_weight = 0.1 ;
    }

    owner_proportion[ j ] = owner_weight / tot_owner_weight ;
    // cerr << "owner_p: " << owner_proportion[ j ] << endl ;
  }

  // Initialisation of the sub division length for each neutral factories
  sum = 0 ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] *= owner_proportion[ j ] ;
    sum += tab_sub_division[ j ] ;
  }

  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] *= 1.0 / sum ;
  }

  // print_tab_sub_disivion( tab_sub_division, nb_other_factories ) ;

  //  1 - PRODUCTION PART

  float tot_prod = 0 ;
  // Rescale accordingly to the factory production parameter
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    float prod = l_entities[ other_factories[ j ] ].arg3 ;
    tot_prod += prod ;
  }

  // scale the sub_division taking account of the factory production
  float prod_proportion[ nb_other_factories ] ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    float prod = l_entities[ other_factories[ j ] ].arg3 ;
    prod_proportion[ j ] = prod / tot_prod ;
  }

  // DBG
  sum = 0 ;
  if ( verbose ) { cerr << "prod_proportion: " ; }
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] *= prod_proportion[ j ] ;
    if ( verbose ) { cerr << prod_proportion[ j ] << " " ; }
    sum += tab_sub_division[ j ] ;
  }
  if ( verbose ) { cerr << endl ; }

  if ( verbose ) { cerr << "sum: " << sum << endl ; }

  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] *= 1.0 / sum ;
    if ( verbose ) { cerr << tab_sub_division[ j ] << " " ; }
    sum += tab_sub_division[ j ] ;
  }
  if ( verbose ) { cerr << endl ; }
  if ( verbose ) { cerr << "sum: " << sum << endl ; }

  // print_tab_sub_disivion( tab_sub_division, nb_other_factories ) ;

  //  2 - DISTANCE PART

  float tot_inv_dist = 0 ;
  float dist_proportion[ nb_other_factories ] ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    int ii = my_factories[ i ] ;
    int jj = other_factories[ j ] ;
    if ( jj < ii ) {
      int tmp = jj ;
      jj = ii ;
      ii = tmp ;
    }
    float inv_dist = 1.0 / get_dist_factories( dist_factories, ii, jj ) ;
    tot_inv_dist += inv_dist ;
  }

  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    int ii = my_factories[ i ] ;
    int jj = other_factories[ j ] ;
    if ( jj < ii ) {
      int tmp = jj ;
      jj = ii ;
      ii = tmp ;
    }
    float inv_dist = 1.0 / get_dist_factories( dist_factories, ii, jj ) ;
    dist_proportion[ j ] = inv_dist / tot_inv_dist ;
  }

  // Rescale accordingly to the factory distance parameter
  sum = 0 ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] *= dist_proportion[ j ] ;
    sum += tab_sub_division[ j ] ;
  }

  float inv_sum = 1.0 / sum ;
  sum = 0 ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_sub_division[ j ] *= inv_sum ;
    if ( verbose ) { cerr << tab_sub_division[ j ] << " " ; }
    sum += tab_sub_division[ j ] ;
  }
  if ( verbose ) { cerr << endl ; }
  if ( verbose ) { cerr << "sum: " << sum << endl ; }

  // print_tab_sub_disivion( tab_sub_division, nb_other_factories ) ;

  //  3 - tab_segment PART
  
  float tab_segment[ nb_other_factories + 1 ] ;
  float offset = 0 ;
  tab_segment[ 0 ] = 0 ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    tab_segment[ j + 1 ] = offset + tab_sub_division[ j ] ;
    offset += tab_sub_division[ j ] ;
  }

  // DBG
  // verbose = true ;
  if ( verbose ) { cerr << "tab_segment: " ; }
  for ( int j = 0 ; j < nb_other_factories + 1 ; ++j ) {
    if ( verbose ) { cerr << tab_segment[ j ] << " " ; }
  }
  if ( verbose ) { cerr << endl ; }

  int idx_other_factory = 0 ;
  for ( int j = 0 ; j < nb_other_factories ; ++j ) {
    // if ( ( j * sub_division < alea ) && ( alea <=  ( j + 1 ) * sub_division) ) {
    if ( ( tab_segment[ j ]  < alea ) && ( alea <= tab_segment[ j + 1 ] )  ) {
      idx_other_factory = j ;
    }
  }

  dst_factory = other_factories[ idx_other_factory ] ;
  // cerr << "dst_factory: " << dst_factory << endl ;

  // cerr << "DBG pass 0 " << endl ;
  free( other_factories ) ;
  // cerr << "DBG pass 1 " << endl ;

  return dst_factory ;
}

// int get_dst_factories( int nb_neutral_factories, int nb_opponent_factories,
// 		       int i, float alea, bool verbose = false ) {

//   float sub_division ;
//   int dst_factory ;

//   // If there are still neutral factories
//   if ( 0 < nb_neutral_factories ) {
//     int tot_prod = 0 ;
//     float tot_inv_dist = 0 ;
	
//     // DBG
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       int ii = my_factories[ i ] ;
//       int jj = neutral_factories[ j ] ;
//       if ( jj < ii ) {
// 	int tmp = jj ;
// 	jj = ii ;
// 	ii = tmp ;
//       }

//       int prod = l_entities[ neutral_factories[ j ] ].arg3 ;

//       if ( verbose ) {
// 	cerr << "dist & prod: " <<
// 	  ii << " " <<
// 	  jj << " " <<
// 	  get_dist_factories( dist_factories, ii, jj ) << " " <<
// 	  prod << endl ;
//       }

//       tot_prod     += prod ;
//       tot_inv_dist += 1.0 / get_dist_factories( dist_factories, ii, jj ) ;
//     }

//     // scale the sub_division taking account of the factory production
//     float prod_proportion[ nb_neutral_factories ] ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       float prod = l_entities[ neutral_factories[ j ] ].arg3 ;
//       prod_proportion[ j ] = prod / tot_prod ;
//     }

//     // DBG
//     float sum = 0 ;
//     if ( verbose ) { cerr << "prod_proportion: " ; }
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       if ( verbose ) { cerr << prod_proportion[ j ] << " " ; }
//       sum += prod_proportion[ j ] ;
//     }
//     if ( verbose ) { cerr << endl ; }

//     if ( verbose ) { cerr << "sum: " << sum << endl ; }
	
//     sub_division = 1.0 / nb_neutral_factories ;
//     // Initialisation of the sub division length for each neutral factories
//     // Rescale accordingly to the factory production parameter
//     float tab_sub_division[ nb_neutral_factories ] ;
//     sum = 0 ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       tab_sub_division[ j ] = sub_division * prod_proportion[ j ] ;
//       if ( verbose ) { cerr << tab_sub_division[ j ] << " " ; }
//       sum += tab_sub_division[ j ] ;
//     }
//     if ( verbose ) { cerr << endl ; }
	
//     if ( verbose ) { cerr << "sum: " << sum << endl ; }

//     float inv_sum = 1.0 / sum ;
//     sum = 0 ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       tab_sub_division[ j ] *= inv_sum ;
//       if ( verbose ) { cerr << tab_sub_division[ j ] << " " ; }
//       sum += tab_sub_division[ j ] ;
//     }
//     if ( verbose ) { cerr << endl ; }
//     if ( verbose ) { cerr << "sum: " << sum << endl ; }
	
//     // DISTANCE PART
//     float dist_proportion[ nb_neutral_factories ] ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       int ii = my_factories[ i ] ;
//       int jj = neutral_factories[ j ] ;
//       if ( jj < ii ) {
// 	int tmp = jj ;
// 	jj = ii ;
// 	ii = tmp ;
//       }
//       float inv_dist = 1.0 / get_dist_factories( dist_factories, ii, jj ) ; 
//       dist_proportion[ j ] = inv_dist / tot_inv_dist ;
//     }

//     sum = 0 ;
//     if ( verbose ) { cerr << "dist_proportion: " ; }
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       if ( verbose ) { cerr << dist_proportion[ j ] << " " ; }
//       sum += dist_proportion[ j ] ;
//     }
//     if ( verbose ) { cerr << endl ; }
//     if ( verbose ) { cerr << "sum: " << sum << endl ; }

//     // Rescale accordingly to the factory distance parameter
//     sum = 0 ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       tab_sub_division[ j ] *= dist_proportion[ j ] ;
//       sum += tab_sub_division[ j ] ;
//     }

//     inv_sum = 1.0 / sum ;
//     sum = 0 ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       tab_sub_division[ j ] *= inv_sum ;
//       if ( verbose ) { cerr << tab_sub_division[ j ] << " " ; }
//       sum += tab_sub_division[ j ] ;
//     }
//     if ( verbose ) { cerr << endl ; }
//     if ( verbose ) { cerr << "sum: " << sum << endl ; }


//     float tab_segment[ nb_neutral_factories + 1 ] ;
//     float offset = 0 ;
//     tab_segment[ 0 ] = 0 ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       tab_segment[ j + 1 ] = offset + tab_sub_division[ j ] ;
//       offset += tab_sub_division[ j ] ;
//     }

//     // DBG
//     if ( verbose ) { cerr << "tab_segment: " ; }
//     for ( int j = 0 ; j < nb_neutral_factories + 1 ; ++j ) {
//       if ( verbose ) { cerr << tab_segment[ j ] << " " ; }
//     }
//     if ( verbose ) { cerr << endl ; }

//     int idx_neutral_factory = 0 ;
//     for ( int j = 0 ; j < nb_neutral_factories ; ++j ) {
//       // if ( ( j * sub_division < alea ) && ( alea <=  ( j + 1 ) * sub_division) ) {
//       if ( ( tab_segment[ j ]  < alea ) && ( alea <= tab_segment[ j + 1 ] )  ) {
// 	idx_neutral_factory = j ;
//       }
//     }

//     dst_factory = neutral_factories[ idx_neutral_factory ] ;

//   }

//   // If no neutral factories left
//   else {

//     int idx_other_factory = 0 ;
//     for ( int i = 0 ; i < nb_opponent_factories ; ++i ) {
//       if ( ( i * sub_division < alea ) && ( alea <=  ( i + 1 ) * sub_division) ) {
// 	idx_other_factory = i ;
//       }
//     }

//     // cerr << "idx_other_factory: " << idx_other_factory << endl ;
//     dst_factory = opponent_factories[ idx_other_factory ] ;

//   }

//   return dst_factory ;
// }


int main()
{
  cin >> factoryCount; cin.ignore();
  cin >> linkCount; cin.ignore();

  // nb link max
  int max_nb_link = ( factoryCount - 1 ) * factoryCount / 2 ;
  int dist_factories_trick[ max_nb_link ] ;
  dist_factories = dist_factories_trick ;

  for (int i = 0; i < linkCount; i++) {
    int factory1;
    int factory2;
    int distance;
    cin >> factory1 >> factory2 >> distance; cin.ignore();
    // DBG
    // cerr << factory1 << " " << factory2 << " " << distance << endl ;
    int ii = factory1 ;
    int jj = factory2 ;

    // ii is smaller than jj
    set_dist_factories( dist_factories, ii, jj, distance ) ;
  }

  // DBG
  // for ( int i = 0 ; i < factoryCount - 1 ; ++i ) {
  //   for ( int j = i + 1 ; j < factoryCount ; ++j ) {
  //     cerr << get_dist_factories( dist_factories, i, j ) << " " ;
  //   }
  //   cerr << endl ;
  // }


  // GAME LOOP
  while (1) {

    // int my_factories[ factoryCount ] ;
    // int other_factories[ factoryCount ] ;

    // Allocation of 'l_entities' inside read_input()
    read_input( ) ;

    int idx_my_factory      = 0 ;
    int idx_other_factory   = 0 ;
    int idx_neutral_factory = 0 ;
    
    for ( int i = 0 ; i < entityCount ; ++i ) {

      if ( l_entities[ i ].type == FACTORY_ID ) {
	if ( l_entities[ i ].arg1 == 1 ) {
	  my_factories[ idx_my_factory++ ] = l_entities[ i ].id ;
	}
	else if ( l_entities[ i ].arg1 == -1 ) {
	  opponent_factories[ idx_other_factory++ ] = l_entities[ i ].id ;
	}
	else {
	  neutral_factories[ idx_neutral_factory++ ] = l_entities[ i ].id ;
	}
      }
      
    }

    int nb_my_factories      = idx_my_factory ;
    int nb_opponent_factories   = idx_other_factory ;
    int nb_neutral_factories = idx_neutral_factory ;

    if ( ( nb_my_factories == 0 ) || ( nb_opponent_factories == 0 ) ) {
      cout << "WAIT" << endl ;
      continue ;
    }
	
    cerr << "idx_my_factory: " << idx_my_factory << endl ;
    cerr << "nb_my_factories: " << nb_my_factories << endl ;

    int order[ factoryCount * factoryCount * 3 ] ;
    for ( int i = 0 ; i < factoryCount * factoryCount * 3 ; ++i ) {
      order[ i ] = -1 ;
    }

    // cerr << "PASS BEFORE" << endl ;

    int order_offset = 0 ;
    for ( int i = 0 ; i < nb_my_factories ; ++i ) {
      cerr << "id_factory: " << my_factories[ i ] << endl ;
      int nb_my_cyborg = l_entities[ my_factories[ i ] ].arg2 ;
      cerr << "nb_my_cyborg: " << nb_my_cyborg << endl ;
      if ( nb_my_cyborg == 0 ) {
	continue ;
      }

      // cerr << "alea: " << alea << endl ;
      // cerr << "sub_division: " << sub_division << endl ;

      // int potential_dest_factories[ nb_opponent_factories + nb_neutral_factories ] ;
      int potential_dest_factories[ factoryCount ] ;

      memset( potential_dest_factories, 0, factoryCount * sizeof( int ) ) ;

      // TODO: compute the number of cyborg that can be send

      for ( int j = 0 ; j < nb_my_cyborg ; ++j ) {
	float alea = rand( ) / float( RAND_MAX ) ;
	
	// int dst_factory = get_dst_factories( nb_neutral_factories, nb_opponent_factories,
	// 				     i, alea ) ;
	int dst_factory = get_dst_factories_2( nb_neutral_factories,
					       i, alea ) ;
	// cerr << "JUST AFTER " << dst_factory << endl ;

	potential_dest_factories[ dst_factory ]++ ;
	// cerr << "OUTSIDE " << dst_factory << endl ;
      }

      print_dist_to_other_factory( my_factories[ i ] ) ;

      cerr << "0 potential_dest_factories: " << endl ;
      for ( int j = 0 ; j < factoryCount ; ++j ) {
	cerr << j << " : " << potential_dest_factories[ j ] << endl ;
      }

      // TODO: balance the number of cyborg to send to conquir a factory
      int nb_cyborg_to_dispatch = 0 ;
      for ( int j = 0 ; j < factoryCount ; ++j ) {
	if ( 0 < potential_dest_factories[ j ] ) {
	  if ( l_entities[ j ].arg2 < potential_dest_factories[ j ] ) {
	    int diff = potential_dest_factories[ j ] - ( l_entities[ j ].arg2 + 1 ) ;
	    nb_cyborg_to_dispatch += diff ;
	    potential_dest_factories[ j ] -= diff ;
	  }
	}
      }

      if ( 0 < nb_cyborg_to_dispatch ) {
	for ( int j = 0 ; j < factoryCount ; ++j ) {
	  if ( 0 < potential_dest_factories[ j ] ) {
	    if ( potential_dest_factories[ j ] < l_entities[ j ].arg2 + 1 ) {
	      int diff = ( l_entities[ j ].arg2 + 1 ) - potential_dest_factories[ j ] ;
	      if ( nb_cyborg_to_dispatch < diff ) {
		potential_dest_factories[ j ] += nb_cyborg_to_dispatch ;
		nb_cyborg_to_dispatch          = 0 ;
	      }
	      else {
		potential_dest_factories[ j ] += diff ;
		nb_cyborg_to_dispatch         -= diff ;
	      }
	    }
	  }
	}
      }

      cerr << "1 potential_dest_factories: " << endl ;
      for ( int j = 0 ; j < factoryCount ; ++j ) {
	cerr << j << " : " << potential_dest_factories[ j ] << endl ;
      }

      for ( int j = 0 ; j < factoryCount ; ++j ) {
	if ( 0 < potential_dest_factories[ j ] ) {
	  order[ order_offset * 3 + 0 ] = my_factories[ i ] ;
	  order[ order_offset * 3 + 1 ] = j ;
	  order[ order_offset * 3 + 2 ] = potential_dest_factories[ j ] ;
	  order_offset++ ;
	}
      }

    }  // END - for ( int i = 0 ; i < nb_my_factories ; ++i ) {

    int nb_order = order_offset ;
    cerr << "nb_order: " << nb_order << endl ;

    if ( 0 < nb_order ) {
      for ( int i = 0 ; i < nb_order - 1 ; ++i ) {
	cout << "MOVE" <<
	  " " << order[ i * 3 + 0 ] <<
	  " " << order[ i * 3 + 1 ] <<
	  " " << order[ i * 3 + 2 ] ;
	cout << " ;" ;
      }
      cout << "MOVE" <<
	" " << order[ ( nb_order - 1 ) * 3 + 0 ] <<
	" " << order[ ( nb_order - 1 ) * 3 + 1 ] <<
	" " << order[ ( nb_order - 1 ) * 3 + 2 ] ;
      cout << endl ;
    }
    else { 
      cout << "WAIT" << endl ; 
    }

    free( l_entities ) ;
  }

}
