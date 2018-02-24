#include <iostream>
#include <stdio.h>

int main()
{
  int a = 12 + 256 + 65536 + (256 * 65536 ) ;
  char b [ 4 ] ;
  b[ 0 ] = * ( reinterpret_cast< char *>( &a ) + 0 ) ;
  b[ 1 ] = * ( reinterpret_cast< char *>( &a ) + 1 ) ;
  b[ 2 ] = * ( reinterpret_cast< char *>( &a ) + 2 ) ;
  b[ 3 ] = * ( reinterpret_cast< char *>( &a ) + 3 ) ;

  printf( "b[ 0 ] : %d\n", b[ 0 ] ) ;
  printf( "b[ 1 ] : %d\n", b[ 1 ] ) ;
  printf( "b[ 2 ] : %d\n", b[ 2 ] ) ;
  printf( "b[ 3 ] : %d\n", b[ 3 ] ) ;
}

