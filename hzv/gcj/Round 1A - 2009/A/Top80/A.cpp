#include<stdio.h>
#include<string.h>
#include<memory.h>
int n, m;
char buf[1024];
int a[16];
bool chk[1024];
bool happy(int x, int b)
{
	int nxt;
	memset(chk, false, sizeof(chk));
	while(x!=1)
	{
		if(x<1000 && chk[x]) return false;
		if(x<1000) chk[x]=true;
		for(nxt=0;x;x/=b) nxt+=((x%b)*(x%b));
		x=nxt;
	}
	return x==1;
}
int main()
{
	char filename[32];
	char infile[32], outfile[32];
	scanf("%s", filename);
	strcpy(infile, filename); strcpy(outfile, filename);
	strcat(infile, ".in"); strcat(outfile, ".out");
	FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

	int tc, t;
	int i;
	fscanf(fp, "%d\n", &tc);
	for(t=1;t<=tc;t++)
	{
		n=0;
		fgets(buf, 1000, fp);
		for(i=0;buf[i];)
		{
			if(!('0'<=buf[i] && buf[i]<='9')){i++; continue;}
			sscanf(&buf[i], "%d", &a[++n]);
			while('0'<=buf[i] && buf[i]<='9') i++;
		}
		for(m=2;;m++)
		{
			for(i=1;i<=n;i++)
			{
				if(!happy(m, a[i])) break;
			}
			if(i>n) break;
		}
		fprintf(ofp, "Case #%d: %d\n", t, m);
	}
	return 0;
}
