#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define eps 1e-9

#define forn(i, n) for(int i = 0; i < (int)(n); i++)
#define sz(v)((v).size())

#define fi first
#define se second
#define pb push_back

typedef long long ll;
typedef unsigned long long ull;
typedef double dbl;
typedef long double ldbl;

using namespace std;

bool check( int x, int base )
{
  set <int> was;
  while (true)
  {
    int to = 0;
    while (x)
      to += (x % base) * (x % base), x /= base;
    x = to;

    if (was.count(x))
      break;
    was.insert(x);

    if (x == 1)
      return true;
  }
  return false;
}

bool ok( int x, vector <int> & need )
{
  for (int i = 0; i < (int)need.size(); i++)
    if (!check(x, need[i]))
      return false;
  return true;
}

int main( void )
{
  int tn;
  scanf("%d\n", &tn);


  for (int tt = 1; tt <= tn; tt++)
  {
    printf("Case #%d: ", tt);

    vector <int> need;
    char s[500];
    gets(s);
    stringstream ss(s);
    int x;
    while (ss >> x)
      need.push_back(x);
    int st = 2;
    while (!ok(st, need))
      st++;
    printf("%d\n", st);
  }

   
  
  return 0;
}