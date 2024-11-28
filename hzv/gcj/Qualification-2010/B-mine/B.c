#include<stdio.h>
#include<memory.h>
#include<string.h>
#define TAILLE_MAX 1024

int N, T;
char Lin[TAILLE_MAX], Lout[TAILLE_MAX];
char W[512];
char c;
int len, i, j, k;

int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"); //*ofp=fopen(outfile, "w");

  fscanf(fp, "%d", &N);
  c= fgetc(fp);
  for (T=0; T<N; T++){
    len= 0;
    memset(Lin, 0, sizeof(Lin));
    memset(Lout, 0, sizeof(Lout));

    // on lit jusqu'à la fin de la ligne
    fgets(Lin, TAILLE_MAX, fp);
    fprintf(stderr, "%s", Lin);

    if (T == N-1) fprintf(stderr, "\n");
    
    len= strlen(Lin);
    fprintf(stderr, "la longueur de Lin : %d\n", len);

    i= len-1;
    k= 0;
    while(i>0){
      j= 0;
      // on prend un mot de la ligne originale
      while(Lin[i] != ' '){
	W[j]= Lin[i];
	j++;
	i--;
      }
      //W[j]= '\0';
      i--;
      fprintf(stderr, "le mot saisi : %s\n", W);
      

      fprintf(stderr, "la valeur de j en entrée de boucle : %d\n", j);
      fprintf(stderr, "la valeur de W[j] : %c\n", W[j]);
      fprintf(stderr, "la valeur de W[j-1] : %c\n", W[j-1]);
      fprintf(stderr, "la valeur de W[j-2] : %c\n", W[j-2]);
      fprintf(stderr, "la valeur de W[j-3] : %c\n", W[j-3]);
      fprintf(stderr, "la valeur de W[j-4] : %c\n", W[j-4]);

      int l= k;
      j--;
      while(j-l>= 0){
	Lout[l]= W[j-l];
	l++;
      }
      
      k= l;
      Lout[k]= '\0';
      fprintf(stderr, "la Lout tmp: %s\n", Lout);
    }
    
    fprintf(stderr, "la Lout : %s\n", Lout);
  }

/*     // acquisition de L */
/*     i= 0; */
/*     for(; lenin > 0; lenin--){ */
/*       fseek(fp, -1*sizeof(char), SEEK_CUR); */
/*       c= fgetc(fp); fseek(fp, -1*sizeof(char), SEEK_CUR); */
/*       if (c != ' '){ */
/* 	W[i]= c; */
/* 	i++; */
/*       } */
      
/*       else{ */
/* 	for(j=0; j <= i; j++){ */
/* 	  L[j]= W[i-j]; */
/* 	  lenout++; */
/* 	} */
/* 	L[j+1]= ' '; */
/* 	lenout++; */
/*       } */
/*     } */
      
/*     // affichage de la ligne L */
/*     for (j=0; j<len; j++) */
/*       fprintf(stderr, "%c", Lin[j]); */

/*     // ecriture dans le fichier de sortie */
/*     fprintf(ofp, "Case #%d: ", T+1); */
/*     for (j=0; j < lenout; j++){ */
/*       fprintf(ofp, "%c", L[j]); */
/*     } */
/*     fprintf(ofp, "\n"); */

/*     // on reposition le curseur a la fin de la ligne */
/*     fseek(fp, (lenout+1)*sizeof(char), SEEK_CUR); */

  
  return 1;
}
