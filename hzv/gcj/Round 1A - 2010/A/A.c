#include<stdio.h>
#include<string.h>

enum type{
  RED,
  BLUE,
  NONE
};

enum type mat[64][64];

enum type ecrire_mat(char c){
  switch (c){
  case '.':
    return NONE;
    break;
  case 'B':
    return BLUE;
    break;
  case 'R':
    return RED;
    break;
  default:
    return 1000;
    break;
  }
}

void afficher_mat(int N){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      switch(mat[i][j]){
      case NONE:
	printf(".");
	break;
      case BLUE:
	printf("B");
	break;
      case RED:
	printf("R");
	break;
      }
    }
    printf("\n");
  }
  printf("\n\n");
}

void rotate_mat(int N){
  enum type tmp1, tmp2, tmp3, tmp4;
  int k1, k2;
  if (N%2 == 0){
    k1= N/2;
    k2= N/2;
  }
  else{
    k1= N/2;
    k2= N/2 +1;
  }  

  //printf("la valeur de k : %d\n", k);
  for(int i=0; i<k1; i++){
    for(int j=0; j<k2; j++){
      tmp1= mat[i][j];
      tmp2= mat[j][N-1-i];
      tmp3= mat[N-1-i][N-1-j];
      tmp4= mat[N-1-j][i];

      mat[i][j]= tmp4;
      mat[j][N-1-i]=tmp1;
      mat[N-1-i][N-1-j]= tmp2;
      mat[N-1-j][i]= tmp3;
    }
    //afficher_mat(N);
  }
}

void descendre(int i, int j, int N){
  while (i < N && mat[i+1][j]== NONE){
    mat[i+1][j]= mat[i][j];
    mat[i][j]= NONE;
    i++;
  }
}

void gravity_mat(int N){
  for (int i=N-2; i>=0; i--){
    for(int j= 0; j<N; j++){
      if (mat[i][j] != NONE)
	descendre(i, j, N);
    }
  }
}

int di[5]={-1, -1, 0, 1, 1};
int dj[5]={0,   1, 1, 1, 0};

int spread (int i, int j, int di, int dj, int N, int compt){
  int ni=i+di; 
  int nj=j+dj;
  if(ni<0 || nj<0 || ni>=N || nj>=N) return compt;
  else {
    if (mat[ni][nj] == mat[i][j]){
      compt++;
      return spread(ni, nj, di, dj, N, compt);
    }
    else{
      return compt;
    }
  }
}

int gagnant_mat(int N, int K){
  int compt= 1;
  int res= 0;
  enum type temoin= NONE;

  for(int i= N-1; i>=0; i--){
    for(int j=0; j<N; j++){
      if (mat[i][j] == NONE){
	compt= 1;
	continue;
      }
      else{
	for(int k=0; k<5; k++){
	  if (spread(i, j, di[k], dj[k], N, compt) >= K){
	    if (temoin == NONE){
	      temoin= mat[i][j];
	      compt= 1;
	      if (mat[i][j] == BLUE)
		res= 1;
	      else
		res= 2;
	    }
	    else if (temoin != mat[i][j]){
	      compt= 1;
	      res= 3;
	    }
	  }
	  else
	    compt= 1;
	}
      }
    }
  }
  
  return res;
}

int T, N, K, tmp, W;
char c;

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
    
    printf("valeur de N et K: %d %d\n", N, K);    
    fscanf(fp, "%c", &c);
    for (int i=0; i<N; i++){
      for(int j=0; j<N; j++){
	fscanf(fp, "%c", &c);
	mat[i][j]= ecrire_mat(c);
      }
      fscanf(fp, "%c", &c);
    }
    // fscanf(fp, "%c", &c);
    printf("première affichage\n");
    afficher_mat(N);

    rotate_mat(N);
    printf("affichage 2\n");
    afficher_mat(N);

    gravity_mat(N);
    printf("affichage 3\n");
    afficher_mat(N);

    W= gagnant_mat(N, K);
    
    switch (W){
    case 0:
      fprintf(ofp, "Case #%d: %s\n", tmp+1, "Neither");
      break;
    case 1:
      fprintf(ofp, "Case #%d: %s\n", tmp+1, "Blue");
      break;
    case 2:
      fprintf(ofp, "Case #%d: %s\n", tmp+1, "Red");
      break;
    case 3:
      fprintf(ofp, "Case #%d: %s\n", tmp+1, "Both");
      break;
    }

  }
  return 1;
}
