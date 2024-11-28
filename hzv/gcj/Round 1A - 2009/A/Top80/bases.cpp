#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <vector>
using namespace std;

vector<vector<int> > words;
#define fu(i,m,n) for(int i=m; i<n; i++)

int base[11][20000000];
int first[2048];

int sumd(int n, int base) {
	int ret=0;
	while(n) {
		int nn=n%base;
		ret+=nn*nn;
		n/=base;
	}
	return ret;
}

int NN=20000000;

int isbase(int n, int b) {
	if(n==1) return 1;
	//cout << n << "," << b << endl;
	int& ret=base[b][n];
	if(ret!=-1) return ret;
	ret=0;
	return ret=isbase(sumd(n,b),b);
}

int main(void) {
	memset(base,-1,sizeof(base));
	/*fu(b,2,11) {
		base[b][1]=1;
		fu(j,b,NN) {
			int k=j;
			while(base[b][k]==-1) {
				base[b][k]=0;
				k=sumd(k,b);
			}
			if(base[b][k]) {
				base[b][j]=1;
				cout << j << " ";
			}
		}
		cout << endl;
	}*/
	fu(j,2,NN) {
		int msk=0;
		fu(b,2,11) if(isbase(j,b)) msk|=(1<<b-2);
		//cout << j << " " << msk << endl;
		if(!first[msk]) {
			first[msk]=j;
		}
	}
	fu(j,0,2000) if(!first[j]) {
		first[j]=100000000;
		fu(k,0,2000) if(first[j|k])
			first[j]=min(first[j],first[j|k]);
	}
	int T;
	cin >> T;
	string s;
	getline(cin,s);
	fu(t,0,T) {
		int msk=0;
		string s;
		getline(cin,s);
		istringstream is(s);
		int d;
		while(is>>d) msk|=(1<<d-2);
		cout << "Case #" << t+1 << ": " << first[msk] << endl;
	}
}
