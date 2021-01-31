
#include <iostream>
#include <vector>
#include <set>

#include <cstdio>
#include <cstdlib>

using namespace std;

unsigned int* a_ = NULL;

inline int get_i_bit( unsigned int* tab, int size, int i ) {
  return tab[ i / size ] >> ( i % size ) & 1;
}

inline void set_i_bit( unsigned int* tab, int i, int bit ) {
  tab[ i / 32 ] ^= bit << ( i % 32 );
}

inline void print_tab( unsigned int * tab, int size_B ) {
  for(int i = 0; i < size_B / 2 ; i++) {
    printf ( "%2d ", tab[i] );
  }
  printf ( "\n" );

  for(int i = 0; i < size_B / 2 ; i++) {
    printf ( "%2d ", i);
  }
  printf ( "\n" );

  for(int i = size_B / 2; i < size_B ; i++) {
    printf ( "%2d ", tab[i] );
  }
  printf ( "\n" );

  for(int i = size_B / 2; i < size_B ; i++) {
    printf ( "%2d ", i);
  }
  printf ( "\n" );
}

inline bool check_diag_val_binary( unsigned int * b, int ind_diag, 
				   unsigned int * a, int size ) {
  // unsigned int diag_val = B[ ind_diag ];

  unsigned int diag_val = get_i_bit( b, size, ind_diag );
  unsigned int tmp = 0;

  if ( ind_diag <= size -1 ) {
    for ( int i = 0; i <= ind_diag ; i++ ){
      // tmp ^= A[ size + i ] & A [ ind_diag - i ];

      tmp ^= get_i_bit( a, size, size + i ) & 
	get_i_bit( a, size, ind_diag - i ) ;
      // cout << "  check_diag:" << 
      // 	" A[ " << size + i << "]: " << A[ size + i ] <<
      // 	" A[ " << ind_diag - i << " ]: " << A[ ind_diag - i ] <<
      // 	" &: " << ( A[ size + i ] & A [ ind_diag - i ] ) <<
      // 	" tmp: " << tmp << endl;
    }
  }
  else {
    int norm_ind_diag = ind_diag + 1;
    // for ( int i = 0; i <= (size * 2 -1) - ind_diag; i++ ){
    for ( int i = 0; i <= (size * 2 - 1 -1) - ind_diag; ++i ){

      tmp ^= get_i_bit( a, size, size -1 -i ) & 
	get_i_bit( a, size, norm_ind_diag + i ) ;

      // tmp ^= A[ size -1 -i ] & A [ norm_ind_diag + i ];

      // if (ind_diag == 62 ) {
      // 	// cout << dec << size -1 -i << " : " << size + ( ind_diag - (size -1) ) + i << endl;
      // 	cout << dec << size -1 -i << " : " << ind_diag + i << endl;
      // }
    }
  }

  // cout << "diag_val: " << diag_val << " tmp: " << tmp << endl;
  return tmp == diag_val;
}


inline bool check_diag_val( unsigned int * B, int ind_diag, 
			    unsigned int * A, int size ) {
  unsigned int diag_val = B[ ind_diag ];
  unsigned int tmp = 0;

  if ( ind_diag <= size -1 ) {
    for ( int i = 0; i <= ind_diag ; i++ ){
      tmp ^= A[ size + i ] & A [ ind_diag - i ];
    }
  }

  else {
    // last diagonale index : size * 2 - 2
    int norm_ind_diag = ind_diag + 1;

    // for ( int i = 0; i <= (size -1) - (ind_diag - (size -1)); i++ ){
    for ( int i = 0; i <= (size * 2 - 1 -1) - ind_diag; ++i ){
      // tmp ^= A[ size -1 -i ] & A [ size + ( ind_diag - (size -1) ) + i ];
      tmp ^= A[ size -1 -i ] & A [ norm_ind_diag + i ];

      // if ( 61 <= ind_diag ) {
      // // if ( ind_diag <= 48 ) {
      // 	// cout << dec << size -1 -i << " : " << size + ( ind_diag - (size -1) ) + i << endl;
      // 	cout << diag_val << " : " <<
      // 	  A[ size -1 -i ] << " & " << A [ norm_ind_diag + i ] << " -- " ;
      // 	cout << dec << ind_diag << " : " <<
      // 	  size -1 -i << " - " << norm_ind_diag + i << endl;
      // }

    }
  }

  // cout << "diag_val: " << diag_val << " tmp: " << tmp << endl;
  return tmp == diag_val;
}


inline void reset_state( int * diags_state, int ind_diag, 
			 unsigned int * A, int size ) {
  diags_state[ ind_diag ] = 0;
  if ( ind_diag <= size - 1 ) {
    A [ ind_diag ] = 0;
    A [ ind_diag + size ] = 0;
  }
  else {
    // last diagonale index : size * 2 - 2
    int norm_ind_diag = ind_diag + 1;
    A [ norm_ind_diag ] = 0;
    A [ norm_ind_diag - size ] = 0;    
  }
}


inline void update_A( int state_val, int ind_diag, 
		      unsigned int * A, int size ) {
  if ( ind_diag <= size - 1 ) {
    if ( state_val == 0 ) {
      A[ ind_diag ]        = 0;
      A[ ind_diag + size ] = 0;
    }
    if ( state_val == 1 ) {
      A[ ind_diag ]        = 1;
      A[ ind_diag + size ] = 0;
    }
    if ( state_val == 2 ) {
      A[ ind_diag ]        = 0;
      A[ ind_diag + size ] = 1;
    }
    if ( state_val == 3 ) {
      A[ ind_diag ]        = 1;
      A[ ind_diag + size ] = 1;
    }
  }
  else { 
    // last diagonale index : size * 2 - 2
    int norm_ind_diag = ind_diag + 1;
    if ( state_val == 0 ) {
      A[ norm_ind_diag ]        = 0;
      A[ norm_ind_diag - size ] = 0;
    }
    if ( state_val == 1 ) {
      A[ norm_ind_diag ]        = 1;
      A[ norm_ind_diag - size ] = 0;
    }
    if ( state_val == 2 ) {
      A[ norm_ind_diag ]        = 0;
      A[ norm_ind_diag - size ] = 1;
    }
    if ( state_val == 3 ) {
      A[ norm_ind_diag ]        = 1;
      A[ norm_ind_diag - size ] = 1;
    }

    // if ( ind_diag <= 48 ) {
    //   cout << "ind: " << ind_diag << endl;
    //   cout << "A val: " << A[ ind_diag ] << " " << 
    // 	A[ ind_diag - size ] << endl;
    // }
  }

  if ( ( state_val < 0 ) or ( 3 < state_val ) ) {
    cout << "ERROR: Serious matter!" << endl;
  }
  // cout << "A[ " << ind_diag << " ]: " << A[ ind_diag ] <<
  //   " A[ " << size + ind_diag << " ]: " << A[ size + ind_diag ] << endl;
}

int next_state( int * diags_state, int ind_diag, 
		unsigned int * A, int size ) {
  if ( ind_diag < 0 )
    return ind_diag;

  if ( diags_state[ ind_diag ] == 3 ) {
    reset_state( diags_state, ind_diag, A, size );
    return next_state( diags_state, ind_diag - 1, A, size );
  }
  else { 
    diags_state[ ind_diag ]++;
    update_A( diags_state[ ind_diag ], ind_diag, A, size );
    return ind_diag;
  }
}


int next_state_backward( int * diags_state, int ind_diag, 
			 unsigned int * A, int size ) {
  if ( size * 2 - 1 < ind_diag )
    return ind_diag;

  if ( diags_state[ ind_diag ] == 3 ) {
    reset_state( diags_state, ind_diag, A, size );
    return next_state_backward( diags_state, ind_diag + 1, A, size );
  }
  else { 
    diags_state[ ind_diag ]++;
    update_A( diags_state[ ind_diag ], ind_diag, A, size );
    return ind_diag;
  }
}


inline void set_and_print_a( unsigned int* a, unsigned int* A, int size ) {
  for (int i = 0; i < size / 16; i++) {
    a[i] = 0;
  }
  for (int i = 0 ; i < ( size / 16 ) * size; i++) {
    set_i_bit( a, i, A[ i ] ) ;
  }
  for(int i = 0; i < size / 16; i++) {
    cout << hex << a[i] << " ";
  }
  cout << dec << endl; 
}

inline void print_a( unsigned int* a, int size ) {
  for(int i = size / 16 -1; 0 <= i; --i) {
    printf("%08x ", a[i]);
  }
  cout << dec << endl; 
}

inline void set_a( unsigned int* a, unsigned int* A, int size ) {
  for (int i = 0; i < size / 16; i++) {
    a[i] = 0;
  }
  for (int i = 0 ; i < ( size / 16 ) * size; i++) {
    set_i_bit( a, i, A[ i ] ) ;
  }
  // for(int i = 0; i < size / 16; i++) {
  //   cout << hex << a[i] << " ";
  // }
  // cout << dec << endl; 
}


inline bool full_check_A ( unsigned int* A, unsigned int* B, 
			   int ind_diag, int size ) {
  bool res = true;
  for ( ind_diag = size ; ind_diag < size * 2 ; ind_diag++ ) {
    // cout << "full_check: res: " << res << endl;
    res &= check_diag_val( B, ind_diag, A, size );
  }

  return res ;
}


int build_first_half_A( unsigned int* A, unsigned int* B, 
			int* diags_state, int ind_diag, 
			int size, int first_half_limit ) {
  // while (ind_diag < ( size / 2 ) ) {
  while (ind_diag < first_half_limit ) {
    // cout << "ind_diag: " << ind_diag << " ";
    // set_and_print_a( a_, A, size );
    // cout << "A[ " << ind_diag << " ]: " << A[ ind_diag ] <<
    //   " A[ " << size + ind_diag << " ]: " << A[ size + ind_diag ] << endl;

    if ( ind_diag < 0 ) {
      // cout << "in while: " << ind_diag << "\n";
      // set_and_print_a( a_, A, size );
      break;
    }

    // cout << "check_diag: " << check_diag_val( B, ind_diag, A, size ) << endl;
    // cout << "check_sub_diag: " << check_sub_diag_val( B, ind_diag, A, size ) << endl;

    // if ( check_diag_val( B, ind_diag, A, size ) &&
    // 	 check_sub_diag_val( B, ind_diag, A, size ) ) {
    if ( check_diag_val( B, ind_diag, A, size ) ) {
      ind_diag++;
      // cout << "ind_diag: " << ind_diag << endl;
    }
    else {
      ind_diag = next_state( diags_state, ind_diag, A, size );
    }

    // cout << "in while: " << ind_diag << "\n";
  }

  // cout << "end while" << endl;
  // set_and_print_a( a_, A, size );
  return ind_diag;
}



int build_first_half_A_constraint( unsigned int* A, unsigned int* B, 
				   int* diags_state, int ind_diag, 
				   int size, 
				   std::vector<unsigned int> second_half ) {

  std::vector< std::set<unsigned int> > potential_index( size / 2 );

  int index_potential;
  unsigned tmp_res[2] = {0, 0};
  unsigned int* a = new unsigned int[size / 16];
  unsigned int* b = new unsigned int[size / 16];

  std::set<unsigned int>::iterator it;
  int norm_ind_diag;

  set_a( b, B, size );

  while ( ind_diag < size ) {
    index_potential = ind_diag - size / 2;

    if ( ind_diag < 0 ) {
      break;
    }

    if ( ind_diag < size / 2 ) {
      if ( check_diag_val( B, ind_diag, A, size ) ) {
	++ind_diag;
      }
      else {
	ind_diag = next_state( diags_state, ind_diag, A, size );
      }
    }
    else {
      if ( check_diag_val( B, ind_diag, A, size ) ) {
	set_a( a, A, size );
	potential_index[ index_potential ].clear();
	
	if( ind_diag == size / 2 ) {
	  for ( int i = 0 ; i < second_half.size() / 2 ; ++i ) {
	    tmp_res[ 0 ] = second_half[ i * 2 ];
	    tmp_res[ 1 ] = second_half[ i * 2 + 1 ];

	    if ( ( A[ ind_diag ] == 
		   get_i_bit( tmp_res, size, ind_diag ) ) && 
		 ( A[ ind_diag + size ] == 
		   get_i_bit( tmp_res, size, ind_diag + size ) ) ) {

	      tmp_res[ 0 ] |= a[ 0 ];
	      tmp_res[ 1 ] |= a[ 1 ];

	      if ( check_diag_val_binary( b, 
					  size - 1 + ( size - 1 - ind_diag ) + 1,
					  tmp_res, size ) ) {
		potential_index[ index_potential ].insert( i );
	      }
	      else { 
		continue;
	      }

	    }
	    else {
	      continue;
	    }
	  }
	}

	else {
	  for ( it  = potential_index[ index_potential - 1].begin(); 
		it != potential_index[ index_potential - 1].end(); 
		++it ) {
	    tmp_res[ 0 ] = second_half[ *it * 2 ];
	    tmp_res[ 1 ] = second_half[ *it * 2 + 1 ];

	    if ( ( A[ ind_diag ] == 
		   get_i_bit( tmp_res, size, ind_diag ) ) && 
		 ( A[ ind_diag + size ] == 
		   get_i_bit( tmp_res, size, ind_diag + size ) ) ) {

	      tmp_res[ 0 ] |= a[ 0 ];
	      tmp_res[ 1 ] |= a[ 1 ];

	      if ( check_diag_val_binary( b, 
					  size - 1 + ( size - 1 - ind_diag ) + 1,
					  tmp_res, size ) ) {
		potential_index[ index_potential ].insert( *it );
	      }
	      else { 
		continue;
	      }

	    }
	    else {
	      continue;
	    }
	  }
	}
	
	// cerr << "pass " << index_potential << " " <<
	//   potential_index[ index_potential ].size() << endl;
	
	if ( !potential_index[ index_potential ].empty() )
	  ++ind_diag;
	else
	  ind_diag = next_state( diags_state, ind_diag, A, size );
	// cout << "ind_diag: " << ind_diag << endl;
      }
      else {
	ind_diag = next_state( diags_state, ind_diag, A, size );
      }
    }
    // cout << "in while: " << ind_diag << "\n";
  }
  // while ( ind_diag < size )

  // cout << "end while" << endl;
  // set_and_print_a( a_, A, size );
  return ind_diag;
}

int build_second_half_A( unsigned int* A, unsigned int* B, 
			 int* diags_state, int ind_diag, 
			 int size, int second_half_limit ) {

  while ( second_half_limit < ind_diag  ) {
    // if ( 62 <= ind_diag ) {
    //   cout << "ind_diag: " << ind_diag << endl;
    // }

    if ( size * 2 - 1 -1 < ind_diag ) {
      break;
    }

    if ( check_diag_val( B, ind_diag, A, size ) ) {
      ind_diag--;
    }
    else {
      ind_diag = next_state_backward( diags_state, ind_diag, A, size );
    }
  }

  return ind_diag;
}



int build_A( unsigned int* A, unsigned int* B, 
	     int* diags_state, int ind_diag, 
	     int size ) {
  while (ind_diag < size) {
    // cout << "ind_diag: " << ind_diag << " ";
    // set_and_print_a( a_, A, size );
    // cout << "A[ " << ind_diag << " ]: " << A[ ind_diag ] <<
    //   " A[ " << size + ind_diag << " ]: " << A[ size + ind_diag ] << endl;

    if ( ind_diag < 0 ) {
      // cout << "in while: " << ind_diag << "\n";
      set_and_print_a( a_, A, size );
      break;
    }

    // cout << "check_diag: " << check_diag_val( B, ind_diag, A, size ) << endl;
    // cout << "check_sub_diag: " << check_sub_diag_val( B, ind_diag, A, size ) << endl;

    // if ( check_diag_val( B, ind_diag, A, size ) &&
    // 	 check_sub_diag_val( B, ind_diag, A, size ) ) {
    if ( check_diag_val( B, ind_diag, A, size ) ) {
      ind_diag++;
      // cout << "ind_diag: " << ind_diag << endl;
    }
    else {
      if ( B[ ind_diag ] == 3 ) {
	reset_state( diags_state, ind_diag, A, size );
	ind_diag = next_state( diags_state, ind_diag - 1, A, size );
      }
      else {
	ind_diag = next_state( diags_state, ind_diag, A, size );
      }
    }

    // cout << "in while: " << ind_diag << "\n";
  }

  // cout << "end while" << endl;
  // set_and_print_a( a_, A, size );
  return ind_diag;
}


int get_A( unsigned int* A, unsigned int* B, 
	   int* diags_state, int ind_diag, 
	   int size, unsigned int* a, 
	   long unsigned int &counter ) {
  bool full_check = false;

  while ( ! full_check ) {
    ind_diag = build_A( A, B, diags_state, ind_diag, size );
    if ( ind_diag < 0 )
      return ind_diag;

    // set_and_print_a( a, A, size );
    full_check = full_check_A ( A, B, ind_diag, size );
    if ( full_check ) {
      // set_and_print_a( a, A, size );
      return ind_diag;
    }

    ind_diag = next_state( diags_state, size - 1, A, size );
    if ( ind_diag < 0 )
      return ind_diag;

    counter++;
    if ( !( counter % 10000000 ) )
      cout << "counter: " << counter << endl;
  }
}

int main()
{
  int size;
  int size_B;
  cin >> size;
 
  size_B = size * 2;

  unsigned int* a = new unsigned int[size / 16]; // <- output tab to encrypt
  unsigned int* b = new unsigned int[size / 16]; // <- input tab
  unsigned int* A = new unsigned int[size_B]; // <- binary version of input tab
  unsigned int* B = new unsigned int[size_B]; // <- binary version of input tab 

  int* diags_state = new int[size * 2 - 1];

  unsigned int tmp_a_first[2];
  unsigned int tmp_a_second[2];
  unsigned int tmp_res[2];

  a_ = a;

  for (int i = 0; i < size / 16; i++) {   // Read size / 16 integers to a
    cin >> hex >> b[i];
    // cout << a[i] << "\n\n" ;
    // cout << sizeof( a[i] ) << "\n\n" ;
  }

  for (int i = 0; i < size_B ; i++) {
    B[i] = 0;
  }
  for (int i = 0; i < size * 2 ; i++) {
    diags_state[i] = 0;
  }

  // B : donne les contraintes sur la diagonale
  for (int i = 0; i < size_B ; i++) {
    B[ i ] = get_i_bit( b, size, i );
  }
  print_tab( B, size_B );

  // int first_half_limit = size / 2 + 1;
  int first_half_limit = size / 2;
  // int second_half_limit = size * 2 - 1 - ( size / 2 - 1 ) - 1;
  int second_half_limit = size * 2 + 1 - size / 2 - 1 -1 -1;
  // int second_half_limit = size * 2 - 1 - ( size / 2 - 1 ) ;

  long unsigned int counter = 0;

  int ind_diag;

  std::vector<unsigned int> first_half;
  std::vector<unsigned int> second_half;

  std::vector<unsigned int> res;

  for (int i = 0; i < size_B ; i++) {
    A[i] = 0;
  }
  for (int i = 0; i < size * 2 ; i++) {
    diags_state[i] = 0;
  }
  ind_diag = 0;
  // unsigned int tmp[2];
  while ( build_first_half_A( A, B, diags_state, ind_diag, 
			      size, first_half_limit ) == first_half_limit ) {
    // set_and_print_a( a, A, size );
    set_a( a, A, size );
    first_half.push_back( a[0] );
    first_half.push_back( a[1] );
    ind_diag = next_state( diags_state, first_half_limit - 1, A, size );
  }

  cout << first_half.size() << endl ;

  for (int i = 0; i < size_B ; i++) {
    A[i] = 0;
  }
  for (int i = 0; i < size * 2 ; i++) {
    diags_state[i] = 0;
  }
  ind_diag = size * 2 - 1 - 1;
  while ( build_second_half_A( A, B, diags_state, ind_diag, 
			       size, second_half_limit ) == second_half_limit ) {
    // set_and_print_a( a, A, size );
    set_a( a, A, size );
    second_half.push_back( a[0] );
    second_half.push_back( a[1] );
    ind_diag = next_state_backward( diags_state, second_half_limit + 1, A, size );
  }

  cout << second_half.size() << endl ;

  cout << ( second_half.size() / 2 ) * ( second_half.size() / 2 ) << endl ;

  ind_diag = 0;
  for (int i = 0; i < size * 2 ; i++) {
    diags_state[i] = 0;
  }

  while( build_first_half_A_constraint( A, B, diags_state, ind_diag, 
					size, second_half ) == size ) {
    // if ( full_check_A( A, B, ind_diag, size ) )
    set_and_print_a( a, A, size );
    ind_diag = next_state( diags_state, size - 1, A, size );
  }

  // while ( get_A( A, B, diags_state, ind_diag, size, a, counter ) == size ) {
  //   set_and_print_a( a, A, size );
  //   ind_diag = next_state( diags_state, size - 1, A, size );

  exit(0);

  // for ( std::vector<unsigned int>::iterator it_first = first_half.begin() ;
  // 	it_first != first_half.end() ;
  // 	++it_first ) {

  //     tmp_a_first[ 0 ] = *it_first;
  //     ++it_first;
  //     tmp_a_first[ 1 ] = *it_first;

  //     print_a( tmp_a_first, size );
  // }

  // for ( std::vector<unsigned int>::iterator it_second = second_half.begin() ;
  // 	it_second != second_half.end() ;
  // 	++it_second ) {

  //     tmp_a_second[ 0 ] = *it_second;
  //     ++it_second;
  //     tmp_a_second[ 1 ] = *it_second;

  //     print_a( tmp_a_second, size );
  // }

  // exit( 1 );

  bool valid = true;

  counter = 0;

  for ( std::vector<unsigned int>::iterator it_first = first_half.begin() ;
	it_first != first_half.end() ;
	++it_first ) {

    tmp_a_first[ 0 ] = 0;
    tmp_a_first[ 1 ] = 0;

    tmp_a_first[ 0 ] = *it_first;
    ++it_first;
    tmp_a_first[ 1 ] = *it_first;

    ++counter;
    if ( counter % int ( 0.01 * first_half.size() ) == 0 ) {
      cout << "counter: " << counter << endl;
      print_a( tmp_a_first, size );
    }

    for ( std::vector<unsigned int>::iterator it_second = second_half.begin() ;
	  it_second != second_half.end() ;
	  ++it_second ) {

      // tmp_a_second[ 0 ] = 0;
      // tmp_a_second[ 1 ] = 0;

      tmp_a_second[ 0 ] = *it_second;
      ++it_second;
      tmp_a_second[ 1 ] = *it_second;

      // print_a( tmp_a_second, size );

      // tmp_res[ 0 ] = 0;
      // tmp_res[ 1 ] = 0;

      tmp_res[ 0 ] = tmp_a_first[ 0 ] ^ tmp_a_second[ 0 ];
      tmp_res[ 1 ] = tmp_a_first[ 1 ] ^ tmp_a_second[ 1 ];

      if ( tmp_a_first[ 1 ] == 21241 ) { // 52f9
	print_a( tmp_res, size );
      }

      valid = true;

      for ( int i = size / 2 - 1 +1; 
	    i < size * 2 - ( size / 2 ) - 1; ++i ) {
	    // i < size * 2 - 1 - ( size / 2 - 1 ) - 1 +1; ++i ) {
	valid &= check_diag_val_binary( b, i, 
					tmp_res, size );
	if ( ! valid )
	  break;
      }

      // cout << "pass" << endl;
      if ( valid ) {
	print_a( tmp_res, size );
	res.push_back( tmp_res[0] );
	res.push_back( tmp_res[1] );
      }

      // counter++;
      // if ( !( counter % 10000 ) )
      // 	cout << "counter: " << counter << endl;
    }
  }

  cout << "nb sol: " << res.size() / 2 << endl;

  for ( std::vector<unsigned int>::iterator it_res = res.begin() ;
	it_res != res.end() ;
	++it_res ) {

    tmp_res[0] = *it_res;
    ++it_res;
    tmp_res[1] = *it_res;

    print_a( tmp_res, size );
  }

  exit( 1 );

  // More than 2 140 000 000 tested combinations
  while ( get_A( A, B, diags_state, ind_diag, size, a, counter ) == size ) {
    set_and_print_a( a, A, size );
    ind_diag = next_state( diags_state, size - 1, A, size );
  }

  cout << "No more solutions !" << endl;

  // for (int i = 0; i < size; i++)
  //   for (int j = 0; j < size; j++)
  //     // b[(i + j) / 32] ^= ( (a[i / 32] >> (i % 32)) &
  //     b[(i + j) / 32] ^= ( (a[i >> 5] >> (i % 32)) &
  // 	  // (a[j / 32 + size / 32] >> (j % 32)) & 1 ) << ((i + j) % 32);   
  // 			   (a[(j >> 5) + (size >> 5)] >> (j % 32)) & 1 ) << ((i + j) % 32);   
  // // Magic centaurian operation
 
  // set_and_print_a( a, A, size );

  return 0;
}
