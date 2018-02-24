#include <iostream>
#include <limits>       // std::numeric_limits

int main( ) {
  double r, g, b;
  r = 1.0;
  g = 0.5;
  b = 0.0;
  std::cout << "r: " << static_cast<unsigned char> (r) << std::endl ;
  std::cout << "g: " << static_cast<unsigned char> (g) << std::endl ;
  std::cout << "b: " << static_cast<unsigned char> (b) << std::endl ;
  std::cout << "(int)r: " << (int)static_cast<unsigned char> (r) << std::endl ;
  std::cout << "(int)g: " << (int)static_cast<unsigned char> (g) << std::endl ;
  std::cout << "(int)b: " << (int)static_cast<unsigned char> (b) << std::endl ;
}

// g++ -o static_cast static_cast.cpp && ./static_cast
