#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
  std::cout << fs::path("/foo/bar.txt").extension() << std::endl;

  return 0;
}

// g++ -std=c++17 -o filesystem filesystem.cpp && ./filesystem
