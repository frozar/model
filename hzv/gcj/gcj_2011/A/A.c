#include<stdio.h>
#include<string.h>
#include<math.h>

int T, N, K;
int tab_0[100], tab_B[100];

int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  fscanf(fp, "%d", &T);
  
  for(int tmp=0; tmp<T; tmp++){
    fscanf(fp, "%d", &N);
    
    memset(tab_O, '\0', 100*sizeof(int));
    memset(tab_B, '\0', 100*sizeof(int));
    int index_O = 0, index_B=0;
    char choix;
    for (int i=0; i < N; i++){
      fscanf(fp, "%c", &choix);
      if (choix == 'O')
	fscanf(fp, "%d", tab_O + index_O++);
      else 
	fscanf(fp, "%d", tab_B + index_B++);
    }



    if (K == pow(2,N)-1 || (K != 0 && ( (int)(K-pow(2,N)+1) %(int)(pow(2,N)) == 0)))
      fprintf(ofp, "Case #%d: ON\n", tmp+1);
    else
      fprintf(ofp, "Case #%d: OFF\n", tmp+1);
  }

  return 1;
}
