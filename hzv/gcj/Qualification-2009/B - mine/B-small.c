#include<stdio.h>
#include<string.h>

int T, H, W;
char lettre= 'a';

enum Booleens{
  FAUX,
  VRAI
};

typedef struct position_{
  int l, c;
} pos;

typedef struct pt_ {
  int alt;
  char c;
} pt;

int alreadyass(pt point){
  return (point.c >= 'a');
}

pos ppmin(pt matrix[H][W], int i, int j){
  pos res;
  // Les coins
  if (i == 0 && j == 0){
    if (matrix[i][j+1].alt < matrix[i+1][j].alt){
      res.l= i; res.c= j+1;}
    else{
      res.l= i+1; res.c= j;}

    return res;
  }
  else if (i == 0 && j == W-1){
    if (matrix[i][j-1].alt < matrix[i+1][j].alt){
      res.l= i; res.c= j-1;}
    else{
      res.l= i+1; res.c= j;}

    return res;
  }
  else if (i == H-1 && j == 0){
    if (matrix[i-1][j].alt < matrix[i][j+1].alt){
      res.l= i-1; res.c= j;}
    else{
      res.l= i; res.c= j+1;}

    return res;
  }
  else if (i == H-1 && j == W-1){
    if (matrix[i][j-1].alt < matrix[i-1][j].alt){
      res.l= i; res.c= j-1;}
    else{
      res.l= i-1; res.c= j;}

    return res;
  }
  // Les bords
  else if (i == 0){
    if (matrix[i][j-1].alt < matrix[i+1][j].alt){
      if (matrix[i][j-1].alt < matrix[i][j+1].alt){
	res.l= i; res.c= j-1;}
      else{
	res.l= i; res.c= j+1;}
    }
    else{
      if (matrix[i+1][j].alt < matrix[i][j+1].alt){
	res.l= i+1; res.c= j;}
      else{
	res.l= i; res.c= j+1;}
    }
    return res;
  }
  else if (i == H-1){
    if (matrix[i][j-1].alt < matrix[i-1][j].alt){
      if (matrix[i][j-1].alt < matrix[i][j+1].alt){
	res.l= i; res.c= j-1;}
      else{
	res.l= i; res.c= j+1;}
    }
    else{
      if (matrix[i-1][j].alt < matrix[i][j+1].alt){
	res.l= i-1; res.c= j;}
      else{
	res.l= i; res.c= j+1;}
    }
    return res;
  }
  else if (j == 0){
    if (matrix[i-1][j].alt < matrix[i][j+1].alt){
      if (matrix[i-1][j].alt < matrix[i+1][j].alt){
	res.l= i-1; res.c= j;}
      else{
	res.l= i+1; res.c= j;}
    }
    else{
      if (matrix[i][j+1].alt < matrix[i+1][j].alt){
	res.l= i; res.c= j+1;}
      else{
	res.l= i+1; res.c= j;}
    }
    return res;
  }
  else if (j == W-1){
    if (matrix[i-1][j].alt < matrix[i][j-1].alt){
      if (matrix[i-1][j].alt < matrix[i+1][j].alt){
	res.l= i-1; res.c= j;}
      else{
	res.l= i+1; res.c= j;}
    }
    else{
      if (matrix[i][j-1].alt < matrix[i+1][j].alt){
	res.l= i; res.c= j-1;}
      else{
	res.l= i+1; res.c= j;}
    }
    return res;
  }
  else{
    if (matrix[i-1][j].alt < matrix[i][j+1].alt){
      if (matrix[i-1][j].alt < matrix[i+1][j].alt){
	if (matrix[i-1][j].alt < matrix[i][j-1].alt){
	  res.l= i-1; res.c= j;}
	else{
	  res.l= i; res.c= j-1;}
	return res;
      }
      else{
	if (matrix[i+1][j].alt < matrix[i][j-1].alt){
	  res.l= i+1; res.c= j;}
	else{
	  res.l= i; res.c= j-1;}
	return res;
      }
    }
    else{
      if (matrix[i][j+1].alt < matrix[i+1][j].alt){
	if (matrix[i][j+1].alt < matrix[i][j-1].alt){
	  res.l= i; res.c= j+1;}
	else{
	  res.l= i; res.c= j-1;}
	return res;
      }
      else{
	if (matrix[i+1][j].alt < matrix[i][j-1].alt){
	  res.l= i+1; res.c= j;}
	else{
	  res.l= i; res.c= j-1;}
	return res;
      }
    }
  }
}
      

void ass(pt matrix[H][W], int i, int j){
/*   // premiere ligne : regard à gauche */
/*   if (i == 0){ */
/*     if (j > 0){ */
/*       if (matrix[i][j-1].alt < matrix[i][j].alt) */
/* 	matrix[i][j].c= matrix[i][j-1].c; */
/*       else{ */
/* 	lettre++; */
/* 	matrix[i][j].c= lettre; */
/*       } */
/*     } */
/*   } */
/*   // cas général */
/*   else{ */
/*     // regard à gauche et en haut */
/*     if (j > 0){ */
/*       if ((matrix[i][j-1].alt < matrix[i][j].alt) && (matrix[i-1][j].alt < matrix[i][j].alt)){ */
/* 	if (matrix[i][j-1].alt < matrix[i-1][j].alt) */
/* 	  matrix[i][j].c= matrix[i][j-1].c; */
/* 	else */
/* 	  matrix[i][j].c= matrix[i-1][j].c; */
/*       } */
/*       else if ((matrix[i][j-1].alt < matrix[i][j].alt) || (matrix[i-1][j].alt < matrix[i][j].alt)){ */
/* 	if (matrix[i][j-1].alt < matrix[i][j].alt) */
/* 	  matrix[i][j].c= matrix[i][j-1].c; */
/* 	else */
/* 	  matrix[i][j].c= matrix[i-1][j].c; */
/*       } */
/*       else{ */
/* 	lettre++; */
/* 	matrix[i][j].c= lettre; */
/*       } */
/*     } */
/*     // regard en haut */
/*     else{ */
/*       if (matrix[i-1][j].alt < matrix[i][j].alt) */
/* 	matrix[i][j].c= matrix[i-1][j].c; */
/*       else{ */
/* 	lettre++; */
/* 	matrix[i][j].c= lettre; */
/*       } */
/*     } */
/*   } */
/* } */
  pos min= ppmin(matrix, i, j);
  pt tmp= matrix[min.l][min.c];
  
  if (tmp.alt < matrix[i][j].alt){
    if (alreadyass(tmp))
      matrix[i][j].c= tmp.c;
    else{
      lettre++;
      matrix[i][j].c= lettre;
      tmp.c= lettre;
    }
  }
}

void flow(pt matrix[H][W], int i, int j){
  // dernière ligne : regard à droite
  if (i == H-1){
    if (j < W-1){
      if (matrix[i][j].alt > matrix[i][j+1].alt)
	matrix[i][j+1].c= matrix[i][j].c;
    }
  }
  // dernière colonne : regard en bas
  else if (j == W-1){
    if(i < H-1){
      if (matrix[i][j].alt > matrix[i+1][j].alt)
	matrix[i+1][j].c= matrix[i][j].c;
    }
  }
  // cas général
  else{
    if ((matrix[i][j].alt > matrix[i][j+1].alt) && (matrix[i][j].alt > matrix[i+1][j].alt)){
      if (matrix[i+1][j].alt > matrix[i][j+1].alt)
	matrix[i][j+1].c= matrix[i][j].c;
      else
	matrix[i+1][j].c= matrix[i][j].c;
    }
    else if((matrix[i][j].alt > matrix[i][j+1].alt) || (matrix[i][j].alt > matrix[i+1][j].alt)){
      if (matrix[i][j].alt > matrix[i][j+1].alt)
	matrix[i][j+1].c= matrix[i][j].c;
      else
	matrix[i+1][j].c= matrix[i][j].c;
    }
  }
}

void affichage(pt matrix[H][W]){
  int i, j;
  for (i=0; i<H; i++){
    for(j=0; j<W; j++){
      printf("%c ", matrix[i][j].c);
    }
    printf("\n");
  }
}

int main (){
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");
  
  int i, j, k;
  fscanf(fp, "%d", &T);

  for(k=0; k<T; k++){
    fscanf(fp, "%d %d", &H, &W);
  
    pt matrix[H][W];
    
    // Initialisation de matrix
    char c= fgetc(fp);
    c= fgetc(fp);
    for(i=0; i<H; i++){
      for(j=0; j<W; j++){
	//fprintf(stderr, "first%c", c);
	matrix[i][j].alt= c - '1'+1;
	matrix[i][j].c= 'a'-1;
	// l'espace
	c= fgetc(fp); 
	//fprintf(stderr, "second%c", c);
	// le chiffre suivant
	c= fgetc(fp);
	//fprintf(stderr, "third%c", c);
      }
    }
    matrix[0][0].c= 'a';

    // début du travail sur matrix
    for(i=0; i<H; i++){
      for(j=0; j<W; j++){
	if(alreadyass(matrix[i][j]))
	  flow(matrix, i, j);
	else{
	  ass(matrix, i, j);
	  flow(matrix, i, j);
	}
	affichage(matrix);
      }
    }
    //fin du travail sur matrix

    // Ecriture dans le fichier .out
    fprintf(ofp, "Case #%d:\n", k+1);
    for(i=0; i<H; i++){
      for(j=0; j<W; j++){
	fprintf(ofp, "%c ", matrix[i][j].c);
      }
      fprintf(ofp, "\n");
    }
    fprintf(ofp, "\n");

    lettre= 'a';
  }
}
