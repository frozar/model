#include<stdio.h>
#include<string.h>
#include<math.h>

int T, N, K, tmp;

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
    fscanf(fp, "%d%d", &N, &K);
    
    if (K == pow(2,N)-1 || (K != 0 && ( (int)(K-pow(2,N)+1) %(int)(pow(2,N)) == 0)))
      fprintf(ofp, "Case #%d: ON\n", tmp+1);
    else
      fprintf(ofp, "Case #%d: OFF\n", tmp+1);
  }

  return 1;
}
