#include<stdio.h>
#include<memory.h>
#include<string.h>
int N, C, I, T;
int L[2048];
int i, j;

int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");


  fscanf(fp, "%d", &N);
  
  for (T=0; T<N; T++){
    fscanf(fp, "%d", &C);
    fscanf(fp, "%d", &I);
    memset(L, 0, sizeof(L));

    // initialisation du tableau
    for (i=0; i<I; i++){
      fscanf(fp, "%d", &L[i]);
    }

    i=0; j=i;
    for(i=0; i<I; i++){
      for(j=i+1; j<I; j++){
	if (C == L[i]+L[j]){
	  fprintf(ofp, "Case #%d: %d %d\n", T+1, i+1, j+1);
	}
      }
    }
  }

  return 1;
}
