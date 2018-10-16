#include <iostream>

int main(int argc, char ** argv) {
  std::cout << "sizeof(unsigned int)      : " << sizeof(unsigned int) << std::endl;
  std::cout << "sizeof(unsigned long)     : " << sizeof(unsigned long) << std::endl;
  std::cout << "sizeof(unsigned long long): " << sizeof(unsigned long long) << std::endl;
  return 0;
}

// g++ -o type_size type_size.cpp && ./type_size
