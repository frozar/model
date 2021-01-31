#pragma GCC optimize "-O3,omit-frame-pointer,inline"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Statement of int*_t and uint*_t type
#include <stdint.h>

// Specific format for printf and uint*_t argument
#include <inttypes.h>

// memcpy
#include <string.h>

#define NB_LINE    12
#define NB_COL      6
#define NB_PIECES   8

/* #define NB_PRED   3 */
#define NB_PRED   8
#define NB_COLOR  5
#define NB_ROT    4
#define NB_HEIGHT 2

#define tint  int8_t

#define min( x, y )  ( (x) < (y) ) ? ( x ) : ( y ) ;
#define max( x, y )  ( (x) < (y) ) ? ( y ) : ( x ) ;

struct height_score {
  tint max ;
  tint sum ;
};

struct score_stat {
  int len_chain ;
  int tab[ NB_PRED ];
};

tint my_grid[NB_LINE][NB_COL];
tint his_grid[NB_LINE][NB_COL];

void print_grid( tint grid[NB_LINE][NB_COL] ){

  for ( tint i = 0; i < NB_LINE ; ++i ) {
    for ( tint j = 0; j < NB_COL ; ++j ) {
      /* fprintf( stderr, "%"PRIi8" ", grid[ i ][ j ] ); */
      fprintf( stderr, "%2d ", grid[ i ][ j ] );
    }
    fprintf( stderr, "\n" );
  }   

}

void print_pieces( tint pieces[NB_PIECES][2] ){

  for ( tint i = 0; i < NB_PIECES ; ++i ) {
    for ( tint j = 0; j < 2 ; ++j ) {
      fprintf( stderr, "%"PRIi8" ", pieces[ i ][ j ] );
    }
    fprintf( stderr, "\n" );
  }   

}

/* void cp_grid( tint grid[NB_LINE][NB_COL],  */
/* 	      tint wk_grid[NB_LINE][NB_COL] ){ */
  
/*   for ( tint i = 0; i < NB_LINE ; ++i ) { */
/*     for ( tint j = 0; j < NB_COL ; ++j ) { */
/*       wk_grid[ i ][ j ] = grid[ i ][ j ]; */
/*     } */
/*   }   */

/* } */

tint verbose = 0;
tint verbose_2 = 0;

tint test_fallen_piece_aux( tint grid[NB_LINE][NB_COL], 
			    tint color,
			    tint pos ){

  tint i_empty = NB_LINE - 1 ;
  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    if ( grid[ i ][ pos ] != ( tint ) -1 ) {
      i_empty = i - 1 ;
      break;
    }
  }

  if ( 0 <= i_empty ) {
    grid[ i_empty ][ pos ] = color;

    /* if ( verbose ) { */
    /*   fprintf( stderr, "GOOD i_empty: %d\n", i_empty ); */
    /*   print_grid( grid ); */
    /* } */
    
    return 1;
  }
  else {
    
    /* if ( verbose ) { */
    /*   fprintf( stderr, "BAD i_empty: %d\n", i_empty ); */
    /*   print_grid( grid ); */
    /* } */

    return 0;
  }

}

tint test_fallen_piece( tint grid[NB_LINE][NB_COL], 
			tint piece[2],
			tint pos, tint rot ){

  // Deal with the colorA and colorB independantly
  // and according with the rot parameter

  tint color_a = piece[ 0 ];
  tint color_b = piece[ 1 ];
  tint color_first, color_second;
  tint pos_first, pos_second;

  switch ( rot ){
  case 0:
    color_first  = color_a ;
    color_second = color_b ;
    pos_first  = pos ;
    pos_second = pos + 1 ;
    break ;

  case 1:
    color_first  = color_a ;
    color_second = color_b ;
    pos_first  = pos ;
    pos_second = pos ;
    break ;

  case 2:
    color_first  = color_b ;
    color_second = color_a ;
    pos_first  = pos - 1 ;
    pos_second = pos ;
    break ;

  case 3:
    color_first  = color_b ;
    color_second = color_a ;
    pos_first  = pos  ;
    pos_second = pos ;
    break ;
  }

  if ( ( pos_first < 0 ) || ( 5 < pos_first ) ||
       ( pos_second < 0 ) || ( 5 < pos_second ) ) {
    return 0 ;
  }

  /* tint res = ( test_fallen_piece_aux( grid, color_first, pos_first ) == ( tint ) 1 ) &&  */
  /*   ( test_fallen_piece_aux( grid, color_second, pos_second ) == ( tint ) 1 ); */

  /* if ( verbose ){ */
  /*   fprintf( stderr, "TEST_FALLEN res: %d\n", res ); */
  /* } */

  /* return res ; */
  return ( test_fallen_piece_aux( grid, color_first, pos_first ) == ( tint ) 1 ) && \
    ( test_fallen_piece_aux( grid, color_second, pos_second ) == ( tint ) 1 );
  
}

tint get_submission( tint *submission, tint len ) {

  tint res = 1;

  if ( submission[ 0 ] == ( tint ) -1 ) { 
      /* memset( submission, ( tint ) 0, sizeof( tint ) * NB_PRED * 2 ); */
      memset( submission, ( tint ) 0, sizeof( tint ) * len );
      res = 1;
  }
  else {
    /* for ( tint i = 0 ; i < NB_PRED ; ++i ) { */
    for ( tint i = 0 ; i < len / 2 ; ++i ) {
      if ( submission[ 2 * i + 1 ] == 3 ) {
	submission[ 2 * i + 1 ] = 0;
	
	if ( submission[ 2 * i + 0 ] == NB_COL - 1 ) {
	  submission[ 2 * i + 0 ] = 0;
	  res = 0;
	  /* break; */
	}
	else {
	  submission[ 2 * i + 0 ] += 1;
	  res = 1;
	  break;
	}

      }
      else {
	submission[ 2 * i + 1 ] += 1;
	res = 1;
	break;
      }
    }
  }

  return res;
}

void ind_2_x_rot( tint index, tint * x, tint * rot ) {

  switch( index ){
  case 0:
    *x = 0;
    *rot = 0;
    break;
  case 1:
    *x = 0;
    *rot = 1;
    break;
  case 2:
    *x = 0;
    *rot = 3;
    break;
  case 19:
    *x = 5;
    *rot = 1;
    break;
  case 20:
    *x = 5;
    *rot = 2;
    break;
  case 21:
    *x = 5;
    *rot = 3;
    break;
    /* defautl: */
    /*   ++ind_submission; */
    /*   *x = ind_submission / NB_ROT; */
    /*   *rot = ind_submission % NB_ROT; */
  }

  if ( ( *x == -1 ) && ( *rot == -1 ) ) {
    ++index;
    *x = index / NB_ROT;
    *rot = index % NB_ROT;      
  }  
  
}

tint get_submission_2( tint *submission, tint len ) {

  /* tint res = 1; */

  tint ind_submission ;
  int nb_possibility = 22 ;
  tint x;
  tint rot;
  
  for ( tint i = 0 ; i < len / 2 ; ++i ){

    x = -1;
    rot = -1;
    
    ind_submission = ( tint ) ( rand() % nb_possibility );
    if ( verbose_2 ){
      fprintf( stderr, "ind_submission: %d\n", ind_submission );
    }
    
    ind_2_x_rot( ind_submission, &x, &rot );
  
    submission[ 2 * i     ] = x;
    submission[ 2 * i + 1 ] = rot;

    if ( verbose_2 ){
      fprintf( stderr, "++ind_submission: %d\n", ind_submission );
      fprintf( stderr, "ind_submission / NB_ROT: %d\n", ind_submission / NB_ROT );
      fprintf( stderr, "ind_submission % NB_ROT: %d\n", ind_submission % NB_ROT );
      fprintf( stderr, "x: %d\n", x );
      fprintf( stderr, "rot: %d\n", rot );
    }
  }

  return 1;
}

void apply_gravity( tint grid[NB_LINE][NB_COL] ){

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

tint reduce_grid_atomic( tint grid[NB_LINE][NB_COL], 
			 int *nb_to_explose,
			 tint bonus_color[ NB_COLOR ] ){

  if ( verbose ) {

    fprintf( stderr, "INPUT reduce_grid_atomic\n" );
    print_grid( grid );

  }
  
  tint colors_position[ 5 ][ NB_LINE * NB_COL * 2 / 2 ] = { { -1 } };
  tint nb_colors[ 5 ] = { 0 };

  tint cur_color;
  tint ind_color;
  tint cur_cpt;

  for ( tint i = 0 ; i < NB_LINE ; ++i ) {
    for ( tint j = 0 ; j < NB_COL ; ++j ) {
      if ( 1 <= grid[ i ][ j ] ) {
	cur_color = grid[ i ][ j ];
	ind_color = cur_color - 1;
	cur_cpt = nb_colors[ ind_color ];
	
	colors_position[ ind_color ][ 2 * cur_cpt     ] = i ;
	colors_position[ ind_color ][ 2 * cur_cpt + 1 ] = j ;

	nb_colors[ ind_color ] += 1;
      }
    }
  }

  if( verbose ) {
    tint i = 0;
    while ( colors_position[ 0 ][ 2 * i ] != 0 ){
      fprintf( stderr, "colors_position i: %d\n" , colors_position[ 0 ][ 2 * i     ] );
      fprintf( stderr, "colors_position j: %d\n" , colors_position[ 0 ][ 2 * i + 1 ] );
      ++i;
    }
  }
  
  tint to_explose[ NB_LINE * NB_COL * 2 / 2 ] = { -1, -1 };
  *nb_to_explose = 0;
  tint color_pos[ 2 ] = { 0, 0 };
  tint has_change = 0;

  tint buff[ NB_LINE * NB_COL * 2 / 2 ]; // = { -1, -1 };
  memset( buff, ( tint ) -1, NB_LINE * NB_COL * 2 / 2 );
  tint gr_size; // = 0 ;
  tint ind_buff;
  tint found;

  int i_loop = 0;
  
  for ( tint i_color = 0 ; i_color < NB_COLOR ; ++i_color ) {

    /* fprintf( stderr, "I_COLOR: %d\nnb_color: %d\n", i_color, nb_colors[ i_color ] ); */
    
    ind_color = 0 ;
    ind_buff  = 0 ;
    gr_size = 0 ;
    /* i_loop = 0 ; */
    while( ind_color < nb_colors[ i_color ] ) {

      /* ++i_loop; */

      /* fprintf( stderr, "i_loop: %d\n", i_loop ); */
      
      /* i = 0; */
      /* while ( buff[ 2 * i ] != -1 ){ */
      /* 	fprintf( stderr, "buff i: %d\n" , buff[ 2 * i     ] ); */
      /* 	fprintf( stderr, "buff j: %d\n" , buff[ 2 * i + 1 ] ); */
      /* 	++i; */
      /* } */

      /* fprintf( stderr, "colors_position: %d\n", colors_position[ i_color ][ 2 * ind_color ] ); */
      /* fprintf( stderr, "colors_position: %d\n", colors_position[ i_color ][ 2 * ind_color + 1 ] ); */

      /* fprintf( stderr, "gr_size: %d\n", gr_size ); */
      
      if ( gr_size == 0 ) {
	color_pos[ 0 ] = colors_position[ i_color ][ 2 * ind_color ];
	color_pos[ 1 ] = colors_position[ i_color ][ 2 * ind_color + 1 ];
	++ind_color;
	/* fprintf( stderr, "ind_color: %d\n", ind_color ); */

	if ( ( color_pos[ 0 ] == -1 ) && ( color_pos[ 1 ] == -1 ) ) {
	  continue;
	}

	buff[ gr_size * 2     ] = color_pos[ 0 ];
	buff[ gr_size * 2 + 1 ] = color_pos[ 1 ];
	++gr_size;
      }
      else {
	color_pos[ 0 ] = buff[ 2 * ind_buff     ];
	color_pos[ 1 ] = buff[ 2 * ind_buff + 1 ];
      }

      /* fprintf( stderr, "color_pos[ 0 ]: %d\n", color_pos[ 0 ] ); */
      /* fprintf( stderr, "color_pos[ 1 ]: %d\n", color_pos[ 1 ] ); */
      
      found = 0;
      
      for ( tint i = ind_color ; i < nb_colors[ i_color ] ; ++i ) {
	if ( ( ( colors_position[ i_color ][ 2 * i     ] == color_pos[ 0 ] + 1 ) &&
	       ( colors_position[ i_color ][ 2 * i + 1 ] == color_pos[ 1 ]     ) ) ||
	     ( ( colors_position[ i_color ][ 2 * i     ] == color_pos[ 0 ] - 1 ) &&
	       ( colors_position[ i_color ][ 2 * i + 1 ] == color_pos[ 1 ]     ) ) ||
	     ( ( colors_position[ i_color ][ 2 * i     ] == color_pos[ 0 ]     ) &&
	       ( colors_position[ i_color ][ 2 * i + 1 ] == color_pos[ 1 ] + 1 ) ) ||
	     ( ( colors_position[ i_color ][ 2 * i     ] == color_pos[ 0 ]     ) &&
	       ( colors_position[ i_color ][ 2 * i + 1 ] == color_pos[ 1 ] - 1 ) ) ) {

	  buff[ gr_size * 2     ] = colors_position[ i_color ][ 2 * i     ];
	  buff[ gr_size * 2 + 1 ] = colors_position[ i_color ][ 2 * i + 1 ];

	  colors_position[ i_color ][ 2 * i     ] = -1 ;
	  colors_position[ i_color ][ 2 * i + 1 ] = -1 ;

	  ++gr_size;
	  found = 1;
	}
      }

      /* fprintf( stderr, "found: %d\n", found ); */
      /* fprintf( stderr, "ind_buff: %d\n", ind_buff); */
      /* fprintf( stderr, "gr_size: %d\n", gr_size); */
	
      if ( ( !found ) && ( ind_buff == gr_size - 1 ) ) {

	if ( 4 <= gr_size ) {
	  for ( tint i = 0 ; i < gr_size ; ++i ) {
	    to_explose[ 2 * (*nb_to_explose)     ] = buff[ 2 * i     ];
	    to_explose[ 2 * (*nb_to_explose) + 1 ] = buff[ 2 * i + 1 ];
	    ++(*nb_to_explose) ;
	    has_change = 1;
	    bonus_color[ i_color ] = 1 ;
	  }
	}
	
	gr_size  = 0 ;
	ind_buff = 0 ;
      }

      if ( gr_size != 0 ) {

	++ind_buff;
	
      }
    } // while( ind_color < nb_colors[ i_color ] )

  } // for ( tint i_color = 0 ; i_color < NB_COLOR ; ++i_color )

  if ( verbose ) { 
    fprintf( stderr, "nb_to_explose: %d\n", *nb_to_explose );
  }

  if ( *nb_to_explose ) {
    for ( tint ind = 0 ; ind < (*nb_to_explose) ; ++ind ) {
      tint i = to_explose[ 2 * ind     ] ;
      tint j = to_explose[ 2 * ind + 1 ] ;

      grid[ i ][ j ] = -1 ;

      tint i_min = max( i - 1, 0 );
      tint i_max = min( i + 1, NB_LINE - 1 );
      tint j_min = max( j - 1, 0 );
      tint j_max = min( j + 1, NB_COL - 1 );

      if ( grid[ i_min ][ j_min ] == 0 )
	grid[ i_min ][ j_min ] = -1;
      if ( grid[ i_min ][ j_max ] == 0 )
	grid[ i_min ][ j_max ] = -1;
      if ( grid[ i_max ][ j_min ] == 0 )
	grid[ i_max ][ j_min ] = -1;
      if ( grid[ i_max ][ j_max ] == 0 )
	grid[ i_max ][ j_max ] = -1;
    }
  
    apply_gravity( grid );
  }

  return has_change;
}

int reduce_grid( tint grid[NB_LINE][NB_COL],
		 int * score, int * len_chain ){
  /* int len_chain = 0 ; */
  int nb_to_explose = 0 ;
  int CP = 0 ;
  tint bonus_color[ NB_COLOR ];
  int CB = 0 ;
  tint nb_bonus_color = 0 ;
  int GB = 0 ;
  int cpt_explose = 0 ;
  int sub_total = 0 ;
  /* int score = 0 ; */

  /* fprintf( stderr, "pass 2\n" ); */
  memset( bonus_color, ( tint ) 0, NB_COLOR );  
  tint has_change = reduce_grid_atomic( grid,
					&nb_to_explose,
					bonus_color );
  cpt_explose += nb_to_explose;
  
  /* fprintf( stderr, "has_change: %d\n", has_change ); */
  while ( has_change ){
    *len_chain += 1;
    // memset( bonus_color, ( tint ) 0, NB_COLOR );  
    has_change = reduce_grid_atomic( grid,
				     &nb_to_explose,
				     bonus_color );
    cpt_explose += nb_to_explose;
    /* fprintf( stderr, "has_change: %"PRIi8"\n", has_change ); */
  }

  if ( *len_chain ) {
    CP = 8 ;
    for ( int i = 1 ; i < *len_chain ; ++i ){
      CP *= 2 ;
    }    
  }

  for ( tint i = 0 ; i < NB_COLOR ; ++i ){
    if ( bonus_color[ i ] == 1 ){
      ++nb_bonus_color;
    }
  }

  switch ( nb_bonus_color ) {
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

  if ( cpt_explose && ( 4 <= cpt_explose ) ){
    GB = 4 - cpt_explose ;
  }
  if ( 11 <= cpt_explose ) {
    GB = 8 ;
  }

  sub_total = min( CP + CB + GB, 999 );

  *score = ( 10 * cpt_explose ) * sub_total;
  
  if ( verbose ) {

    fprintf( stderr, "OUTPUT reduce_grid\n" );
    print_grid( grid );

  }

  /* return len_chain ; */
  /* return score ; */
}

void get_height_stat( tint grid[NB_LINE][NB_COL],
		      struct height_score *h_s ){

  h_s->sum = 0 ;
  h_s->max = 0 ;
  
  tint cur_height = 0;
  for ( tint j = 0 ; j < NB_COL ; ++j ) {
    cur_height = 0 ;
    for ( tint i = NB_LINE - 1 ; -1 < i ; --i ) {
      if ( grid[ i ][ j ] != -1 ) {
	++cur_height;
      }
      else {
	break;
      }
    }
    h_s->sum += cur_height ;
    h_s->max = max( h_s->max, cur_height ) ;
  }
}

int f_compar( const void * elt1, const void * elt2 ) {

  struct height_score h_s_1 = *( struct height_score *)elt1;
  struct height_score h_s_2 = *( struct height_score *)elt2;

  if ( h_s_1.sum < h_s_2.sum ){
    return -1;
  }
  else if ( h_s_1.sum > h_s_2.sum ) {
    return 1;
  }
  else if ( h_s_1.max < h_s_2.max ) {
    return -1;
  }
  else if ( h_s_1.max > h_s_2.max ) {
    return 1;
  }
  else {
    return 0;
  }
    
}

int f_compar_2( const void * elt1, const void * elt2 ) {

  /* int s_t_1[ NB_PRED ] = ( int *)elt1; */
  /* int s_t_2[ NB_PRED ] = ( int *)elt2; */
  struct score_stat *s_t_1 = ( struct score_stat *)elt1;
  struct score_stat *s_t_2 = ( struct score_stat *)elt2;

  if ( s_t_1->len_chain < s_t_2->len_chain ) {
    return 1;
  }
  else if ( s_t_1->len_chain > s_t_2->len_chain ) {
    return -1;
  }
    
  for ( tint i = 0 ; i < NB_PRED ; ++i ){
    if ( s_t_1->tab[ i ] < s_t_2->tab[ i ] ){
      return 1;
    }
    else if ( s_t_1->tab[ i ] > s_t_2->tab[ i ] ){
      return -1;
    }
  }

  return 0;    
}

void lowest_height( tint grid[NB_LINE][NB_COL],
		    tint piece[ 2 ],
		    tint *x, tint *rot ){

  tint wk_grid[NB_LINE][NB_COL] ;
  tint one_submission[ 2 ] = { -1, -1 };
  struct height_score score_tab[ NB_COL * NB_ROT ];
  struct height_score sorted_score_tab[ NB_COL * NB_ROT ];
  int ind_sub = -1 ;
  int dummy_score ;
  int dummy_len_chain ;
  
  while ( get_submission( one_submission, 2 ) ){
    ++ind_sub;
    memcpy( (void *)wk_grid, (void *) grid, sizeof( tint ) * NB_LINE * NB_COL );
    
    if ( test_fallen_piece( wk_grid, piece,
			    one_submission[ 0 ],
			    one_submission[ 1 ] ) ) {

      /* reduce_grid( wk_grid ); */
      reduce_grid( wk_grid, &dummy_score, &dummy_len_chain );
      get_height_stat( wk_grid, &( score_tab[ ind_sub ] ) );
      
    }
    else {
      score_tab[ ind_sub ].sum = NB_COL * NB_LINE;
      score_tab[ ind_sub ].max = NB_LINE;
    }
      
  } // while ( get_submission( one_submission, 2 ) )

  /* for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) { */
  /*   fprintf( stderr, "score_tab[ %d ]: sum %d max %d\n", */
  /* 	     i, score_tab[ i ].sum, score_tab[ i ].max ); */
  /* } */
  
  memcpy( sorted_score_tab,
	  score_tab,
	  sizeof( struct height_score ) * NB_COL * NB_ROT );
  
  qsort( sorted_score_tab,
	 NB_COL * NB_ROT,
	 sizeof( struct height_score ),
	 f_compar );

  /* for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) { */
  /*   fprintf( stderr, "sorted_score_tab[ %d ]: sum %d max %d\n", */
  /* 	     i, sorted_score_tab[ i ].sum, sorted_score_tab[ i ].max ); */
  /* } */
  
  struct height_score better_score = sorted_score_tab[ 0 ] ;
  tint better_score_ind = 0;
  
  for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) {
    if ( ( score_tab[ i ].sum == better_score.sum ) &&
	 ( score_tab[ i ].max == better_score.max ) ) {
      better_score_ind = i;
      break ;
    }
  }

  *x   = better_score_ind / NB_ROT;
  *rot = better_score_ind % NB_ROT;
}

void look_for_chain( tint grid[NB_LINE][NB_COL], 
		     tint pieces[NB_PIECES][2], 
		     tint *x, tint *rot ){

  tint submission[ NB_PRED * 2 ];
  int nb_submission = 0;
  int max_submission = 10000;
  /* int max_submission = 12500; */
  /* int max_submission = 1000; */
  /* int max_submission = 20000; */
  /* int max_submission = 30000; */
  memset( submission, ( tint ) -1, sizeof( tint ) * NB_PRED * 2 );

  tint wk_grid[NB_LINE][NB_COL];
  int len_chain = 0;
  
  tint pos_submission;
  tint rot_submission;
  /* int score_tab[ NB_ROT * NB_COL ]; */

  /* int score_tab_2[ NB_ROT * NB_COL ][ NB_PRED ]; */
  /* int tmp_score_tab[ NB_PRED ]; */
  struct score_stat score_tab_2[ NB_ROT * NB_COL ];
  struct score_stat tmp_score_tab;

  tint ind_submission;
  
  int cur_score ;
  int cpt_score ;
  int score ;

  /* memset( score_tab, 0, sizeof( int ) * NB_ROT * NB_COL ); */

  /* memset( score_tab_2, 0, sizeof( int ) * NB_ROT * NB_COL * NB_PRED ); */
  memset( score_tab_2, 0, sizeof( struct score_stat ) * NB_ROT * NB_COL );

  int i_dbg = 0 ;
  /* while ( get_submission( submission ) ) { */
  while ( get_submission_2( submission, NB_PRED * 2 ) ) {

    ++nb_submission ;

    if ( max_submission < nb_submission ){
      break ;
    }
    
    /* ++i_dbg; */

    /* if ( i_dbg % 1000 == 0 ) { */
    /*   /\* verbose_2 = 1; *\/ */
    /*   fprintf( stderr, "submission:" ); */
    /*   for ( tint i = 0 ; i < NB_PRED * 2; ++i ){ */
    /* 	fprintf( stderr, "%d ", submission[ i ] ); */
    /*   } */
    /*   fprintf( stderr, "\n" ); */
    /* } */
    /* else { */
    /*   /\* verbose_2 = 0; *\/ */
    /* } */
    
    /* if ( ( submission[ 0 ] == 1 ) && */
    /* 	 ( submission[ 1 ] == 3 ) && */
    /* 	 ( submission[ 2 ] == 0 ) && */
    /* 	 ( submission[ 3 ] == 0 ) && */
    /* 	 ( submission[ 4 ] == 0 ) && */
    /* 	 ( submission[ 5 ] == 0 ) ) { */

    /*   verbose = 1 ; */

    /* } */
    /* else { */

    /*   verbose = 0 ; */

    /* } */
    
    memcpy( (void *)wk_grid, (void *) grid, sizeof( tint ) * NB_LINE * NB_COL );

    cpt_score = 0 ;
    score = 0 ;
    len_chain = 0 ;
    tmp_score_tab.len_chain = 0 ;
    memset( tmp_score_tab.tab, ( int ) 0, sizeof( int ) * NB_PRED );

    int continue_test = 1 ;
    
    for ( tint i = 0 ; i < NB_PRED ; ++i ) {
      if ( test_fallen_piece( wk_grid, pieces[ i ],
			      submission[ 2 * i ],
			      submission[ 2 * i + 1 ] ) ) {

	/* len_chain = reduce_grid( wk_grid ); */
	reduce_grid( wk_grid,
		     &score,
		     &len_chain );
	/* tmp_score_tab[ i ] = score ; */
	/* tmp_score_tab.len_chain = len_chain ; */
	tmp_score_tab.len_chain = max( tmp_score_tab.len_chain, len_chain );
	/* tmp_score_tab.len_chain = max( tmp_score_tab.len_chain, len_chain ) - 3; */
	/* tmp_score_tab.len_chain = -3; */
	tmp_score_tab.tab[ i ] = score ;
	cpt_score += score ;
      }
      else {
	cpt_score = 0 ;
	score = 0 ;
	continue_test = 0;
	break ;
      }
    }

    if ( ! continue_test ) {
      continue;
    }

    tint ind_toto ;
    
    /* if ( len_chain ) { */
    if ( cpt_score ) {
      pos_submission = submission[ 0 ];
      rot_submission = submission[ 1 ];

      /* cur_score = score_tab[ pos_submission * NB_ROT + rot_submission ]; */
      /* /\* cur_score = max( cur_score, len_chain ); *\/ */
      /* cur_score = max( cur_score, cpt_score ); */
      /* score_tab[ pos_submission * NB_ROT + rot_submission ] = cur_score; */

      /* ind_submission = pos_submission * NB_ROT + rot_submission; */
      ind_toto = submission[ 0 ] * NB_ROT + submission[ 1 ];
      ind_submission = ind_toto ;
      for ( tint i = 0 ; i < NB_PRED ; ++i ) {

	if ( score_tab_2[ ind_submission ].len_chain < tmp_score_tab.len_chain ){
	  score_tab_2[ ind_submission ].len_chain = tmp_score_tab.len_chain;
	  for ( tint j = i + 1 ; j < NB_PRED ; ++j ){
	    score_tab_2[ ind_submission ].tab[ j ] = tmp_score_tab.tab[ j ];
	  }
	  break;
	}
	else if ( score_tab_2[ ind_submission ].len_chain == tmp_score_tab.len_chain ){
	  continue ;
	}
	else if ( score_tab_2[ ind_submission ].len_chain > tmp_score_tab.len_chain ){
	  break ;
	}
	else if ( score_tab_2[ ind_submission ].tab[ i ] < tmp_score_tab.tab[ i ] ){
	  score_tab_2[ ind_submission ].tab[ i ] = tmp_score_tab.tab[ i ];
	  for ( tint j = i + 1 ; j < NB_PRED ; ++j ){
	    score_tab_2[ ind_submission ].tab[ j ] = tmp_score_tab.tab[ j ];
	  }
	  break;
	}
	else if ( score_tab_2[ ind_submission ].tab[ i ] == tmp_score_tab.tab[ i ] ) {
	  continue;
	}
	else if ( score_tab_2[ ind_submission ].tab[ i ] > tmp_score_tab.tab[ i ] ) {
	  break;
	}
      }
      
    }
    
    /* print_grid( wk_grid ); */

    /* fprintf( stderr, "len_chain: %d\n", len_chain); */
    
    /* break; */
  }

  tint better_submission = -1 ;
  int score_max = -1 ;

  /* for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) { */
  /*   cur_score = score_tab[ i ] ; */

  /*   if ( ( cur_score != 0 ) && ( score_max < cur_score ) ) { */
  /*     fprintf( stderr, "score_max: %d\n", score_max ); */
  /*     fprintf( stderr, "cur_score: %d\n", cur_score ); */
  /*     better_submission = i; */
  /*     score_max = cur_score; */
  /*   } */
  /* } */

  struct score_stat sorted_score_tab_2[ NB_ROT * NB_COL ];

  memcpy( sorted_score_tab_2,
	  score_tab_2,
	  sizeof( struct score_stat ) * NB_ROT * NB_COL );

  qsort( sorted_score_tab_2,
	 NB_COL * NB_ROT,
	 sizeof( struct score_stat ),
	 f_compar_2 );

  score_max = -1;
  tmp_score_tab.len_chain = sorted_score_tab_2[ 0 ].len_chain ;
  for ( tint i = 0 ; i < NB_PRED ; ++i ) {
    tmp_score_tab.tab[ i ] = sorted_score_tab_2[ 0 ].tab[ i ];
    score_max += tmp_score_tab.tab[ i ];
  }

  fprintf( stderr, "tmp_score_tab: len_chain %d : ",
	   tmp_score_tab.len_chain );
  for ( tint j = 0 ; j < NB_PRED ; ++j ) {
    fprintf( stderr, "%d ", tmp_score_tab.tab[ j ] );
  }
  fprintf( stderr, "\n");

  fprintf( stderr, "score_max: %d\n", score_max );

  tint equality ;
  for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) {
    equality = 1 ;
    
    if ( score_tab_2[ i ].len_chain != tmp_score_tab.len_chain ){
      equality = 0 ;
      continue ;
    }
    for ( tint ii = 0 ; ii < NB_PRED ; ++ii ) {
      if ( score_tab_2[ i ].tab[ ii ] != tmp_score_tab.tab[ ii ] ) {
	equality = 0 ;
	continue ;
      }
    }

    if ( equality && ( score_max != -1 ) ){
      better_submission = i;
      break ;
    }
  }

  /* for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) { */
  /*   fprintf( stderr, "score_2 %d: len_chain %d : ", i, score_tab_2[ i ].len_chain ); */
  /*   for ( tint j = 0 ; j < NB_PRED ; ++j ) { */
  /*     fprintf( stderr, "%d ", score_tab_2[ i ].tab[ j ] ); */
  /*   } */
  /*   fprintf( stderr, "\n", i ); */
  /* } */

  for ( tint i = 0 ; i < NB_COL * NB_ROT ; ++i ) {
    /* fprintf( stderr, "sorted score_2 %d: len_chain %d : ", */
    /* 	     i, sorted_score_tab_2[ i ].len_chain ); */
    for ( tint j = 0 ; j < NB_PRED ; ++j ) {
      /* fprintf( stderr, "%d ", sorted_score_tab_2[ i ].tab[ j ] ); */
    }
    /* fprintf( stderr, "\n", i ); */
  }

  fprintf( stderr, "better_submission: %d\n", better_submission );
  
  if ( better_submission != -1 ) {
    /* pos_submission = better_submission / NB_ROT ; */
    /* rot_submission = better_submission % NB_ROT ; */
    /* *x   = pos_submission ; */
    /* *rot = rot_submission ; */

    *x = -1 ;
    *rot = -1 ;
    
    ind_2_x_rot( better_submission, x, rot );
  }
  else {
    lowest_height( grid, pieces[ 0 ], x, rot );
    /* *x   = 2 ; */
    /* *rot = 0 ; */
  }

  /* for ( int i = 0 ; i < NB_COL * NB_ROT ; ++i ) { */
  /*   fprintf( stderr, "score %d: %d\n", i, score_tab[ i ] ); */
  /* } */
  /* fprintf( stderr, "better_submission: %d\n", better_submission ); */
}


void get_x_rot( tint grid[NB_LINE][NB_COL], 
		tint pieces[NB_PIECES][2], 
		tint *x, tint *rot ){

  look_for_chain( grid, pieces, x, rot );
  fprintf( stderr, "AFTER look_for_chain\n" );

}


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement. */
int main()
{
  tint pieces[8][2];

  // game loop
  while (1) {
    for ( tint i = 0; i < 8; ++i) {
      int colorA; // color of the first block
      int colorB; // color of the attached block
      scanf("%d%d", &colorA, &colorB);

      pieces[i][0] = ( tint )colorA;
      pieces[i][1] = ( tint )colorB;
    }

    /* print_pieces( pieces ); */
    /* fprintf( stderr, "\n" ); */

    for ( tint i = 0 ; i < NB_LINE ; ++i ) {
      char row[7];
      scanf("%s", row);

      for ( tint j = 0; j < NB_COL; ++j ) {
	if ( row[ j ] == '.' ) {
	  my_grid[ i ][ j ] = ( tint )-1;
	}
	else {
	  my_grid[ i ][ j ] = ( tint )( row[ j ] - '0' );
	}
      }
    }

    for ( tint i = 0; i < NB_LINE; ++i) {
      char row[7]; // One line of the map ('.' = empty, '0' = skull block, '1' to '5' = colored block)
      scanf("%s", row);

      for ( tint j = 0; j < NB_COL; ++j ) {
	if ( row[ j ] == '.' ) {
	  his_grid[ i ][ j ] = ( tint )-1;
	}
	else {
	  his_grid[ i ][ j ] = ( tint )( row[ j ] - '0' );
	}
      }
    }

    /* print_grid( my_grid ); */

    tint x, rot ;

    get_x_rot( my_grid, pieces, &x, &rot );
    fprintf( stderr, "AFTER get_x_rot\n" );
    fprintf( stderr, "x: %d\n", x );
    fprintf( stderr, "rot: %d\n", rot );
    printf("%d %d\n", x, rot);
    
  }

  return 0;
}



    
    // "x rotation": the column in which to drop your pair of blocks folowed by its rotation (0, 1, 2 or 3)
