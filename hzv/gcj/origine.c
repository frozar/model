#include<stdio.h>
#include<string.h>

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
    
    

    fprintf(ofp, "Case #%d: %s\n");
    
  }

  
  return 1;
}
