// Powered by FTH

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <functional>
#include <utility>
#include <numeric>
#include <complex>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <cassert>
using namespace std;

#define REP(i,n) for(int i = 0; i < (int)(n); i++)
#define FOR(i,c) for(__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define ALLOF(c) ((c).begin()), ((c).end())

bool isHappyInBase(int n, int b) {
    set<int> visited;
    while(visited.insert(n).second) {
        if (n == 1)
            return true;
        int m = 0;
        while(n > 0) {
            int k = n % b;
            m += k * k;
            n /= b;
        }
        n = m;
    }
    return false;
}

int solve(vector<int> v) {
    for(int k = 2; ; k++) {
        bool ok = true;
        REP(i, v.size()) {
            int b = v[i];
            if (!isHappyInBase(k, b)) {
                ok = false;
                break;
            }
        }
        if (ok)
            return k;
    }
}

int main() {

    /*
    for(int a = 2; a <= 10; a++) {
        for(int b = a+1; b <= 10; b++) {
            for(int c = b+1; c <= 10; c++) {
                vector<int> v;
                v.push_back(a);
                v.push_back(b);
                v.push_back(c);
                cout << a << "," << b << "," << c << ": " << solve(v) << endl;
            }
        }
    }
    return 0;
    */

    int nCases;
    {
        string s;
        getline(cin, s);
        istringstream is(s);
        is >> nCases;
    }

    REP(iCase, nCases) {
        string s;
        getline(cin, s);
        istringstream is(s);
        vector<int> v;
        for(int i; is >> i; )
            v.push_back(i);
        printf("Case #%d: %d\n", iCase+1, solve(v));
    }

    return 0;
}
