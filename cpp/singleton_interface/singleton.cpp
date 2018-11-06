
#include <iostream>

#include "threadsettinginterface.h"

using namespace std;

void foo(void)
{
  ThreadSettingInterface::instance()->SetNbThread(1);
  cout << "foo: nb thread is " << ThreadSettingInterface::instance()->GetNbThread() << '\n';
}

void bar(void)
{
  ThreadSettingInterface::instance()->SetNbThread(2);
  cout << "bar: nb thread is " << ThreadSettingInterface::instance()->GetNbThread() << '\n';
}

int main()
{
  cout << "main: nb thread is " << ThreadSettingInterface::instance()->GetNbThread() << '\n';
  foo();
  bar();

  return 0;
}

// ./compile.sh && valgrind --leak-check=full --show-leak-kinds=all ./singleton 
