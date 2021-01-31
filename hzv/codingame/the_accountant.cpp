#pragma GCC optimize "-O3,omit-frame-pointer,inline"
// #pragma GCC optimize "-O1,omit-frame-pointer,inline"
// #pragma GCC optimize "-O0"
#include <iostream>
#include <string>
//#include <vector>
#include <algorithm>

// floor function
#include <cmath>

// memset function
#include <cstring>

// PI value
#include <math.h>

// chrono
#include <chrono>

using namespace std;

#define DEBUG  0

#define now chrono::system_clock::now

#define NB_ENEMIES 100
#define NB_DATA    100

#define X_MAX    16000
#define Y_MAX     9000

#define DIST_2_MAX   337000000

#define ACTION_MOVE  0
#define ACTION_SHOOT 1

// #define TIME_LIMIT_AG_FIRST 0.900
// #define TIME_LIMIT_AG_WHILE 0.07

// #define TIME_LIMIT_MC_FIRST 0.960
// #define TIME_LIMIT_MC_WHILE 0.090

#define TIME_LIMIT_MC_FIRST 0.920
#define TIME_LIMIT_MC_WHILE 0.085

// #define CHUNK_SIZE_FIRST   250
// #define CHUNK_SIZE_WHILE    25

// #define CHUNK_SIZE_FIRST   300
// #define CHUNK_SIZE_WHILE    50

#define CHUNK_SIZE_FIRST   256
#define CHUNK_SIZE_WHILE    32

// #define CHUNK_SIZE_FIRST   150
// #define CHUNK_SIZE_WHILE    16

// #define CHUNK_SIZE_FIRST   750
// #define CHUNK_SIZE_WHILE    75

// #define AG_DEEPNESS     38
#define AG_DEEPNESS     36


// #define AG_POPULATION   1024
// #define AG_POPULATION   5000
// #define AG_POPULATION   2000
// #define AG_POPULATION   5000

// #define AG_ELITE_RATE      0.1
// #define AG_MUTATION_RATE   0.4

#define AG_MALUS_MOVE            -100
#define AG_MALUS_DEATH           -100
#define AG_MALUS_SUICIDE         -200
#define AG_MALUS_DATA_LOST        -10
#define AG_BONUS_KILL             100.0

// #define NB_ALEA_MONOIDE   2 * 1024 * 1024
// #define NB_ALEA_FLOAT    10 * 1024 * 1024

#define NB_ALEA_MONOIDE   3 * 1024 * 1024
#define NB_ALEA_FLOAT    10 * 1024 * 1024

int antecedent[ 14 ] = { 158988089, 
			 67860440, 
			 38732002, 
			 25477833, 
			 18235270, 
			 13803665, 
			 10874632, 
			 8827108, 
			 7333489, 
			 6206801, 
			 5333598, 
			 4641588, 
			 4082821, 
			 3624404 } ;


int DO_damage_to_enemy( int x ) {

  int res = -1 ;

  for ( int i = 0 ; i < 14 ; ++i ) {
    if ( antecedent[ i ] < x ) {
      res = i + 1 ;
      break ;
    }
  }
  
  return res ;

}

// int DO_damage_to_enemy( int d2 ) {

//   if (d2 < 10874633) {
//     if (d2 < 5333599) {
//       if (d2 < 3624405)
// 	return 15;
//       else if (d2 < 4082822)
// 	return 14;
//       else if (d2 < 4641589)
// 	return 13;
//       else
// 	return 12;
//     } else {
//       if (d2 < 6206802)
// 	return 11;
//       else if (d2 < 7333490)
// 	return 10;
//       else if (d2 < 8827109)
// 	return 9;
//       else
// 	return 8;
//     }
//   } else {
//     if (d2 < 38732003) {
//       if (d2 < 13803666)
// 	return 7;
//       else if (d2 < 18235271)
// 	return 6;
//       else if (d2 < 25477834)
// 	return 5;
//       else
// 	return 4;
//     } else {
//       if (d2 < 67860441)
// 	return 3;
//       else if (d2 < 158988090)
// 	return 2;
//       else
// 	return 1;
//     }
//   }

// }


typedef struct{

  int action ;
  int param_1 ;
  int param_2 ;
  
} monoide_t ;

typedef struct {
  monoide_t data[ AG_DEEPNESS ] ;
  float     score ;
  int       score_true ;
} gene_t ;


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


monoide_t tab_alea [ NB_ALEA_MONOIDE ] = { 0 } ;

void init_tab_alea( ) {
  
  // Real alea
  // srand( time( 0 ) ) ;

  float angle ;
  float dist ;

  float d_x ;
  float d_y ;

  float var_alea ;

  for ( int i = 0 ; i < NB_ALEA_MONOIDE ; ++i ) {
    // var_alea = rand( ) / float( RAND_MAX ) ;
    var_alea = get_rand( ) ;
    
    if( var_alea <= 0.5 ) {
      tab_alea[ i ].action = ACTION_MOVE ;
      // dist = rand( ) / float( RAND_MAX ) ;
      dist = get_rand( ) ;

      // if ( dist <= 0.1 ) {
      // if ( dist <= 0.1 ) {
      // 	dist   = floor( dist * 11 ) * 100 ;

      if ( dist <= 0.1 ) {
      	dist   = floor( get_rand( ) * 11 ) * 100 ;

	// dist   = 1000 ;
	// dist   = 500 + floor( rand() / float( RAND_MAX ) * 6 ) * 100 ;

	// dist   = floor( rand( ) / float( RAND_MAX ) * 11 ) * 100 ;

	// dist   = floor( rand() / float( RAND_MAX ) * 5 ) * 250 ;
	// dist   = floor( rand() / float( RAND_MAX ) * 6 ) * 200 ;
	
	// angle  = floor( rand() / float( RAND_MAX ) * 20 ) * 2 * M_PI / 20 ;

	// angle  = floor( rand( ) / float( RAND_MAX ) * 15 ) * 2 * M_PI / 15 ;
	angle  = floor( get_rand( ) * 15 ) * 2 * M_PI / 15 ;

	// angle  = floor( rand() / float( RAND_MAX ) * 10 ) * 2 * M_PI / 10 ;

	d_y = sqrtf( ( tan( angle ) * tan( angle ) * dist * dist ) / 
		     ( 1 + tan( angle ) * tan( angle ) ) ) ;
	d_x = d_y / tan( angle ) ;
      
	if ( M_PI < angle ) {
	  d_x = - d_x ;
	  d_y = - d_y ;
	}

	tab_alea[ i ].param_1 = d_x ;
	tab_alea[ i ].param_2 = d_y ;
      }
      
      else {
      	tab_alea[ i ].param_1 = 1000 ;
      	tab_alea[ i ].param_2 = 1000 ;
      }
    
    }

    else {
      tab_alea[ i ].action = ACTION_SHOOT ;
    } // END - if( ACTION_MOVE == tab_alea[ i ].action ) {

  }

}


int ind_alea = 0 ;

monoide_t get_alea( ) {
  monoide_t res = tab_alea[ ind_alea ] ;
  ++ind_alea ;
  
  if ( ind_alea == NB_ALEA_MONOIDE ) 
    ind_alea = 0 ;

  return res ;  
}

typedef struct {

  int id;
  int x;
  int y;
  int life;
  // int target_dp_x ;
  // int target_dp_y ;
  // int origin_life;
  
} enemy_t ;

typedef struct {

  int id ;
  int x ;
  int y ;
  bool alive ;
  
} data_t ;

int enemy_count     ;
int enemy_count_old ;
int data_count ;

enemy_t enemies   [ NB_ENEMIES ] ;
enemy_t wk_enemies[ NB_ENEMIES ] ;
data_t  data   [ NB_DATA ] ;
data_t  wk_data[ NB_DATA ] ;

int my_x ;
int my_y ;

int cur_iter ;

int nb_shoot      = 0 ;
int nb_enemy_dead = 0 ;
int enemies_life  = 0 ;

int shortest_length_gene = 2 * AG_DEEPNESS ;

void get_gene_data( monoide_t data[ AG_DEEPNESS ] ) {
  for ( int j = 0 ; j < AG_DEEPNESS ; ++j ) {
    data[ j ] = get_alea( ) ;
  }
}

gene_t get_gene( ) {
  gene_t res ;
  res.score       = 0 ;
  res.score_true  = 0 ;
  get_gene_data( res.data ) ;
  return res ;
}


void print_dbg_action( const monoide_t & data_0 ) {
  if ( ACTION_SHOOT <= data_0.action ) {
    cerr << "SHOOT " << data_0.param_1 ;
  }

  else {
    cerr << "MOVE " 
	 << my_x + data_0.param_1 << " ("
	 << data_0.param_1 << ") "
	 << my_y + data_0.param_2 << " ("
	 << data_0.param_2 << ")" ;
  }
}

void print_a_gene( const gene_t & gene ) {

  cerr << "gene.score: " << gene.score << endl ;
  cerr << "gene.score_true: " << gene.score_true << endl ;
  for ( int i = 0 ; i < AG_DEEPNESS ; ++i ) {
    print_dbg_action( gene.data[ i ] ) ;
    cerr << " | " ;
    if ( i % 3 == 2 ) {
      cerr << endl ;
    }
  }
  cerr << endl ;

}

// void init_genes( vector<gene_t> & genes ) {

//   for ( int i_pop = 0 ; i_pop < AG_POPULATION ; ++i_pop ) {
//     genes[ i_pop ] = get_gene( ) ;
//   }

// }


void cp_enemies( ) {
  for ( int i = 0 ; i < enemy_count ; ++i ) {
    wk_enemies[ i ] = enemies[ i ] ;
  }
}


void cp_data( ) {
  for ( int i = 0 ; i < data_count ; ++i ) {
    wk_data[ i ] = data[ i ] ;
  }
}


int dist2_enemy_data( const enemy_t & enemy_0, const data_t & data_0 ) {
  return ( ( enemy_0.x - data_0.x ) * ( enemy_0.x - data_0.x ) + 
	   ( enemy_0.y - data_0.y ) * ( enemy_0.y - data_0.y ) ) ;
}


int dist2_enemy_my_pos( const int & my_x_0, const int & my_y_0,  
			const enemy_t & enemy_0 ) {
  return ( ( enemy_0.x - my_x_0 ) * ( enemy_0.x - my_x_0 ) + 
	   ( enemy_0.y - my_y_0 ) * ( enemy_0.y - my_y_0 ) ) ;
}


int dist2_data_my_pos( const int & my_x_0, const int & my_y_0,  
		       const data_t & data_0 ) {
  return ( ( data_0.x - my_x_0 ) * ( data_0.x - my_x_0 ) + 
	   ( data_0.y - my_y_0 ) * ( data_0.y - my_y_0 ) ) ;
}


float damage_enemy( const int & my_x_0, const int & my_y_0,  
		    enemy_t & enemy_0, int & wk_nb_enemy_dead ) {

  float idamage = DO_damage_to_enemy( ( enemy_0.x - my_x_0 ) *
				      ( enemy_0.x - my_x_0 ) + 
				      ( enemy_0.y - my_y_0 ) *
				      ( enemy_0.y - my_y_0 ) ) ;

  enemy_0.life -= idamage ;

  if ( enemy_0.life <= 0 ) {
    ++wk_nb_enemy_dead ;
    return AG_BONUS_KILL ;
  }
  else {
    // return idamage / 15.0 ;
    return 0 ;
  }

}

void get_critical_enemy( const enemy_t tab_enemies[ NB_ENEMIES ], 
			 const data_t tab_data[ NB_DATA ], 
			 const int & enemy_count, 
			 const int & data_count,
			 const int & my_x, const int & wk_my_y, 
			 int & res_id, int & res_ind ) {

  res_id  = -1 ;
  res_ind = -1 ;

  int dist_min = 3 * DIST_2_MAX ;

  int tmp_dist2 ;

  for ( int ind_en = 0 ; ind_en < enemy_count ; ++ind_en ) {

    if ( tab_enemies[ ind_en ].life <= 0 ) {
      continue ;
    }
    
    for ( int ind_da = 0 ; ind_da < data_count ; ++ind_da ) {

      if ( ! tab_data[ ind_da ].alive ) {
	continue ;
      }

      // goback
      // tmp_dist2  = dist2_enemy_data( tab_enemies[ ind_en ], tab_data[ ind_da ] ) ;
      // tmp_dist2 += dist2_data_my_pos( my_x, my_y, tab_data[ ind_da ] ) ;
      // tmp_dist2 += dist2_enemy_my_pos( my_x, my_y, tab_enemies[ ind_en ] ) ;

      tmp_dist2  = 1.0 * dist2_enemy_data( tab_enemies[ ind_en ], tab_data[ ind_da ] ) ;
      // tmp_dist2 += 1.0 / 3 * dist2_enemy_my_pos( my_x, my_y, tab_enemies[ ind_en ] ) ;
      
      // tmp_dist2 += 1.0 * dist2_data_my_pos( my_x, my_y, tab_data[ ind_da ] ) ;
      tmp_dist2 += 0.13 * dist2_enemy_my_pos( my_x, my_y, tab_enemies[ ind_en ] ) ; 
     
      if ( tmp_dist2 < dist_min ) {
	dist_min = tmp_dist2 ;
	res_id   = tab_enemies[ ind_en ].id ;
	res_ind  = ind_en ;
      }

    }
  }

}

void get_critical_data( const enemy_t tab_enemies[ NB_ENEMIES ], 
			const data_t tab_data[ NB_DATA ], 
			const int & enemy_count, 
			const int & data_count,
			int & res_ind_da, int & res_ind_en ) {

  int dist_min = DIST_2_MAX ;

  int tmp_dist2 ;

  for ( int ind_en = 0 ; ind_en < enemy_count ; ++ind_en ) {

    if ( tab_enemies[ ind_en ].life <= 0 )
      continue ;
    
    for ( int ind_da = 0 ; ind_da < data_count ; ++ind_da ) {

      if ( ! tab_data[ ind_da ].alive ) {
	continue ;
      }

      tmp_dist2 = dist2_enemy_data( tab_enemies[ ind_en ], tab_data[ ind_da ] ) ;

      if ( tmp_dist2 < dist_min ) {
	dist_min = tmp_dist2 ;
	res_ind_da = ind_da ;
	res_ind_en = ind_en ;
      }

    }
  }

}


void get_sub_critical_data( const int & ind_en_0,
			    const int & ind_da_0,
			    const enemy_t tab_enemies[ NB_ENEMIES ], 
			    const data_t tab_data[ NB_DATA ], 
			    const int & data_count, 
			    int & res_ind_da ) {

  int dist_min = DIST_2_MAX ;

  int tmp_dist2 ;

  res_ind_da = ind_da_0 ;

  for ( int ind_da = 0 ; ind_da < data_count ; ++ind_da ) {

    if ( ( ind_da == ind_da_0 ) || ( ! tab_data[ ind_da ].alive ) ) {
      continue ;
    }      

    tmp_dist2 = dist2_enemy_data( tab_enemies[ ind_en_0 ], tab_data[ ind_da ] ) ;

    if ( tmp_dist2 < dist_min ) {
      dist_min = tmp_dist2 ;
      res_ind_da = ind_da ;
    }

  }

}


void print_enemies( const enemy_t enemies[ NB_ENEMIES ], const int & enemy_count ) {

  cerr << "Enemies: " << endl ;
  for( int i = 0 ; i < enemy_count ; ++i ) {
    cerr << enemies[ i ].id   << ", "
	 << enemies[ i ].x    << ", "
	 << enemies[ i ].y    << ", "
	 << enemies[ i ].life << " | "
	 << sqrtf( ( float ) dist2_enemy_my_pos( my_x, my_y,  
						enemies[ i ] ) )
	 << endl ;
  }

}

void print_data( const data_t data[ NB_DATA ], const int & data_count ) {

  cerr << "Data: " << endl ;
  for( int i = 0 ; i < data_count ; ++i ) {
    cerr << data[ i ].id   << ", "
	 << data[ i ].x    << ", "
	 << data[ i ].y    << ", "
	 << data[ i ].alive 
	 << endl ;
  }

}

int get_nb_data_alive( data_t data[ NB_DATA ], int data_count ) {

  int res = 0 ;
  
  for ( int i_da = 0 ; i_da < data_count ; ++i_da ) {
    if ( data[ i_da ].alive )
      ++res ;
  }

  return res ;

}

bool gene_compare_higher_first( const gene_t & g_0, const gene_t & g_1 ) {

  // print_a_gene( g_0 ) ;
  // cerr << "IN COMPARE" << endl ;
  
  if ( g_0.score_true > g_1.score_true ) {
    return true ;
  }
  else if ( g_0.score_true < g_1.score_true ) {
    return false ;
  }
  else if ( g_0.score_true == g_1.score_true ) {
    if ( g_0.score > g_1.score ) {
      return true ;
    }
    else if ( g_0.score < g_1.score ) {
      return false ;
    }
    // Perfect equallity
    else {
      return false ;
    }
  }

  return true ;
}


void read_input( bool first_time = false ) {

  int x;
  int y;
  cin >> x >> y; cin.ignore();

  // cerr << x << " " << y << endl ;

  my_x = x ;
  my_y = y ;

  memset( data, 0, sizeof( data_t ) * NB_DATA ) ;
  memset( enemies, 0, sizeof( enemy_t ) * NB_ENEMIES ) ;

  int dataCount;
  cin >> dataCount; cin.ignore();

  // cerr << "dataCount: " << dataCount << endl ;

  data_count = dataCount ;
  
  // cerr << "Data: " << endl ; 
  for ( int i = 0 ; i < dataCount ; ++i ) {
    int dataId;
    int dataX;
    int dataY;
    cin >> dataId >> dataX >> dataY; cin.ignore();

    // cerr << dataId << " " << dataX << " " << dataY << endl ;

    data[ i ] = { dataId, dataX, dataY, true } ;
    // cerr << dataId << " "
    // 	 << dataX << " "
    // 	 << dataY
    // 	 << endl ;
  }

  int enemyCount;
  cin >> enemyCount; cin.ignore();

  // cerr << enemyCount << endl ;

  enemy_count = enemyCount ;

  for (int i = 0; i < enemyCount; i++) {
    int enemyId;
    int enemyX;
    int enemyY;
    int enemyLife;
    cin >> enemyId >> enemyX >> enemyY >> enemyLife; cin.ignore();

    // cerr << enemyId << " " << enemyX << " " << enemyY << " " << enemyLife << endl ;

    enemies[ i ] = { enemyId, enemyX, enemyY, enemyLife } ;

    if ( first_time ) {
      enemies_life += enemyLife ;
    }
  }

}


void compute_score_true( data_t wk_data[ NB_DATA ], const int & wk_data_count,
			 const int & wk_nb_enemy_dead, const int & wk_nb_shoot, 
			 int & score_true ) {

  // GENE SCORE TRUE
  int DP = get_nb_data_alive( wk_data, wk_data_count ) ;
  
  score_true = wk_nb_enemy_dead * 10 + DP * 100 +		\
    DP * max( 0, ( enemies_life - 3 * ( wk_nb_shoot ) ) ) * 3 ;

  // if ( verbose ) {
  //   cerr << "In compute_score_true" << endl ;
  //   cerr << "DP: " << DP << endl;
  //   cerr << "wk_nb_enemy_dead: " << wk_nb_enemy_dead << endl ;
  //   cerr << "wk_nb_shoot: " << wk_nb_shoot << endl ;
  //   cerr << "enemies_life: " << enemies_life << endl ;
  //   cerr << "score_true: " << score_true << endl ;
  // }

}

// int tmp_best_gene_score_true ;

void simulation_a_gene( gene_t & gene ) {

  int res_x, res_y ;
  int ind_en ;

  int ind_nearest_data ;
  int dist2_min ;
  int tmp_dist2 ;
  data_t * nearest_data ;
  enemy_t * cur_enemy ;
  int d_x ;
  int d_y ;

  float factor ;
  float factor_score ;

  monoide_t * m ;

  // Initialisation of the environment for a simualtion of a gene
  int wk_my_x = my_x ;
  int wk_my_y = my_y ;

  cp_data( ) ;
  cp_enemies( ) ;
  int wk_enemy_count   = enemy_count ; 
  int wk_data_count    = data_count ;

  int wk_nb_shoot      = nb_shoot ;
  int wk_nb_enemy_dead = nb_enemy_dead ;

  gene.score       = 0 ;
  gene.score_true  = 0 ;

  int i_deep ;
  for ( i_deep = 0 ; i_deep < AG_DEEPNESS ; ++i_deep ) {

    factor_score = ( ( float ) AG_DEEPNESS ) / ( 1 + i_deep ) ;

    
    // ### 0 - ENEMIES MOVE towards their target data point
    for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {

      cur_enemy = &wk_enemies[ i_en ] ;
      if ( cur_enemy->life <= 0 ) {
	continue ;
      }

      // if ( ( cur_enemy->target_dp_x != -1 ) && ( cur_enemy->target_dp_y != -1 ) ) {

      // 	cur_enemy->x = cur_enemy->target_dp_x ;
      // 	cur_enemy->y = cur_enemy->target_dp_y ;

      // 	cur_enemy->target_dp_x = -1 ;
      // 	cur_enemy->target_dp_y = -1 ;
      // }

      // else { 
	ind_nearest_data = -1 ;
	dist2_min        = DIST_2_MAX ;

	// Get the closest alive data point
	for ( int ind_da = 0 ; ind_da < wk_data_count ; ++ind_da ) {

	  if ( ! wk_data[ ind_da ].alive ) {
	    continue ;
	  }

	  tmp_dist2 = dist2_enemy_data( *cur_enemy, wk_data[ ind_da ] ) ;
	  if ( tmp_dist2 < dist2_min ) {
	    dist2_min        = tmp_dist2 ;
	    ind_nearest_data = ind_da ;
	  }
	  
	}

	nearest_data = &wk_data[ ind_nearest_data ] ;

	d_x = nearest_data->x - cur_enemy->x ;
	d_y = nearest_data->y - cur_enemy->y ;

	if ( ( d_x != 0 ) || ( d_y != 0 ) ) {

	  if ( ( 500 * 500 ) < dist2_min ) {
	    factor = 500.0 / sqrtf( d_x * d_x + d_y * d_y ) ;
	  }
	  else {
	    factor = sqrtf( dist2_min / ( d_x * d_x + d_y * d_y ) ) ;
	  }
	
	  // cur_enemy->x += ( int ) floor( factor * d_x ) ;
	  // cur_enemy->y += ( int ) floor( factor * d_y ) ;
	  cur_enemy->x += ( int ) ( factor * d_x ) ;
	  cur_enemy->y += ( int ) ( factor * d_y ) ;
	}
      // }
      
    }  // END - for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {


    m = &gene.data[ i_deep ] ;

    // ### 1 - ACTION MOVE
    if ( m->action == ACTION_MOVE ) {

      // IF a standard delta_x and delta_y is given as input
      if ( ! ( ( m->param_1 == 1000 ) && ( m->param_2 == 1000 ) ) ) {
	res_x = wk_my_x + m->param_1 ;
	res_y = wk_my_y + m->param_2 ;

	// MOVE is icorrect: out of bound
	if ( ( res_x < 0 ) || ( X_MAX < res_x ) || \
	     ( res_y < 0 ) || ( Y_MAX < res_y ) ) {

	  // GENE SCORE
	  gene.score += AG_MALUS_MOVE * factor_score ;
	  gene.score_true = 0 ;
	  break ;

	}
      
	// MOVE correct
	wk_my_x = res_x ;
	wk_my_y = res_y ;

      }

      else {

	int ind_da_0 ;
	int ind_da_1 ;
	int ind_en_0 ;
	
	get_critical_data( wk_enemies, wk_data, wk_enemy_count, wk_data_count,
			   ind_da_0, ind_en_0 ) ;
	
	get_sub_critical_data( ind_en_0, ind_da_0,
			       wk_enemies, wk_data, wk_data_count,
			       ind_da_1 ) ;

	float alea_variable = get_rand( ) ;

	if ( alea_variable <= ( 1.0 / 3 ) ) {
	  d_x = wk_data[ ind_da_0 ].x - wk_my_x ;
	  d_y = wk_data[ ind_da_0 ].y - wk_my_y ;
	}
	else if ( alea_variable <= ( 2 * 1.0 / 3 ) ) {
	  d_x = wk_data[ ind_da_1 ].x - wk_my_x ;
	  d_y = wk_data[ ind_da_1 ].y - wk_my_y ;
	}
	else {
	  d_x = ( wk_data[ ind_da_0 ].x + wk_data[ ind_da_1 ].x ) / 2 - wk_my_x ;
	  d_y = ( wk_data[ ind_da_0 ].y + wk_data[ ind_da_1 ].y ) / 2 - wk_my_y ;
	}
	
	// IF I am not on the critical data_point
	if ( ( d_x != 0 ) || ( d_y != 0 ) ) {

	  if ( ( 1000 * 1000 ) < ( d_x * d_x + d_y * d_y ) ) {
	    factor = 1000.0 / sqrtf( d_x * d_x + d_y * d_y ) ;
	    m->param_1 = ( int ) ( factor * d_x ) ;
	    m->param_2 = ( int ) ( factor * d_y ) ;
	  }
	  else {
	    m->param_1 = d_x ;
	    m->param_2 = d_y ;
	  }
	
	  wk_my_x += m->param_1 ;
	  wk_my_y += m->param_2 ;
	}
      }
      
    }  // END - if ( m->action == ACTION_MOVE ) {
    
    
    // ### 2 - CHECK if my position is a dead position
    for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {

      if ( wk_enemies[ i_en ].life <= 0 ) {
	continue ;
      }
      
      
      if ( dist2_enemy_my_pos( wk_my_x, wk_my_y,
			       wk_enemies[ i_en ] ) <= 2000 * 2000 ) {
	// GENE SCORE
	gene.score      += AG_MALUS_SUICIDE * factor_score ;
	gene.score_true  = 0 ;
	
	
	return ;
      }
      
    }  // END - for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {
    
    
    // ### 3 - ACTION SHOOT
    if ( ACTION_SHOOT <= m->action ) {
      
      ++wk_nb_shoot ;

      get_critical_enemy( wk_enemies, wk_data, wk_enemy_count, wk_data_count,
			  wk_my_x, wk_my_y,
			  m->param_1, ind_en ) ;

      // GENE SCORE
      gene.score += damage_enemy( wk_my_x, wk_my_y, 
      				  wk_enemies[ ind_en ],
      				  wk_nb_enemy_dead) * factor_score ;

    }


    // ### 4 - COMPUTE DATA LOST
    for ( int i_da = 0 ; i_da < wk_data_count ; ++i_da ) {

      if ( ! wk_data[ i_da ].alive ) {
	continue ;
      }

      for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {

	if ( wk_enemies[ i_en ].life <= 0 ) {
	  continue ;
	}

	if ( dist2_enemy_data( wk_enemies[ i_en ], wk_data[ i_da ] ) == 0 ) {
	
	  // wk_enemies[ i_en ].target_dp_x = wk_data[ i_da ].x ;
	  // wk_enemies[ i_en ].target_dp_y = wk_data[ i_da ].y ;
	  
	  wk_data[ i_da ].alive = false ;
	  // GENE SCORE
	  gene.score += AG_MALUS_DATA_LOST * factor_score ;

	  break ;

	}
      }  // END - for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {

    }  // END - for ( int i_da = 0 ; i_da < wk_data_count ; ++i_da ) {

    
    // ### 5 - CHECK if no more data point are in the game
    if ( get_nb_data_alive( wk_data, wk_data_count ) == 0 ) {
      // GENE SCORE
      gene.score += AG_MALUS_DEATH * factor_score ;
      // gene.score_true = 0 ; 

      compute_score_true( wk_data, wk_data_count,
      			  wk_nb_enemy_dead, wk_nb_shoot,
      			  gene.score_true ) ;

      shortest_length_gene = min( shortest_length_gene, i_deep ) ;

      break ;
    }


    // ### 6 - CHECK IF EVERY ENEMY ARE DEAD
    bool remain_enemy = false ;
    for ( int i_en = 0 ; i_en < wk_enemy_count ; ++i_en ) {
      if ( 0 < wk_enemies[ i_en ].life ) {
	remain_enemy = true ;
	break ;
      }
    }
    if ( ! remain_enemy ) {
      
      compute_score_true( wk_data, wk_data_count,
			  wk_nb_enemy_dead, wk_nb_shoot,
			  gene.score_true ) ;

      shortest_length_gene = min( shortest_length_gene, i_deep ) ;

      break ;
    }

  }  // END - for ( int i_deep = 0 ; i_deep < AG_DEEPNESS ; ++i_deep ) {

}  // END - simulation_a_gene( gene_t & gene ) {


void print_action( const monoide_t & data_0 ) {
  if ( ACTION_SHOOT <= data_0.action ) {
    ++nb_shoot ;
    cout << "SHOOT " << data_0.param_1 << endl ;
  }

  else {
    cout << "MOVE " 
	 << my_x + data_0.param_1 << " "
	 << my_y + data_0.param_2 << endl ;
  }
}


void update_best_gene( gene_t & best_gene, const gene_t & wk_gene ) {

  if ( gene_compare_higher_first( wk_gene, best_gene ) ) {
    best_gene = wk_gene ;
  }

}


void get_best_gene_MC( const float time_limit, 
		       const int chunk_size,
		       gene_t & best_gene, chrono::duration< float > & d ) {

  gene_t wk_gene ;
  int nb_simu = 0 ;

  chrono::time_point< chrono::system_clock > start = now( ) ;

  // pure MONTE CARLO
  while ( d.count() < ( time_limit ) ) {

    for ( int i_chunk = 0 ; i_chunk < chunk_size ; ++i_chunk ) {
      // tmp_best_gene_score_true = best_gene.score_true ;
      wk_gene = get_gene( ) ;
      simulation_a_gene( wk_gene ) ;

      update_best_gene( best_gene, wk_gene ) ;

      ++nb_simu ;
    }
  
    d = now() - start ;
  }

  cerr << "  ### MC NB SIMULATION: " << nb_simu << endl ;

  cerr << " In main BEST gene score:      " << best_gene.score << endl ;
  cerr << " In main BEST gene score true: " << best_gene.score_true << endl ;
  // print_a_gene( best_gene ) ;
  cerr << endl ;

}


int main( ) {

  chrono::time_point< chrono::system_clock > start ;
  chrono::duration< float > d ;

  float time_init ;

  // 0 - INITIALISE global variable
  start = now() ;


  init_tab_alea_float( ) ; // Precompute an array of random float
  init_tab_alea( ) ;       // Precompute an array of random gene

  // vector<gene_t> genes( AG_POPULATION ) ;
  
  // vector<gene_t> genes ;
  
  // if ( SIMULATION_SWITCH == SIMULATION_AG ) {
  //   genes.reserve( AG_POPULATION ) ;
  //   init_genes( genes ) ;    // Initialise all genes for AG
  // }
  
  cur_iter = 0 ;
  
  d = now( ) - start ;

  time_init = d.count( ) ;
  
  cerr << "Init time: " << d.count( ) * 1000 << " ms" << endl ;


  // HEAD code
  // 1 - FIRST GAME iteraton outside the main while loop
  start = now( ) ;
  
  read_input( true ) ;

  ++cur_iter ;
  cerr << "cur_iter: " << cur_iter << endl ;
  cerr << "my_x " << my_x << endl ;
  cerr << "my_y " << my_y << endl ;

  // print_enemies( enemies, enemy_count ) ;
  // print_data( data, data_count ) ;

  enemy_count_old = enemy_count ;
  
#if DEBUG
  cerr << "my_x " << my_x << endl ;
  cerr << "my_y " << my_y << endl ;

  print_enemies( enemies, enemy_count ) ;
  print_data( data, data_count ) ;
  cerr << endl ;
#endif

  gene_t best_gene ;
  best_gene.score       = AG_MALUS_SUICIDE * 2 ;
  best_gene.score_true  = 0 ;

  d = now() - start ;

  // switch( SIMULATION_SWITCH ) {
  // case SIMULATION_MC :
    get_best_gene_MC( TIME_LIMIT_MC_FIRST - time_init, CHUNK_SIZE_FIRST,
		      best_gene, d ) ;
  //   break ;
  // case SIMULATION_AG :
  //   get_best_gene_AG( TIME_LIMIT_AG_FIRST - time_init,
  //   		      genes, 
  //   		      best_gene, d ) ;
  //   break ;
  // }

  cerr << "AFTER SIMULATION" << endl ;
  // print_a_gene( best_gene ) ;
  // simulation_a_gene( best_gene, true ) ;


  // cerr << "WHILE loop iteraton time: " << d.count( ) * 1000 << " ms" << endl ;

  // cerr << "nb_enemy_dead: " << nb_enemy_dead << endl ;
  // simulation_a_gene( best_gene, true ) ;

  // if ( ( 4 <= cur_iter ) && ( cur_iter <= 9 ) ) {
  //   cout << "MOVE 15000 8500" << endl ;
  // }
  // else {
    print_action( best_gene.data[ 0 ] ) ;
  // }
  
  d = now() - start ;
  
  cerr << "First iteraton time: " << d.count( ) * 1000 << " ms" << endl ;

  // 2 - GAME LOOP
  while ( 1 ) {

    start = now() ;

    read_input( ) ;    

    ++cur_iter ;
    cerr << "cur_iter: " << cur_iter << endl ;
    cerr << "my_x " << my_x << endl ;
    cerr << "my_y " << my_y << endl ;

    // print_enemies( enemies, enemy_count ) ;
    // print_data( data, data_count ) ;

    nb_enemy_dead   += enemy_count_old - enemy_count ;
    enemy_count_old  = enemy_count ;

    // cerr << "my_x " << my_x << endl ;
    // cerr << "my_y " << my_y << endl ;

    // print_enemies( enemies, enemy_count ) ;
    // print_data( data, data_count ) ;

    // Circulation permutation of action's best_gene
    for ( int j = 0 ; j < AG_DEEPNESS - 1 ; ++j ) {
      best_gene.data[ j ] = best_gene.data[ j + 1 ] ;
    }
    best_gene.data[ AG_DEEPNESS - 1 ] = get_alea( ) ;
    simulation_a_gene( best_gene ) ;
    // cerr << "DBG AFTER circular permutation" << endl ;
    // cerr << "best_gene.score_true: " << best_gene.score_true << endl ;
    // // print_a_gene( best_gene ) ;
    // simulation_a_gene( best_gene, true ) ;
    // cerr << "best_gene.score_true: " << best_gene.score_true << endl ;
    // cerr << "AFTER SIMULATION" << endl ;
    // cerr << "best_gene.score_true: " << best_gene.score_true << endl ;

    d = now() - start ;
    
    // switch( SIMULATION_SWITCH ) {
    // case SIMULATION_MC :
    get_best_gene_MC( TIME_LIMIT_MC_WHILE, CHUNK_SIZE_WHILE,
		      best_gene, d ) ;
    // break ;

    // case SIMULATION_AG :
    //   // Circulation permutation of action's all genes
    //   for ( int i_pop = 0 ; i_pop < AG_POPULATION ; ++i_pop ) {
    //   	for ( int j = 0 ; j < AG_DEEPNESS - 1 ; ++j ) {
    //   	  genes[ i_pop ].data[ j ] = genes[ i_pop ].data[ j + 1 ] ;
    //   	}
    //   	genes[ i_pop ].data[ AG_DEEPNESS - 1 ] = get_alea( ) ;
    //   }

    //   get_best_gene_AG( TIME_LIMIT_AG_WHILE,
    //   			genes, 
    //   			best_gene, d ) ;
    //   break ;
    // }

    // cerr << "nb_enemy_dead: " << nb_enemy_dead << endl ;
    // cerr << "DBG ALL simulation" << endl ;
    // simulation_a_gene( best_gene, true ) ;
    // cerr << "AFTER SIMULATION" << endl ;
    // print_a_gene( best_gene ) ;

    // if ( ( 4 <= cur_iter ) && ( cur_iter <= 15 ) ) {
    //   cout << "MOVE 15000 8500" << endl ;
    // }
    // else {
      print_action( best_gene.data[ 0 ] ) ;
    // }

    d = now() - start ;
  
    cerr << "WHILE loop iteraton time: " << d.count( ) * 1000 << " ms" << endl ;
    // cerr << "enemy_count: " << enemy_count << endl ;
    // cerr << "data_count: " << data_count << endl ;
    
  } // END - while( 1 )
}
