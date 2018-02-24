#include <iostream>

// The only possible prototype to ensure that argv
// will not be modified in any way
void
toto (const int & argc, char const *const *const & argv)
{
  std::cout << "In function, look at argv again: " << argv[argc - 1] << std::endl;
}

int main(int argc, char ** argv) {
  std::cout << "Look at argv: " << argv[argc - 1] << std::endl;

  // The const convertion are possible in this way:
  // https://stackoverflow.com/questions/15943721/cant-convert-from-char-to-const-char#answer-15944260
  // https://www.codeproject.com/Articles/7042/How-to-interpret-complex-C-C-declarations
  const char *const *const argv_cp_0 = argv;
  char *const *const argv_cp_1 = argv;
  char * *const argv_cp_2 = argv;

  toto (argc, argv);
  toto (argc, argv_cp_0);
  toto (argc, argv_cp_1);
  toto (argc, argv_cp_2);
  return 0;
}

// g++ -o argv argv.cpp && ./argv toto tata
