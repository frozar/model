// Documentation link:
// https://sourcemaking.com/design_patterns/singleton/cpp/1
// https://codereview.stackexchange.com/questions/197486/singleton-class-and-correct-way-to-access-it-in-c?newreg=c163b1884cf34c9dbb697f41e91ede9e

#include <iostream>

using namespace std;

class GlobalClass
{
  int m_value;
  GlobalClass(int v = 0)
  {
    m_value = v;
  }

  ~GlobalClass();
public:
  int get_value()
  {
    return m_value;
  }
  void set_value(int v)
  {
    m_value = v;
  }

  static GlobalClass& instance()
  {
    static GlobalClass instance;
    return instance;
  }
};

void foo(void)
{
  GlobalClass::instance().set_value(1);
  cout << "foo: global_ptr is " << GlobalClass::instance().get_value() << '\n';
}

void bar(void)
{
  GlobalClass::instance().set_value(2);
  cout << "bar: global_ptr is " << GlobalClass::instance().get_value() << '\n';
}

int main()
{
  cout << "main: global_ptr is " << GlobalClass::instance().get_value() << '\n';
  foo();
  bar();

  return 0;
}

// g++ -o singleton_without_leak singleton_without_leak.cpp && valgrind --leak-check=full --show-leak-kinds=all ./singleton_without_leak
