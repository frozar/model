#include<stdio.h>
#include<string.h>

int C, T, N, K, tmp, S, i, j, k, ok, print;
long int B, echange;
long int X[64], D[64];
long int V[64];

int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");


  fscanf(fp, "%d", &C);
  
  for(tmp=0; tmp<C; tmp++){
    S= 0;
    print= 0;
    fscanf(fp, "%d %d %ld %d", &N, &K, &B, &T);
    
    for(i=0; i<N; i++){
      fscanf(fp, "%ld", &X[i]);
    }

    for(i=0; i<N; i++){
      fscanf(fp, "%ld", &V[i]);
    }

    for(i=0; i<N; i++){
      D[i]= X[i]+V[i]*T;
    }
    
    ok= 1;
    if(K == 0){
      fprintf(ofp, "Case #%d: %s\n", tmp+1, "0");
      print= 1;
    }
    else{
      for(i=N-2; i>=0; i--){
	for(j=i; j<(N-1); j++){
	  if(!print){
	    for(k=0; k<K; k++){
	      if(D[N-1-k] < B) ok= 0;
	    }
	    if(ok){
	      fprintf(ofp, "Case #%d: %d\n", tmp+1, S);
	      S= 0;
	      print= 1;
	    }
	    else{
	      ok= 1;
	    }
	    if(D[j] > D[j+1]){
	      S++;
	      echange= D[j];
	      D[j]= D[j+1];
	      D[j+1]= echange;
	    }
	  }
	}
      }
    }
    
    if(!print){
      fprintf(ofp, "Case #%d: %s\n", tmp+1, "IMPOSSIBLE");
      print= 1;
      S= 0;
    }
  }

  return 1;
}
