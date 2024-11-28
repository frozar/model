#include<stdio.h>
#include<string.h>

int T, N, tmp, i, j, compt;
int A[65536], B[65536];

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
    compt= 0;
    fscanf(fp, "%d", &N);

    for(i=0; i<N; i++){
      fscanf(fp, "%d %d", &A[i], &B[i]);
    }
    
    for(i=0; i<N; i++){
      for(j=i; j<N; j++){
	if( (A[i]<A[j] && B[i]>B[j]) || (A[i]>A[j] && B[i]<B[j]))
	  compt++;
      }
    }

    fprintf(ofp, "Case #%d: %d\n", tmp+1, compt);
  }
  return 1;
}
