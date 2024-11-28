#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "gmp.h"

int trint (const void *nbre1, const void *nbre2){
  int comp= mpz_cmp(nbre1, nbre2);
  comp= -comp;
  return comp;
}

mpz_t pgcd(mpz_t nombre1, mpz_t nombre2)
{
  if(mpz_cmp_si(nombre2, 0))
    return nombre1;
  mpz_mod(r, nombre1, nombre2);
  return pgcd(nombre2, r);
}

int C, tmp, N, i, j;
mpz_t res, nb1, nb2, y;
mpz_t diff, modulo, r;
mpz_init(r);
mpz_t tab[1024], tabaux[1024];

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
    fscanf(fp, "%d", &N);
    
    fprintf(stderr, "passe 1: %d\n", tmp+1);

    if (N == 2){
      fscanf(fp, "%llu", &nb1);
      fscanf(fp, "%llu", &nb2);

      if (nb2 > nb1){
	mpz_t ech= nb1;
	nb1= nb2;
	nb2= ech;
      }
      
      diff= nb1-nb2;
      fprintf(stderr, "calcul de diff \n");
      modulo= ((nb2)%diff);
      fprintf(stderr, "calcul de modulo \n");
      y= diff - modulo;
      fprintf(stderr, "calcul de y \n");
      fprintf(ofp, "Case #%d: %Zd\n", tmp+1, y);
    }
    else{
      for(i=0; i<N; i++){
	fscanf(fp, "%llu", &tab[i]);
      }
    
      qsort(tab, N, sizeof(mpz_t), trint);

      fprintf(stderr, "tab \n");
      for(i=0; i<N; i++)
	fprintf(stderr, "%Zd ", tab[i]);
      fprintf(stderr, "\n");

      for(i=0; i<(N-1); i++){
	tabaux[i]= tab[0] - tab[i+1];
      }

      tabaux[N-1]= tab[1]-tab[2];
      
      fprintf(stderr, "tabaux \n");
      for(i=0; i<N; i++)
	fprintf(stderr, "%Zd ", tabaux[i]);
      fprintf(stderr, "\n");
	    
      res= tabaux[0];
    
      for(i=1; i<N; i++){
	res= pgcd(res, tabaux[i]);
      }

      fprintf(stderr, "res: %Zd \n", res);

      if(res == 1)
	y= 0;
      else
	y= res - (tab[0]%res);
      fprintf(stderr, "y: %Zd \n", y);
      fprintf(ofp, "Case #%d: %Zd\n", tmp+1, y);
    }
  }
  return 1;
}
