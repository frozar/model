// #pragma GCC optimize "-O3,omit-frame-pointer,inline"
#pragma GCC optimize "-O0"
#include <iostream>
#include <string>
// #include <vector>
#include <algorithm>

// memset function
#include <cstring>

// chrono
#include <chrono>

using namespace std;

#define now chrono::system_clock::now

#define TYPE_WIZARD          0
#define TYPE_OPPONENT_WIZARD 1
#define TYPE_SNAFFLE         2
#define TYPE_BLUDGER         3

#define DEEPNESS 1

#define ACTION_MOVE  0
#define ACTION_THROW 1

#define FRICTION_WIZARD   0.75
#define FRICTION_BLUDGER  0.9
#define FRICTION_SNAFFLE  0.75

#define TIME_LIMIT_MC_FIRST 0.920
#define TIME_LIMIT_MC_WHILE 0.085

#define NB_ALEA_FLOAT    10 * 1024 * 1024

float tab_alea_float[ NB_ALEA_FLOAT ] = { 0 } ;

void init_tab_alea_float( ) {
  for ( int i = 0 ; i < NB_ALEA_FLOAT ; ++i ) {
    tab_alea_float[ i ] = rand( ) / float( RAND_MAX ) ;
  }
}

int ind_alea_float = 0 ;

float get_rand( ) {
  float res = tab_alea_float[ ind_alea_float ] ;
  ++ind_alea_float ;
  
  if ( ind_alea_float == NB_ALEA_FLOAT ) 
    ind_alea_float = 0 ;

  return res ;  
}


typedef struct entity_t {
  int entity_id ;
  int entity_type ;
  float x ;
  float y ;
  float vx ;
  float vy ;
  int radius ;
  float mass ;
  int state ;
} entity_t ;

typedef struct position_t {
  int x ;
  int y ;
} position_t ;

// if 0 you need to score on the right of the map, if 1 you need to score on the left
int myTeamId ;
position_t my_goal_pos ;
position_t his_goal_pos ;

int nb_entities ;
int nb_snaffle ;

float eps = 1e-10 ;
float eps_time = 1e-4 ;

entity_t l_entity[ 12 + 13 ] ;
entity_t l_snaffle[ 7 ] ;


void compute_goal_pos ( ) {

  if ( myTeamId == 0 ) {
    my_goal_pos.x  = 0 ;
    my_goal_pos.y  = 3750 ;
    his_goal_pos.x = 16000 ;
    his_goal_pos.y = 3750 ;
  }
  else {
    my_goal_pos.x  = 16000 ;
    my_goal_pos.y  = 3750 ;
    his_goal_pos.x = 0 ;
    his_goal_pos.y = 3750 ;
  }
  
}


void print_entity( entity_t * ent ) {

  cerr << "  " << ent->entity_id
       << " "  << ent->entity_type
       << " "  << ent->x
       << " "  << ent->y
       << " "  << ent->vx
       << " "  << ent->vy
       << " "  << ent->radius
       << " "  << ent->mass
       << " "  << ent->state
       << endl ;

}

void print_l_entity( entity_t l_entity[ 12 + 13 ] ) {

  cerr << "list entities:" << endl ;
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    // print_entity( l_entity[ i ] ) ;
    print_entity( &(l_entity[ i ]) ) ;
  }
  
}



void get_my_wizard( entity_t l_entity[ 12 + 13 ],
		    entity_t ** my_wizard_0,
		    entity_t ** my_wizard_1 ) {

  bool first = true ;
  for ( int i = 0 ; i < nb_entities ; ++i ) {

    if ( l_entity[ i ].entity_type == TYPE_WIZARD ) {
      if ( first ) {
	*my_wizard_0 = &(l_entity[ i ]) ;
	first = false ;
      }
      else {
	*my_wizard_1 = &(l_entity[ i ]) ;
	break ;
      }
    }
    
  }

  // cerr << "0: my_wizard_0->entity_id: " << (*my_wizard_0)->entity_id << endl ;

}


void get_bludger( entity_t l_entity[ 12 + 13 ],
		  entity_t ** bludger_0,
		  entity_t ** bludger_1 ) {

  bool first = true ;
  for ( int i = 0 ; i < nb_entities ; ++i ) {

    if ( l_entity[ i ].entity_type == TYPE_BLUDGER ) {
      if ( first ) {
	*bludger_0 = &(l_entity[ i ]) ;
	first = false ;
      }
      else {
	*bludger_1 = &(l_entity[ i ]) ;
	break ;
      }
    }
    
  }

  // cerr << "0: my_wizard_0->entity_id: " << (*my_wizard_0)->entity_id << endl ;

}


void read_input( ) {

  nb_snaffle = 0 ;
  
  cin >> nb_entities; cin.ignore();
  for ( int i = 0 ; i < nb_entities ; i++ ) {
    // entity identifier
    int entityId ;
    // "WIZARD", "OPPONENT_WIZARD", "SNAFFLE" or "BLUDGER"
    string entityType ;
    // position ( x, y )
    int x ;
    int y ;
    // velocity ( vx, vy )
    int vx ;
    int vy ;
    int radius ;
    float mass ;
    // 1 if the wizard is holding a Snaffle, 0 otherwise
    int state ;

    cin >> entityId >> entityType >> x >> y >> vx >> vy >> state; cin.ignore();

    int i_entityType = 0 ;

    if ( !entityType.compare( "WIZARD" ) ) {
      i_entityType = TYPE_WIZARD ;
      radius       = 400.0 ;
      mass         = 1.0 ;
    }
    else if ( !entityType.compare( "OPPONENT_WIZARD" ) ) {
      i_entityType = TYPE_OPPONENT_WIZARD ;
      radius       = 400.0 ;
      mass         = 1.0 ;
    }
    else if ( !entityType.compare( "SNAFFLE" ) ) {
      i_entityType = TYPE_SNAFFLE ;
      radius       = 150.0 ;
      mass         = 0.5 ;
      ++nb_snaffle ;
    }
    else if ( !entityType.compare( "BLUDGER" ) ) {
      i_entityType = TYPE_BLUDGER ;
      radius       = 200.0 ;
      mass         = 8.0 ;
    }

    l_entity[ i ] = { entityId, i_entityType, x, y, vx, vy, radius, mass, state } ;

    if ( i_entityType == TYPE_SNAFFLE ) {
      l_snaffle[ nb_snaffle - 1 ] = { entityId, i_entityType, x, y, vx, vy, radius, mass, state } ;
    }
  }

  cerr << "IN read_input" << endl ;

}


void nearest_snaffle( entity_t l_snaffle[ 7 ],
		      const entity_t & my_wizard_0, const entity_t & my_wizard_1,
		      entity_t & snaffle_0, entity_t & snaffle_1 ) {

  int dist_min = 16000 * 16000 + 7500 * 7500 ;
  int wk_dist ;

  entity_t cur_snaffle ;

  // snaffle nearest to my_wizard_0
  for ( int i = 0 ; i < nb_snaffle ; ++i ) {

    cur_snaffle = l_snaffle[ i ] ;
    
    wk_dist = ( my_wizard_0.x - cur_snaffle.x ) * ( my_wizard_0.x - cur_snaffle.x ) +
              ( my_wizard_0.y - cur_snaffle.y ) * ( my_wizard_0.y - cur_snaffle.y ) ;

    if ( wk_dist < dist_min ) {
      dist_min  = wk_dist ;
      snaffle_0 = cur_snaffle ;
    }
    
  }


  dist_min = 16000 * 16000 + 7500 * 7500 ;
  
  // snaffle nearest to my_wizard_1
  for ( int i = 0 ; i < nb_snaffle ; ++i ) {

    cur_snaffle = l_snaffle[ i ] ;
    
    wk_dist = ( my_wizard_1.x - cur_snaffle.x ) * ( my_wizard_1.x - cur_snaffle.x ) +
              ( my_wizard_1.y - cur_snaffle.y ) * ( my_wizard_1.y - cur_snaffle.y ) ;

    if ( ( wk_dist < dist_min ) && ( cur_snaffle.entity_id != snaffle_0.entity_id ) ) {
      dist_min  = wk_dist ;
      snaffle_1 = cur_snaffle ;
    }
    
  }

}


int round_half_away( float d ) {
  return ( int ) ( d > 0 ? d + 0.5 : d - 0.5 ) ;
}


typedef struct action_t {
  int act ;
  int delta_x ;
  int delta_y ;
  int thrust ;
} action_t ;


typedef struct gene_t {
  action_t action[ DEEPNESS ] ;
  float    score ;
} gene_t ;


void print_gene( const gene_t & gene ) {

  for ( int i = 0 ; i < DEEPNESS ; ++i ) {
    if ( gene.action[ i ].act == ACTION_MOVE ) {
      cerr << "MOVE" ;
    }
    else {
      cerr << "THROW" ;
    }
    cerr << " " << gene.action[ i ].delta_x
	 << " " << gene.action[ i ].delta_y
	 << " " << gene.action[ i ].thrust
	 << endl ;
  }
  cerr << "score: " << gene.score << endl ;

}


void update_velocity_begin( const action_t & cur_act,
			    // entity_t * wizard,
			    entity_t & wizard,
			    // float & real_vx, float & real_vy,
			    bool verbose = false ) {

  if ( verbose ) {
    cerr << "IN MOVE ACTION" << endl ;
  }

  // cerr << "SIMU: IN MOVE" << endl ;
      
  // float real_vx ;
  // float real_vy ;

  float thrust_vec_x = cur_act.delta_x ;
  float thrust_vec_y = cur_act.delta_y ;

  if ( verbose ) {
    cerr << "cur_act.delta_x: " << cur_act.delta_x << endl ;
    cerr << "cur_act.delta_y: " << cur_act.delta_y << endl ;
  }
  
  // Normalisaton of the move vector
  float norm_thrust_vec = sqrt( thrust_vec_x * thrust_vec_x + thrust_vec_y * thrust_vec_y ) ;
  // thrust_vec_x /= norm_thrust_vec ;
  // thrust_vec_y /= norm_thrust_vec ;

  // Rescaling of the move vector
  // thrust_vec_x = cur_act.thrust * thrust_vec_x / norm_thrust_vec ;
  // thrust_vec_y = cur_act.thrust * thrust_vec_y / norm_thrust_vec ;

  float d_vx = cur_act.thrust * thrust_vec_x / ( norm_thrust_vec * wizard.mass ) ;
  float d_vy = cur_act.thrust * thrust_vec_y / ( norm_thrust_vec * wizard.mass ) ;

  if ( verbose ) {
  cerr << "thrust_vec_x: " << thrust_vec_x << endl ;
  cerr << "thrust_vec_y: " << thrust_vec_y << endl ;

  // cerr << "wizard->vx: " << wizard->vx << endl ;
  // cerr << "wizard->vy: " << wizard->vy << endl ;

  // cerr << "wizard->mass: " << wizard->mass << endl ;
  }
  
  // Add to the current velocity vector
  // real_vx = wizard->vx + thrust_vec_x / wizard->mass ;
  // real_vy = wizard->vy + thrust_vec_y / wizard->mass ;
  // wizard->vx += thrust_vec_x / wizard->mass ;
  // wizard->vy += thrust_vec_y / wizard->mass ;
  wizard.vx += d_vx ;
  wizard.vy += d_vy ;

  if ( verbose ) {
    // cerr << "real_vx: " << real_vx << endl ;
    // cerr << "real_vy: " << real_vy << endl ;
    // cerr << "real_vx: " << wizard->vx << endl ;
    // cerr << "real_vy: " << wizard->vy << endl ;
  }
  
  // // Update position without forgetting the rounding stuff
  // wizard->x += round_half_away( real_vx ) ;
  // wizard->y += round_half_away( real_vy ) ;

  // cerr << "round_half_away( real_vx ): " << round_half_away( real_vx ) << endl ;
  // cerr << "round_half_away( real_vy ): " << round_half_away( real_vy ) << endl ;

}


void apply_friction( entity_t & ent ) {
  if ( ent.entity_type == TYPE_WIZARD ) {
    ent.vx *= FRICTION_WIZARD ;
    ent.vy *= FRICTION_WIZARD ;
  }
  else if ( ent.entity_type == TYPE_OPPONENT_WIZARD ) {
    ent.vx *= FRICTION_WIZARD ;
    ent.vy *= FRICTION_WIZARD ;
  }
  else if ( ent.entity_type == TYPE_SNAFFLE ) {
    ent.vx *= FRICTION_SNAFFLE ;
    ent.vy *= FRICTION_SNAFFLE ;
  }
  else if ( ent.entity_type == TYPE_BLUDGER ) {
    ent.vx *= FRICTION_BLUDGER ;
    ent.vy *= FRICTION_BLUDGER ;
  }
}


float check_collision( entity_t & ent_0, entity_t & ent_1, bool verbose = false ) {

  // print_entity( &ent_0 ) ;
  // print_entity( &ent_1 ) ;

  // If the length of the relative move is shorter than the distance between
  // the two sphere minus the sum of radius,
  // escape
  // Consider the entity as static
  float rel_vx = ent_0.vx - ent_1.vx ;
  float rel_vy = ent_0.vy - ent_1.vy ;

  if ( ( rel_vx < eps ) && ( rel_vy < eps ) ) {
    return 1 ;
  }

  // cerr << "rel_vx: " << rel_vx << endl ;
  // cerr << "rel_vy: " << rel_vy << endl ;

  // Coordinate of ent_0 in a markland where ent_1 is the origin of the landmark
  float x_0 = ent_0.x - ent_1.x ;
  float y_0 = ent_0.y - ent_1.y ;

  float t_closest = -( rel_vx * x_0 + rel_vy * y_0 ) / ( rel_vx * rel_vx + rel_vy * rel_vy ) ;

  // if ( verbose ) {
  //   cerr << "t_closest: " << t_closest << endl ;
  // }

  float x_closest = x_0 + rel_vx * t_closest ;
  float y_closest = y_0 + rel_vy * t_closest ;

  // cerr << "X_0: " << x_0 << " " << y_0 << endl ;
  // cerr << "x_closest: " << x_closest << " " << y_closest << endl ;

  float sum_squared_radius = ( ent_0.radius + ent_1.radius ) *
    ( ent_0.radius + ent_1.radius ) ;
    
  // float length_rel_v ;

  // If the closest point is too far away for the antogoniste entity,
  // escape
  if ( sum_squared_radius < ( x_closest * x_closest + y_closest * y_closest ) ) {
    return 1 ;
  }

  // if ( verbose ) {
  //   cerr << "  !! MAYBE COLLISION !!" << endl ;
  // }

  // cerr << "rel_v : ( " << rel_vx << ", " << rel_vy << " )" << endl ;

  float d_after_collision = sqrt( sum_squared_radius -
				  ( x_closest * x_closest + y_closest * y_closest ) ) ;

  // if ( verbose ) {
  //   cerr << "d_after_collision: " << d_after_collision << endl ;
  //   cerr << "d_tot: " << sqrt( ( x_closest - x_0 ) * ( x_closest - x_0 ) +
  // 			       ( y_closest - y_0 ) * ( y_closest - y_0 ) ) << endl; 
  // }

  float len_rel_v = sqrt( rel_vx * rel_vx + rel_vy * rel_vy ) ;
    
  float delta_t_before_collision = ( sqrt( ( x_closest - x_0 ) * ( x_closest - x_0 ) +
					   ( y_closest - y_0 ) * ( y_closest - y_0 ) ) - 
				     d_after_collision ) / len_rel_v ;

  if ( verbose ) {
    cerr << "delta_t_before_collision: " << delta_t_before_collision << endl ;
    // if ( delta_t_before_collision < 0 ) {
    //   print_entity( &ent_0 ) ;
    //   print_entity( &ent_1 ) ;
    // }
    if ( ( 0 < delta_t_before_collision ) && ( delta_t_before_collision < 1 ) ) {
      print_entity( &ent_0 ) ;
      print_entity( &ent_1 ) ;
    }
  }

  // If the collision time is to far away,
  // escape
  if ( ( delta_t_before_collision < eps_time ) || ( 1 < delta_t_before_collision ) ) {
    return 1 ;
  }

  return delta_t_before_collision ;

}


void update_velocity_after_choc( entity_t & ent_0, entity_t & ent_1,
				 bool verbose = false ) {

  float rel_vx = ent_0.vx - ent_1.vx ;
  float rel_vy = ent_0.vy - ent_1.vy ;

  float imp_x = -2 * ent_0.mass * ent_1.mass * rel_vx / ( ent_0.mass + ent_1.mass ) ;
  float imp_y = -2 * ent_0.mass * ent_1.mass * rel_vy / ( ent_0.mass + ent_1.mass ) ;

  // cerr << "imp: " << imp_x << " " << imp_y << endl ;
    
  float len_imp = sqrt( imp_x * imp_x + imp_y * imp_y ) ;
  // cerr << "len_imp: " << len_imp << endl ;
  // Normalize the impulsion to a norm of 100 at least
  if ( len_imp < 100 ) {
    cerr << "Rescale impulsion" << endl ;
    imp_x = imp_x / len_imp * 100 ;
    imp_y = imp_y / len_imp * 100 ;
  }


  // Update the speed after the collision
  ent_0.vx = ent_0.vx + imp_x / ent_0.mass ;
  ent_0.vy = ent_0.vy + imp_y / ent_0.mass ;

  ent_1.vx = ent_1.vx - imp_x / ent_1.mass ;
  ent_1.vy = ent_1.vy - imp_y / ent_1.mass ;

  if ( verbose ) {
    cerr << "IN " << __FUNCTION__ << endl ;
    print_entity( &ent_0 ) ;
    print_entity( &ent_1 ) ;
  }

}


void do_move( entity_t * l_entity, float delta_t, bool verbose = false ) {
  
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    entity_t & ent = l_entity[ i ] ;
    ent.x += ent.vx * delta_t ;
    ent.y += ent.vy * delta_t ;
  }

  if ( verbose ) {
    cerr << "IN " << __FUNCTION__ << endl ;
    print_l_entity( l_entity ) ;
  }
  
}


void get_closest_wizard( entity_t * l_entity, const entity_t & ent_ref, 
			 entity_t & closest_entity ) {

  float dist2_min = 16000 * 16000 ;

  float cur_dist2 ;
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    entity_t & cur_ent = l_entity[ i ] ;
    if ( ( cur_ent.entity_type == TYPE_SNAFFLE ) ||
	 ( cur_ent.entity_type == TYPE_BLUDGER ) ) {
      continue ;
    }

    cur_dist2 = ( cur_ent.x - ent_ref.x ) * ( cur_ent.x - ent_ref.x ) +
                ( cur_ent.y - ent_ref.y ) * ( cur_ent.y - ent_ref.y ) ;
    if ( cur_dist2 < dist2_min ) {
      dist2_min      = cur_dist2 ;
      closest_entity = cur_ent ;
    }
  }

}


int play( const action_t & cur_act_0, const action_t & cur_act_1,
	  entity_t * l_entity, bool verbose = false ) {

  entity_t & wizard_0 = l_entity[ 0 ] ;
  entity_t & wizard_1 = l_entity[ 1 ] ;

  // ### 0: TODO: deal with this action
  if ( cur_act_0.act == ACTION_THROW ) {

    // if ( wizard_0->state == 0 ) {
    if ( wizard_0.state == 0 ) {
      return -1 ;
    }
    
  }

  if ( cur_act_1.act == ACTION_THROW ) {

    // if ( wizard_1->state == 0 ) {
    if ( wizard_1.state == 0 ) {
      return -2 ;
    }
    
  }

  // ### 1: Update velocity for every entity ( wizard and bludger )
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    entity_t & cur_ent = l_entity[ i ] ;

    if ( cur_ent.entity_id == 0 ) {
      if ( cur_act_0.act == ACTION_MOVE ) {
	update_velocity_begin( cur_act_0, cur_ent ) ;
      }
    }

    else if ( cur_ent.entity_id == 1 ) {
      if ( cur_act_1.act == ACTION_MOVE ) {
	update_velocity_begin( cur_act_1, cur_ent ) ;
      }
    }

    else if ( cur_ent.entity_type == TYPE_BLUDGER ) {
      entity_t closest_wizard ;
      get_closest_wizard( l_entity, cur_ent, closest_wizard ) ;

      float thrust_vec_x    = closest_wizard.x - cur_ent.x ;
      float thrust_vec_y    = closest_wizard.y - cur_ent.y ;
      float norm_thrust_vec = sqrt( thrust_vec_x * thrust_vec_x + thrust_vec_y * thrust_vec_y ) ;
      thrust_vec_x *= 1000 / norm_thrust_vec ;
      thrust_vec_y *= 1000 / norm_thrust_vec ;

      cur_ent.vx += thrust_vec_x / cur_ent.mass ;
      cur_ent.vy += thrust_vec_y / cur_ent.mass ;

      // cerr << "Bludger:        " ;
      // print_entity( &cur_ent ) ;
      // cerr << "closest wizard: " ;
      // print_entity( &closest_wizard ) ;
    }

  }

  cerr << endl ;

  // ### 2: If there is any collision between wizard_0 and the other entities
  float tot_dt = 0 ;
  while ( eps < fabs( tot_dt - 1 ) ) {

    if ( verbose ) {
      cerr << "  !! BEGIN delta_t search" << endl ;
    }
    
    float delta_t_min = 1 ;
    int idx_ent_col_0 = -1 ;
    int idx_ent_col_1 = -1 ;
    float wk_delta_t ;
    for ( int i = 0 ; i < nb_entities - 1 ; ++i ) {
      entity_t & ent_0 = l_entity[ i ] ;
      for ( int j = i + 1 ; j < nb_entities ; ++j ) {
	entity_t & ent_1 = l_entity[ j ] ;
	
	wk_delta_t = check_collision( ent_0, ent_1 ) ;
	// wk_delta_t = check_collision( ent_0, ent_1, true ) ;

	if ( wk_delta_t < delta_t_min ) {
	  delta_t_min = wk_delta_t ;
	  idx_ent_col_0 = i ;
	  idx_ent_col_1 = j ;
	  // if ( verbose ) {
	    cerr << "  0 UPDATE delta_t_min" << endl ;
	    cerr << "  int i: " << i << " j: " << j << endl ;
	    cerr << "delta_t_min: " << delta_t_min << endl ;
	    print_entity( &ent_0 );
	    print_entity( &ent_1 );
	  // }
	}

      }
    }

    if ( verbose ) {
      cerr << "  !! END delta_t search: " << delta_t_min << endl ;
      cerr << endl ;
    }

    // // WIP !!
    // if ( delta_t_min < 1 ) {
    //   cerr << "delta_t_min: " << delta_t_min << endl ;
    // }

    if ( 1 < tot_dt + delta_t_min ) {
      delta_t_min = 1 - tot_dt ;
      if ( verbose ) { 
	cerr << "  1 UPDATE delta_t_min" << endl ;
	cerr << "delta_t_min: " << delta_t_min << endl ;
	cerr << "tot_dt: " << tot_dt << endl ;
      }
    }

    tot_dt += delta_t_min ;

    // Move until the end of the time step or until a potential collision
    do_move( l_entity, delta_t_min ) ;

    // Update the velocity of entity after choc
    if ( eps < fabs( tot_dt - 1 ) ) {
      if ( ( idx_ent_col_0 != -1 ) && ( idx_ent_col_1 != -1 ) ) {
	entity_t & ent_0 = l_entity[ idx_ent_col_0 ] ;
	entity_t & ent_1 = l_entity[ idx_ent_col_1 ];

	update_velocity_after_choc( ent_0, ent_1 ) ;
      }
    }

    if ( verbose ) { 
      print_l_entity( l_entity ) ;
      cerr << endl ;
    }

  }


  // ### 3: Check if snaffle are catch by a wizard





  // ### 4: Apply the friction coefficient over all entities
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    entity_t & cur_ent = l_entity[ i ] ;
    apply_friction( cur_ent ) ;
  }


  
  print_l_entity( l_entity ) ;
  cerr << endl ;


  
  // ### 5: Round the values
  for ( int i = 0 ; i < nb_entities ; ++i ) {
    entity_t & cur_ent = l_entity[ i ] ;
    cur_ent.x  = round_half_away( cur_ent.x ) ;
    cur_ent.y  = round_half_away( cur_ent.y ) ;
    cur_ent.vx = round_half_away( cur_ent.vx ) ;
    cur_ent.vy = round_half_away( cur_ent.vy ) ;
  }

  if ( verbose ) {
    cerr << "After friction application" << endl ;
    print_entity( &l_entity[ 0 ] ) ;
    print_entity( &l_entity[ 9 ] ) ;
  }

  return 0 ;
}
  

void get_action( action_t & act ) {
  float tmp_rand = get_rand( ) ;
  if ( tmp_rand < 0.5 ) {
    act.act = ACTION_MOVE ;
  }
  else {
    act.act = ACTION_THROW ;
  }

  act.delta_x = ( get_rand( ) - 0.5 ) * 1000 ;
  act.delta_y = ( get_rand( ) - 0.5 ) * 1000 ;
    
  tmp_rand = get_rand( ) ;
  if ( tmp_rand < 1.0 / 3.0 ) {
    act.thrust = 50 ;
  }
  else if ( tmp_rand < 2.0 * 1.0 / 3.0 ) {
    act.thrust = 100 ;
  }
  else {
    act.thrust = 150 ;
  }
}


void simulation ( const entity_t * l_entity, int nb_entities,
		  gene_t & gene_0, gene_t & gene_1 ) {

  entity_t wk_l_entity[ 12 + 13 ] ;

  memcpy( wk_l_entity, l_entity, sizeof( entity_t ) * nb_entities ) ;

  entity_t * my_wizard_0 ;
  entity_t * my_wizard_1 ;
  
  get_my_wizard( wk_l_entity,
		 &my_wizard_0, &my_wizard_1 ) ;

  int res_play ;
  
  for ( int idx_simu = 0 ; idx_simu < DEEPNESS ; ++idx_simu ) {
    res_play = play( gene_0.action[ idx_simu ], gene_1.action[ idx_simu ],
		     wk_l_entity ) ;

    if ( res_play != 0 ) {
      if ( res_play == -1 ) {
	get_action( gene_0.action[ idx_simu ] ) ;
      }
      else if ( res_play == -2 ) {
	get_action( gene_1.action[ idx_simu ] ) ;
      }

      --idx_simu ;
    }
  }
  
  // cerr << "AFTER A SIMULATION" << endl ;
  // print_l_entity( wk_l_entity ) ;
  // cerr << endl ;

  // cerr << "AFTER A SIMULATION -- wizard_0" << endl ;
  // cerr << " x: " << my_wizard_0->x
  //      << " y: " << my_wizard_0->y
  //      << " vx: " << my_wizard_0->vx
  //      << " vy: " << my_wizard_0->vy
  //      << endl ;

}


void get_gene( gene_t & gene ) {

  gene.score = 0 ;
  
  float tmp_rand ;
  
  for ( int i = 0 ; i < DEEPNESS ; ++i ) {
    get_action( gene.action[ i ] ) ;
  }

}


void print_action( const entity_t & my_wizzard, const gene_t & gene ) {

  const action_t * act = &gene.action[ 0 ] ;
  
  if ( act->act == ACTION_MOVE ) {
    cout << "MOVE" ;
  }
  else {
    cout << "THROW" ;
  }
  cout << " " << my_wizzard.x + act->delta_x
       << " " << my_wizzard.y + act->delta_y
       << " " << act->thrust
       << endl ;
}


void test_do_collision_and_move( ) {
  entity_t ent_test_0 = { 0, TYPE_WIZARD, 1000, 1000, 500, 0, 400, 1, 0 } ;
  // entity_t ent_test_1 = { 1, TYPE_BLUDGER, 2100, 1000, -500, 0, 200, 8, 0 } ;
  entity_t ent_test_1 = { 1, TYPE_BLUDGER, 2100, 1000, -500, 0, 200, 8, 0 } ;

  entity_t l_entity[ 2 ] = { ent_test_0, ent_test_1 } ;

  entity_t & ent_0 = l_entity[ 0 ] ;
  entity_t & ent_1 = l_entity[ 1 ] ;

  nb_entities = 2 ;
  
  cerr << "BEGIN test" << endl ;
  print_entity( &ent_0 ) ;
  print_entity( &ent_1 ) ;

  float delta_t = check_collision( ent_0, ent_1, true ) ;

  cerr << "delta_t: " << delta_t  << endl ;
  
  do_move( l_entity, delta_t, true ) ;
  
  update_velocity_after_choc( ent_0, ent_1, true ) ;

  do_move( l_entity, 1.0 - delta_t, true ) ;

  cerr << "END test" << endl ;
  print_entity( &ent_0 ) ;
  print_entity( &ent_1 ) ;

  exit( 0 ) ;
}


/**
 * Grab Snaffles and try to throw them through the opponent's goal!
 * Move towards a Snaffle and use your team id to determine where you need to throw it.
 **/
int main()
{
  // test_do_collision_and_move( ) ;

  chrono::time_point< chrono::system_clock > start ;
  chrono::duration< float > d ;

  // 0 - INITIALISE global variable
  start = now( ) ;

  cin >> myTeamId; cin.ignore( ) ;

  // Precompute an array of random float
  init_tab_alea_float( ) ;

  
  compute_goal_pos( ) ;


  entity_t * my_wizard_0 ;
  entity_t * my_wizard_1 ;
  entity_t * bludger_0 ;
  entity_t * bludger_1 ;
  // entity_t snaffle_0 ;
  // entity_t snaffle_1 ;
  
  // game loop
  while ( 1 ) {

    read_input( ) ;

    print_l_entity( l_entity ) ;
    
    get_my_wizard( l_entity,
		   &my_wizard_0, &my_wizard_1 ) ;

    // cerr << "1: my_wizard_0->entity_id: " << my_wizard_0->entity_id << endl ;
    // cerr << "WIZARD" << endl;
    
    // print_entity( my_wizard_0 ) ;
    // print_entity( my_wizard_1 ) ;

    get_bludger( l_entity,
		 &bludger_0, &bludger_1 ) ;    
    
    // nearest_snaffle( l_snaffle, my_wizard_0, my_wizard_1,
    // 	                snaffle_0, snaffle_1 ) ;

    // cerr << "SNAFFLE" << endl;
    
    // print_entity( snaffle_0 ) ;
    // print_entity( snaffle_1 ) ;

    // gene_t gene_0 = { { ACTION_MOVE, 4000, 4000, 150, ACTION_MOVE, 4000, 4000, 150 },
    // 		      0.0 } ;
    // gene_t gene_0 = { { ACTION_MOVE, 3000, -1000, 150 },
    // 		      0.0 } ;

    gene_t gene_0 ;
    gene_t gene_1 ;
    
    get_gene( gene_0 ) ;
    get_gene( gene_1 ) ;

    gene_0.action[ 0 ] = { ACTION_MOVE,
    			   bludger_0->x - my_wizard_0->x,
    			   bludger_0->y - my_wizard_0->y,
    			   150 } ;


    // gene_0.action[ 0 ] = { ACTION_MOVE,
    // 			   my_wizard_1->x - my_wizard_0->x,
    // 			   my_wizard_1->y - my_wizard_0->y,
    // 			   50 } ;

    gene_1.action[ 0 ] = { ACTION_MOVE,
    			   my_wizard_0->x - my_wizard_1->x,
    			   my_wizard_0->y - my_wizard_1->y,
    			   50 } ;


    // float target_x_0 = 2500 - my_wizard_0->x ;
    // float target_y_0 = 3750 - my_wizard_0->y ;
    // float target_x_1 = 2500 - my_wizard_1->x ;
    // float target_y_1 = 3750 - my_wizard_1->y ;

    // gene_0.action[ 0 ] = { ACTION_MOVE,
    // 			   target_x_0,
    // 			   target_y_0,
    // 			   100 } ;

    // gene_1.action[ 0 ] = { ACTION_MOVE,
    // 			   target_x_1,
    // 			   target_y_1,
    // 			   100 } ;

    simulation( l_entity, nb_entities, gene_0, gene_1 ) ;

    // print_gene( gene_0 ) ;

    print_action( *my_wizard_0, gene_0 ) ;
    print_action( *my_wizard_1, gene_1 ) ;

    // cout << "MOVE 3250 0 150" << endl;

    // for ( int i = 0 ; i < 2 ; ++i ) {
    //   // i.e.: "MOVE x y thrust" or "THROW x y power"
    //   // cout << "MOVE 8000 3750 100" << endl;
    //   cout << "MOVE 4000 4000 150" << endl;
    // }
    
  }
}
