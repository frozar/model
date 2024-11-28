#include<iostream>
#include<set>
#include<string>
#include<sstream>

using namespace std;

int _next(int n, int x) {
    int a = 0;
    while(n) {
        int m = n % x;
        a += m * m;
        n /= x;
    }
    return a;
}

bool b[12][12000000];

bool happy_small(int x, int base) {
    int y = x;
    set<int> s;
    s.insert(x);
    while(1) {
        x = _next(x, base);
        if(x < y) return b[base][x];
        if(s.find(x) != s.end()) return false;
        s.insert(x);
    }
    return false;
}
bool happy(int x, int base) {
    if(x <= base*base) return happy_small(x, base);
    int y = x;
    while(1) {
        x = _next(x, base);
        if(x < y) return b[base][x];
    }
    return false;
}

int main() {
    int T;
    string str;

    for(int i=2; i<=10; i++) {
        b[i][1] = true;
        for(int j=2; j<11820000; j++) {
            b[i][j] = happy(j, i);
        }
    }

    cin >> T;
    getline(cin, str);

    for(int t=1; t<=T; t++) {
        getline(cin, str);
        int a[10], n=0;
        istringstream sin(str);
        while(sin >> a[n++]);
        n--;
        for(int i=2; ; i++) {
            bool bb = true;
            for(int j=0; j<n; j++) {
                bb = bb and b[a[j]][i];
                if(not bb) break;
            }
            if(bb) {
                cout << "Case #" << t << ": " << i << endl;
                break;
            }
        }
    }
}

