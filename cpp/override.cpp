// kate: hl C++11;
#include <iostream>

struct base1
{
  virtual void foo() = 0;
};

struct derived1 : base1
{
  virtual void foo() override
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

struct base2
{
  virtual void foo()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };
};

struct derived2 : base2
{
  virtual void foo()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

int main()
{
  base1* override1 = new derived1();
  override1->foo();

  base2* override2 = new derived2();
  override2->foo();

  base2* toto = new base2();
  toto->foo();
  //
  // error: invalid conversion from ‘base2*’ to ‘derived2*’
  // derived2* assum_derived = toto;
  // assum_derived->foo();

  return 0;
}

// g++ -o override override.cpp && ./override
