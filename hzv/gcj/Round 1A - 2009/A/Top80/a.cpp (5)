
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <list>
#include <cmath>
#include <complex>
#include <numeric>
#include <cassert>
using namespace std;

#define REP(i,n) for(int i = 0; i < (int)(n); ++i)
#define FOR(i,s) for(__typeof((s).begin()) i = (s).begin(); i != (s).end(); ++i)
#define ALLOF(s) ((s).begin()), ((s).end())

typedef long long integer;


bool isHappy(integer n, int base) {
  set<integer> visited;
  while(visited.count(n) == 0){
    visited.insert(n);
    integer next = 0;
    while(n != 0){
      int mod = n % base;
      next += mod * mod;
      n /= base;
    }
    n = next;
  }
  
  return n == 1;
}

integer solve(const vector<int>& bases) {
  integer res = 2;
  int n = bases.size();

  for(;; ++res){
    bool ok = true;
    REP(i, n){
      int base = bases[i];
      if(!isHappy(res, base)){
	ok = false;
	break;
      }
    }

    if(ok)
      break;
  }


  return res;
}


int main(void) {
  int nCases;
  string line;
  getline(cin, line);
  {
    istringstream sin(line);
    sin >> nCases;
  }
  
  
  REP(iCase, nCases) {
    getline(cin, line);
    istringstream sin(line);
    vector<int> bases;
    for(;;){
      int base;
      sin >> base;
      if(!sin)
	break;
      bases.push_back(base);
    }
    
    integer res = solve(bases);
    
    cout << "Case #" << (iCase + 1) << ": " << res << endl;
  }
  
  return 0;
}
