#include <iostream>

int main(int argc, char ** argv) {
  const char *const * argv_cp_0 = argv;
  const char * * argv_cp_1 = argv;
  return 0;
}

// g++ -O0 -o const_convertion const_convertion.cpp && ./const_convertion toto tata
