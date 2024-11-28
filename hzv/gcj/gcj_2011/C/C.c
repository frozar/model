#include<stdio.h>
#include<string.h>
#include<math.h>

#define max(x, y)  x<y?y:x

int T, N;
int pile[1024];

int main(int argc, char ** argv)
{
  (void)argc;
  char infile[32], outfile[32];
  int combi;
  int sum_P_1, sum_P_2, sum_S_1, sum_S_2, Max;
  int tmp;
  strcpy(infile, argv[1]); strcpy(outfile, argv[1]);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  fscanf(fp, "%d", &T);

  for(int test=0; test<T; test++){
    memset(pile, 0, 1024*sizeof(int));
    combi = 0;
    Max = -1;

    fscanf(fp, "%d", &N); 
    for (int i = 0; i<N; i++){
      fscanf(fp, "%d", pile+i);
    }
    
    while (combi < pow(2, N)){
      sum_P_1=-1;
      sum_P_2=-1;
      sum_S_1=0;
      sum_S_2=0;

      for (int i=0; i<N; i++){
	if ( combi & 1<<i )
	  {
	    if (sum_P_1 == -1)
	      sum_P_1 = pile[i];
	    else
	      sum_P_1 = sum_P_1 ^ pile[i];
	    sum_S_1 += pile[i];
	  }
	else 
	  {
	    if (sum_P_2 == -1)
	      sum_P_2 = pile[i];
	    else
	      sum_P_2 = sum_P_2 ^ pile[i];
	    sum_S_2 += pile[i];
	  }
      }
      
      // fprintf(stderr, "sum_P_1= %d, sum_P_2 = %d, sum_S_1 = %d,  sum_S_2=%d\n", 
      //     sum_P_1, sum_P_2, sum_S_1,  sum_S_2);
      tmp = max(sum_S_1, sum_S_2);
      if ((sum_P_1 == sum_P_2) && (Max < tmp))
	{
	  Max = tmp;
	}

      combi++;
    }

    fprintf(stderr, "Max = %d\n", Max);
    if (Max == -1)
      fprintf(ofp, "Case #%d: NO\n", test+1);
    else
      fprintf(ofp, "Case #%d: %d\n", test+1, Max);
  } // fin du for test

  return 1;
}
