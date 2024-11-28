#include<stdio.h>

#define N 100000000

int csN, csK, ans[2048];
char happy[11][N];
int used[N];

inline int DFS(int base, int x)
{
	if(happy[base][x]) return happy[base][x];
	if(used[x] == base) return -1;
	used[x] = base;
	int s = 0, m = x, t;
	while(m > 0)
	{
		t = m % base;
		s += t * t;
		m /= base;
	}
	happy[base][x] = DFS(base, s);
	return happy[base][x];
}

int main()
{
	int i, j, k;
	for(i = 2; i <= 10; ++i)
	{
		happy[i][0] = -1;
		happy[i][1] = 1;
		for(j = 2; j < N; ++j)
			if(happy[i][j] == 0)
				DFS(i, j);
	}
	for(i = 4; i < 2048; i += 4)
	{
		for(j = 2; j < N; ++j)
		{
			for(k = 2; k <= 10; ++k)
				if((i&(1<<k)) && happy[k][j] != 1) break;
			if(k > 10) break;
		}
		if(j >= N)
			fprintf(stderr, "not found %X\n", i);
		else printf("ans[%4d] = %d;\n", i, j);
		ans[i] = j;
	}
	return 0;
	scanf("%d", &csN);
	for(csK = 1; csK <= csN; ++csK)
	{
	}
}
