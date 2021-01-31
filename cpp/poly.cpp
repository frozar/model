#include <iostream>
#include <vector>

using namespace std;

struct A
{
  int i;
  A (int i) {this->i = i;}

  void p_A(A ob)
  {
    cout << "In p_A: ob.i: " << ob.i << endl;
  }
};

struct subA : public A
{
  subA (int i) : A(i) {}
};

struct subB : public A
{
  subB (int i) : A(i) {}
};

int main()
{
  subA subA_ob( 3 );
  cout << "subA_ob.i: " << subA_ob.i << endl;
  subA_ob.p_A(subA_ob);

  subB subB_ob( 3 );
  subB_ob.p_A(&subA_ob);
}

// g++ -o poly poly.cpp && ./poly
