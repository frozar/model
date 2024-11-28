#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
using namespace std;

#define all(c) ((c).begin()), ((c).end()) 
#define iter(c) __typeof((c).begin())
#define present(c, e) ((c).find((e)) != (c).end()) 
#define cpresent(c, e) (find(all(c), (e)) != (c).end())
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define tr(c, i) for (iter(c) i = (c).begin(); i != (c).end(); ++i)
#define pb(e) push_back(e)
#define mp(a, b) make_pair(a, b)

typedef long long ll;

int readint() { 
  int i, j, s;
  while (!isdigit(i = getchar()) && i != '-');
  if (i == '-') { s = -1; j = 0; }
  else { s = 1; j = i - '0'; }
  while (isdigit(i = getchar())) j = ((j << 1) + (j << 3) + (i - '0'));
  return j * s;
}

int doit(int n, int b) {
  int r = 0;
  while (n > 0) {
    r += (n % b) * (n % b);
    n /= b;
  }
  return r;
}

bool ishappy(int n, int b) {
  set<int> vis;
  while (n != 1) {
    if (vis.count(n)) return false;
    vis.insert(n);
    n = doit(n, b);
  }
  return true;
}

int main() {
  int cases;
  scanf("%d", &cases);
  rep (ca, cases) {
    scanf(" ");
    char line[1024];
    gets(line);
    stringstream ss(line);
    int t;
    vector<int> B;
    while (ss >> t) B.pb(t);

    for (int i = 2; ; i++) {
      rep (j, B.size()) {
        if (!ishappy(i, B[j])) goto ng;
      }
      printf("Case #%d: %d\n", ca + 1, i);
      break;
    ng:
      ;
    }
  }
}
