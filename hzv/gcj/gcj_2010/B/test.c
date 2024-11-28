#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "gmp.h"

int trint (const void *nbre1, const void *nbre2){
  int comp= mpz_cmp(nbre1, nbre2);
  comp= -comp;
  return comp;
}

mpz_t r; 

void pgcdrec(mpz_t nombre1, mpz_t nombre2)
{
  if(!mpz_cmp_si(nombre2, 0)){
    mpz_mod(r, nombre1, nombre2);
    pgcdrec(nombre2, r);
  }
}

void pgcd (void *nombre1, void *nombre2)
{
  mpz_t r;
  mpz_init(r);
  pgcdrec(nombre1, nombre2);
}  

int main()
{  
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  mpz_t nb; mpz_init(nb);
  mpz_t nb1; mpz_init(nb1);
  mpz_t nb2; mpz_init(nb2);
  int taille=300;
  char ch[taille];

  mpz_inp_str(nb, fp, 10);
  mpz_get_str(ch, 10, nb);
  fprintf(ofp, "%s\n", ch);

  mpz_inp_str(nb, fp, 10);
  mpz_get_str(ch, 10, nb);
  fprintf(ofp, "%s\n", ch);

  mpz_inp_str(nb1, fp, 10);
  mpz_get_str(ch, 10, nb1);
  fprintf(ofp, "%s\n", ch);

  mpz_inp_str(nb2, fp, 10);
  mpz_get_str(ch, 10, nb2);
  fprintf(ofp, "%s\n", ch);
  
  fprintf(stderr, "resultat test : %d\n", trint(nb1, nb2));

  return 1;
}

/*   int i; */
/*   int tab[128]; */
  
/*   tab[0]= 56; */
/*   tab[1]= 389; */
/*   tab[2]= 12; */
/*   tab[3]= 3; */
/*   tab[4]= 56; */
/*   tab[5]= 34; */
  
/*   qsort(tab, 6, sizeof(int), trient); */
  
/*   for(i=0; i<6; i++){ */
/*     printf("tab[%d]: %d\n", i+1, tab[i]); */
/*   } */
