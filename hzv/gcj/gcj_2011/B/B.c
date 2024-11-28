#include<stdio.h>
#include<string.h>
#include<math.h>

int T, C, D, N;
char tab_C[128], tab_D[64], tab_N[128];
char res[128];

int search_C(char * tab, char * tab_N, int lenght, int i, int * signet_0, int * signet_1){
  for(int j = *signet_0 + *signet_1 + 2; j<lenght; j++){
    if (tab_N[i] == tab[j*3])
      {
	*signet_0 = j*3;
	*signet_1 = -1;
	return 1;
      }
    if (tab_N[i] == tab[j*3+1]) 
      {
	*signet_0 = -1;
	*signet_1 = j*3 + 1;
	return 1;
      }
  }
  return 0;
}

int search_D(char * tab, char * tab_N, int lenght, int i, int * signet_0, int * signet_1){
  for(int j = *signet_0 + *signet_1 + 2; j<lenght; j++){
    if (tab_N[i] == tab[j*2])
      {
	*signet_0 = j*2;
	*signet_1 = -1;
	return 1;
      }
    if (tab_N[i] == tab[j*2+1]) 
      {
	*signet_0 = -1;
	*signet_1 = j*2 + 1;
	return 1;
      }
  }
  return 0;
}

int main(int argc, char ** argv)
{
  (void)argc;
  char infile[32], outfile[32];
  char rien;
  strcpy(infile, argv[1]); strcpy(outfile, argv[1]);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  fscanf(fp, "%d", &T);
  
  for(int test=0; test<T; test++){

    memset(tab_C, '\0', 128*sizeof(char));
    memset(tab_D, '\0', 64*sizeof(char));
    memset(tab_N, '\0', 128*sizeof(char));
    memset(res, '\0', 128*sizeof(char));

    fscanf(fp, "%d", &C);
    fscanf(fp, "%c", &rien); 
    for (int i = 0; i<C; i++){
      fscanf(fp, "%c%c%c", tab_C+3*i, tab_C+1+3*i, tab_C+2+3*i);
      fscanf(fp, "%c", &rien);
    }
    
    fscanf(fp, "%d", &D);
    fscanf(fp, "%c", &rien); 
    for (int i = 0; i<D; i++){
      fscanf(fp, "%c%c", tab_D+2*i, tab_D+1+2*i);
      fscanf(fp, "%c", &rien);
    }

    fscanf(fp, "%d", &N);
    int index = 0;

    fscanf(fp, "%c", &rien); 
    // parser la chaine de char
    for (int i = 0; i<N; i++){
      fscanf(fp, "%c", tab_N+i);
      
      int signet_0 = -1;
      int signet_1 = -1;
      int continuer = 0;

      // on regarde le tableau C
      while( search_C(tab_C, tab_N, C, i, &signet_0, &signet_1) ) 
	{
	  continuer = 0;
	  if (index > 0)
	    {
	      if ( signet_0 == -1) 
		{
		  if (res[index-1] == tab_C[signet_1 - 1])
		    {
		      res[index-1] = tab_C[signet_1 + 1];
		      continuer = 1;
		      break;
		    }
		}
	      if ( signet_1 == -1) 
		{
		  if (res[index-1] == tab_C[signet_0 + 1])
		    {
		      res[index-1] = tab_C[signet_0 + 2];
		      continuer = 1;
		      break;
		    }
		}
	    }
	}
      
      if (continuer){
	continue;
      }
      else {
	res[index++] = tab_N[i];
      }
      
      // on regarde le tableau D
      signet_0 = -1;
      signet_1 = -1;
      while( search_D(tab_D, tab_N, D, i, &signet_0, &signet_1) ) 
	{
	  if ( signet_0 == -1) 
	    {
	      for (int j = 0 ; j<index; j++)
		{
		  if (res[j] == tab_D[signet_1 - 1])
		    {
		      memset(res, '\0', 128*sizeof(char));
		      index=0;
		      break;
		    }
		}
	    }
	  if ( signet_1 == -1) 
	    {
	      for (int j = 0 ; j<index; j++)
		{
		  if (res[j] == tab_D[signet_0 + 1])
		    {
		      memset(res, '\0', 128*sizeof(char));
		      index=0;
		      break;
		    }
		}
	    }
	  
	}

    } // fin du for i

    int longueur = strlen(res);
    fprintf(ofp, "Case #%d: [", test+1);
    for (int i = 0; i < longueur; i++){
      fprintf(ofp, "%c", res[i]);
      if (i != (longueur - 1))
	fprintf(ofp, ", ");
    }
    fprintf(ofp, "]\n");
  } // fin du for test

  return 1;
}
