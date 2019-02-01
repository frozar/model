#include <iostream>
#include <vector>

class Base
{
public:
  virtual void display() {
    std::cout << "Display from Base class" << std::endl;
  }
};

class Child : public Base
{
public:
  virtual void display() override {
    std::cout << "Display from Child class" << std::endl;
  }
};

template <typename T>
class MyContainer
{
public:
  std::vector<T> m_container;
};

int main(int argc, char ** argv) {
  Base * pObj = new Child;
  pObj->display();
  delete pObj;

  Child c;
  MyContainer<Base> collection;
  collection.m_container.push_back(c);

  return 0;
}

// g++ -o container_issue container_issue.cpp && ./container_issue
