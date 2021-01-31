#include <stdio.h>
#include <stdlib.h>

int main( ) {
  printf( "One bit ?: %d\n", !!( 1 & 5 ) ) ;

  return EXIT_SUCCESS ;
}

// gcc -o binary binary.c && ./binary
