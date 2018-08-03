template<typename TData>
class Foo
{
public:
  typedef Foo Self;
  typedef TData Data;
  Foo();
  // just a comment
  Data getData(int i, char const* s);
  float simple(float f, int i)
  {
    return f;
  }
  float d_simple(float f, int i, int j)
  {
    return f;
  }
private:
  /// my precious data
  Data m_data;
};

// typedef Foo<int> IntFoo;

int main() {
  Foo<int> bar();
  bar.simple(3.14, 1);

  Foo<int> * pBar0;
  Foo<int> * pBar = new Foo<int>();
  pBar->simple(3.14, 2);
  float f1 = 3.14;
  float * pf = &f1;
  pBar->simple(*pf, 3);
  pBar->d_simple(*pf, 4, 5);
  return 0;
}
