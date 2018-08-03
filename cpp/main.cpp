#include <iostream>

void toto () {
  std::cout << "Hello main!" << std::endl;
}

int main(int argc, char ** argv) {
  toto ();

  return 0;
}

// g++ -o main main.cpp && ./main
