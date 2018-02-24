#include <iostream>

int main(int argc, char ** argv) {
  const char *const * argv_cp_0 = argv;
  const char *const * dummy_var;

  std::cout << "argv     : " << argv << std::endl;
  std::cout << "argv_cp_0: " << argv_cp_0 << std::endl;
  argv_cp_0 = dummy_var;
  std::cout << "argv_cp_0: " << argv_cp_0 << std::endl;
  argv_cp_0 = NULL;
  std::cout << "argv_cp_0: " << argv_cp_0 << std::endl;

  const char *const *const argv_cp_1 = argv;
  std::cout << "argv_cp_1: " << argv_cp_1 << std::endl;

  // argv_cp_1 = dummy_var;
  // The above line will not compile with the following error message from the compiler
  // const_attempt.cpp: In function ‘int main(int, char**)’:
  // const_attempt.cpp:16:13: error: assignment of read-only variable ‘argv_cp_1’
  //    argv_cp_1 = dummy_var;
  //              ^

  // argv_cp_1 = NULL;
  // The above line will not compile too for exactly the same reason

  // To better understand how to read complex type declaration in C/C++
  // https://www.codeproject.com/Articles/7042/How-to-interpret-complex-C-C-declarations

  return 0;
}

// g++ -O0 -o const_attempt const_attempt.cpp && ./const_attempt toto tata
