#include<stdio.h>
#include<memory.h>
#include<string.h>
int N, T;
int C[1024];
int res[sizeof(int)*8];
int i, j, k;

int bit_n(int nb, int n) {
  return ((nb & (1 << n)) != 0);
}

void print_res() {
  int i;
  printf("[ ");
  for (i = sizeof(int)*8 ; 0 <= i; --i) {
    printf("%d, ", res[i]);
  }
  printf("]\n");
}

int ok_res() {
  int i;
  for (i = 0; i < sizeof(int)*8; i++) {
    if ((res[i] % 2) != 0)
      return 0;
  }
  return 1;
}

int sum_C() {
  int i, sum = 0;
  for (i = 0; i < N; i++) {
    sum += C[i];
  }
  return sum;
}

int main()
{
  // Lecture de fichier d'entré
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  // T : Nombre de cas
  fscanf(fp, "%d", &T);
  
  // Pour chaque cas
  for (i=0; i<T; ++i){
    // N : quantité de nombre à traiter
    fscanf(fp, "%d", &N);

    // On réinitialise le tableau à zéro
    memset(C, 0, sizeof(C));
    memset(res, 0, sizeof(res));

    // Initialisation du tableau
    for (j=0; j<N; ++j){
      fscanf(fp, "%d", &C[j]);
    }

    // On compte le nombre de bit à 1 selon leur position
    for (k = 0 ; k < N; k++) {
      for(j=0; j<sizeof(int)*8; ++j){
	res[j] += bit_n(C[k], j);
      }
    }
    
    // On récupère le min dans le tableau
    int min = C[0];
    for (k = 0 ; k < N; k++) {
      if (min>C[k]) 
	min = C[k];
    }

    // print_res();
    // printf("min = %d\n", min);

    // Si le tableau compte uniquement des nombres pairs
    if (ok_res()) {
      int tot = 0;

      int sum = sum_C();
      tot =  sum - min;
      
      /* printf("sum = %d\n", sum); */
      /* printf("min = %d\n", min); */
      /* printf("tot = %d\n\n", tot); */

      fprintf(ofp, "Case #%d: %d\n", i+1, tot);  
    }
    // Si le tableau compte un nombre impair
    else {
      fprintf(ofp, "Case #%d: %s\n", i+1, "NO");  
    }
  }

  return 1;
}
