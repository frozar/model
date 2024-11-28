#include<stdio.h>
#include<memory.h>
#include<string.h>
int r, n, m;
char dat[6000][32];
char a[1024];
bool pos[32][32];
int ans;
int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  int i, j, tc;
  int cnt;
  // lecture de la premiere ligne de fichier .in
  fscanf(fp, "%d%d%d", &r, &n, &m);
  // n : nbre de mot du dictionnaire
  // dat : contient tous les mots du dictionnaire
  for(i=1;i<=n;i++) fscanf(fp, "%s", &dat[i][1]);
  // m : nombre de cas a traiter dans le fichier
  for(tc=1;tc<=m;tc++)
    {
      // On fout une ligne du fichier .in dans a
      fscanf(fp, "%s", &a[1]);
      // Initialisation de la matrice pos
      memset(pos, false, sizeof(pos));
      // ctn represente l'indice où on est dans le mot de a
      cnt=0;
      // tant qu'on n'est pas a la fin du mot
      for(i=1;a[i];i++)
	{
	  cnt++;
	  // On considere les parenthéses
	  if(a[i]=='(')
	    {
	      for(j=i+1;a[j]!=')';j++) pos[cnt][a[j]-'a'+1]=true;
	      i=j;
	    }
	  else pos[cnt][a[i]-'a'+1]=true;
	}
      // Iniatialisation du nombre mot juste pour ce cas
      ans=0;
      for(i=1;i<=n;i++)
	{
	  // r : nombre de lettre de l'alphabet
	  for(j=1;j<=r;j++)
	    {
	      // On verifie que la lettre du mot apparait comme vrai dans la matrice pos
	      if(!pos[j][dat[i][j]-'a'+1]) break;
	    }
	  if(j>r) ans++;
	}
      // Ecriture dans le fichier de sortie
      fprintf(ofp, "Case #%d: %d\n", tc, ans);
    }
  return 0;
}
