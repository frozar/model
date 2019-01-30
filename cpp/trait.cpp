#include <iostream>

template< bool b > 
struct algorithm_selector { 
  template< typename T > 
  static void implementation( T& object ) 
  { 
    std::cout << "AnyObject: default_implementation() call!" << std::endl;
  } 
};

template<> 
struct algorithm_selector< true > { 
  template< typename T > 
  static void implementation( T& object )   { 
    object.optimised_implementation(); 
    std::cout << "algorithm_selector< true >::implementation() call!" << std::endl;
  } 
};

class ObjectA { 
};

class ObjectB { 
public: 
  void optimised_implementation() { 
    std::cout << "ObjectB: Inside optimised_implementation()!" << std::endl;
  } 
};

template< typename T >
struct supports_optimised_implementation {
  static const bool value = false; 
};

template<> 
struct 
supports_optimised_implementation< ObjectB > { 
  static const bool value = true; 
};

template< typename T > 
void algorithm( T& object ) { 
  algorithm_selector< supports_optimised_implementation< T >::value >::implementation(object); 
}

int main(int argc, char ** argv) {
  ObjectA a;
  std::cout << "obj a: call to algorithm()!" << std::endl;
  algorithm( a ); 
  std::cout << std::endl;

  ObjectB b; 
  std::cout << "obj b: call to algorithm()!" << std::endl;
  algorithm( b ); 
  return 0; 
}

// g++ -o trait trait.cpp && ./trait
