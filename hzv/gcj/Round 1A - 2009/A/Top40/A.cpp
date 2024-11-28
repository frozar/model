
//Written by Alex Hamed Ahmadi - alex@hamedahmadi.com

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>

using namespace std;

bool baseactive[20];

int getsum(int x, int base) {
  int sum=0;
  while (x) {
    int d=x%base;
    x/=base;
    sum+=d*d;
  }
  return sum;
}

int ish[20000000][20];

bool ishappy(int x, int base) {
  if (x==1) return ish[x][base]=1;
  if (ish[x][base]!=-1) return ish[x][base];
  ish[x][base]=0;
  int y=getsum(x, base);
  if (ishappy(y,base)) {
    return ish[x][base]=1;
  }

  return 0;
}

string s;

int main() {
  memset(ish, -1, sizeof ish);

  int nt;
  getline(cin, s);
  nt = atoi(s.c_str());

  for (int T=1;T<=nt;T++) {
    memset(baseactive, 0, sizeof baseactive);
    getline(cin, s);
    istringstream is(s);
    int x;
    while (is>>x) baseactive[x]=1;

    int ans=0;
    for (ans=2;;ans++) {
      for (int i=2;i<=10;i++) {
	if (baseactive[i] && !ishappy(ans, i)) goto fail;
	//if (!ishappy(ans, i)) goto fail;
      }
      break;
      fail:;
    }
    printf("Case #%d: %d\n", T, ans);
  }

  return 0;
}
