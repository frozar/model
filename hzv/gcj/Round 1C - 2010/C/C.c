#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>

#define max(x,y)  x<y?y:x
#define min(x,y)  x<y?x:y

int T, M, N, K, tmp, i, j, k, l;
char c;
int mat[512][512];
int possible[512][512];
int chesses[512][512];

char carhex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

char lecturehex(char c){
  char place= 0;
  while(c != carhex[(int)(place)]){
    place++;
  }
  return place;
}


// ordre : gauche, haut, droite, bas
void calc_possible(int M, int N){
  int haut = -1, bas = 1, gauche = -1, droite = 1;
  int res = 0;
  for(int i=0; i<M; i++){
    for(int j=0; j<N; j++){
      res = 0;
      res += ( (j == 0) || (mat[i][j] != mat[i][max(0, j+gauche)]) );
      res += ( (i == 0) || (mat[i][j] != mat[max(0, i+haut)][j])) << 1;
      res += ( (j == N-1) || (mat[i][j] != mat[i][min(N-1, j+droite)])) << 2;
      res += ( (i == M-1) || (mat[i][j] != mat[min(M-1, i+bas)][j])) << 3;

      possible[i][j] = res;
    }
  }  
}

int calc_chesses_aux(int u, int v, int * color, int M, int N){
  int cond = 1;
  int same = 2;
  int lastSame = 0;
  int termine = 0;

  if (mat[u][v]){
    return ( (u == M-1) && (v == N-1));
  }

  while (same){
    for (int i=0; i<same && cond && same<M && (i+u)<M; i++){
      for (int j=0; j<same && cond && same<N && (j+v)<N; j++){
	if ( (i+u == 0) && (j+v == 0) ){
	  continue;
	}
	else if( ((i+u) == M-1) && ((j+v) == N-1)){
	  termine = 1;
	  break;
	}
	// 1ere ligne
	else if ( (i+u == 0) && (j+v != 0) ){
	  if ( mat[i+u][j+v-1] != mat[i+u][j+v] ){
	    cond = 1;
	  }
	}
	// 1ere colonne
	else if ( (i+u != 0) && (j+v == 0) ){
	  if ( mat[i+u-1][j+v] != mat[i+u][j+v] ){
	    cond = 1;
	  }
	}
	else if ( (mat[i+u-1][j+v] != mat[i+u][j+v]) && (mat[i+u][j+v-1] != mat[i+u][j+v]) ){
	  cond = 1;
	}
	else {
      	  cond = 0;
	  same = 0;
	}	
      }
    }
    if (cond && !termine){
      lastSame= same;
      same++;
    }
    else if (termine)
      break;
    else
      same = 0;
  }
  
  if (lastSame >= 2){
    for (int i = 0; i < lastSame; i++){
      for (int j = 0; j < lastSame; j++){
	chesses[i+u][j+v] = *color;
      }
    }
  }
  (*color)++;

  return termine;
}

void afficherche(int M, int N){
  int i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      printf("%d ", chesses[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void calc_chesses(int M, int N){
  memset(chesses, 0, M*N);
  int * color = malloc(sizeof(int));
  int u = 0, v = 0;
  
  *color = 1;

  while (!calc_chesses_aux(u, v, color, M, N)) {
    printf("\n");
    printf("u = %d, M = %d\n", u, M);
    printf("v = %d, N = %d\n", v, N);
    if (v < N-1)
      v++;
    else{
      v = 0;
      u++;
      afficherche(M, N);
    }
  }
  
  free(color);
}

void affichermat(int M, int N){
  int i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      printf("%d ", mat[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void afficherpos(int M, int N){
  int i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      printf("%d ", possible[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char ** argv)
{
  (void)argc;
  char filename[16];
  char infile[32], outfile[32];

  //filename = argv[1];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); 
  strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"); //*ofp=fopen(outfile, "w");


  fscanf(fp, "%d", &T);
  
  for(tmp=0; tmp<T; tmp++){
    fscanf(fp, "%d %d", &M, &N);
    
    fscanf(fp, "%c", &c);
    for(i=0; i<M; i++){
      for(j=0; j<N/4; j++){
	k= j;
	fscanf(fp, "%c", &c);
	c= lecturehex(c);
	for(l=0; l<4; l++){
	  if((int)c & 1<<l)
	    mat[i][j*4+l]= 1;
	  else
	    mat[i][j*4+l]= 0;
	}
      }
      fscanf(fp, "%c", &c);
    }

    affichermat(M, N);
    calc_possible(M, N);
    //    afficherpos(M, N);
    calc_chesses(M, N);
    afficherche(M, N);

    printf("%d\n", (possible[0][0] & (1 << 1)) != 0);
    //fprintf(ofp, "Case #%d: %s\n", tmp+1, "Neither");
    
  }

  
  return 1;
}
