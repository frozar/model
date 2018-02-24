#include <iostream>

// The only possible prototype to ensure that argv
// will not be modified in any way
void
toto (const char *const & s)
{
  std::cout << "TOTO: s (ref): " << s << std::endl;
}

void
tata (const char *const s)
{
  std::cout << "TATA: s (val): " << s << std::endl;
}

int main(int argc, char ** argv) {
  toto (argv[0]);
  toto ("1");
  tata (argv[0]);
  tata ("2");
  return 0;
}

// g++ -o const const.cpp && ./const
