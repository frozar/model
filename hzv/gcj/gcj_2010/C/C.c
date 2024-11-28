#include<stdio.h>
#include<string.h>
#include<math.h>

unsigned int T, tmp, R, tmpr, k, N, somme, S;
unsigned int dat[1024], tmpd;
unsigned int i, j;

int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");


  fscanf(fp, "%d", &T);
  
  for(tmp=0; tmp<T; tmp++){
    fscanf(fp, "%d %d %d", &R, &k, &N);
    S= 0;
    memset(dat, 0, sizeof(dat));
    for(i=0; i<N; i++){
      fscanf(fp, "%d", &tmpd);
      dat[i]= tmpd;
    }    
    i= 0; j= 0;    
    for(tmpr= 0; tmpr<R; tmpr++){
      somme= 0;
      while(somme<=k && (j-i)<N ){
	somme= somme + dat[(j%N)];
	j++;
      }
      if (somme > k){
	j--;
	somme= somme-dat[(j%N)];
      }
      
      i= j;
      S= S+somme;
      fprintf(stderr, "la valeur de S : %d\n", S);
    }
    fprintf(ofp, "Case #%d: %d\n", tmp+1, S);
  }

  return 1;
}
