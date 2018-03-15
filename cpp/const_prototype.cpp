#include <iostream>

void toto(int & i)
{
  std::cout << "just  i: " << i << std::endl;
}

void tata(const int & i)
{
  std::cout << "const & i: " << i << std::endl;
  toto (i);
}


int main(int argc, char ** argv) {
  int i = 42;
  tata (i);
  return 0;
}

// g++ -O0 -o const_prototype const_prototype.cpp && ./const_prototype toto tata
