#include<stdio.h>
#include<memory.h>
#include<string.h>
#define INF 999999
int n, m;
// a : la matrice en entrée
int a[128][128];
// path : une matrice auxiliaire indiquant la direciton de l'écoulement à une cellule donnée
int path[128][128];
// dx, dy : les différentes directions possibles : {haut, gauche, droite, bas}
int dx[4]={-1, 0, 0, 1};
int dy[4]={0, -1, 1, 0};
int inv[4]={3, 2, 1, 0};
// ans : la matrice de sortie
// anscnt : contient la bonne valeur à donner à la cellule traitée
int ans[128][128], anscnt;
// v : une matrice indiquant si une cellule est évaluée ou pas
bool v[128][128];

void f1(int x, int y)
{
  v[x][y]=true; ans[x][y]=anscnt;
  int k, nx, ny;
  // si on est en un point d'écoulement
  if(path[x][y]!=-1)
    {
      nx=x+dx[path[x][y]];
      ny=y+dy[path[x][y]];
      // si la cellule vers laquelle on s'écoule n'est pas déjà évaluée : appel rec
      if(!v[nx][ny]) f1(nx, ny);
    }
  // on regarde les cellules alentours
  for(k=0;k<4;k++)
    {
      nx=x+dx[k]; ny=y+dy[k];
      if(nx<1 || ny<1 || nx>n || ny>m) continue;
      // si la cellule en nx, ny est un pt d'écoulement vers la cellule x,y, et qu'elle n'est pas évaluée : appel rec
      if(path[nx][ny]!=-1 && nx+dx[path[nx][ny]]==x && ny+dy[path[nx][ny]]==y && !v[nx][ny])
	f1(nx, ny);
    }
}

int main()
{
  char filename[32];
  char infile[32], outfile[32];
  scanf("%s", filename);
  strcpy(infile, filename); strcpy(outfile, filename);
  strcat(infile, ".in"); strcat(outfile, ".out");
  FILE *fp=fopen(infile, "r"), *ofp=fopen(outfile, "w");

  // t : variable temporaire
  // tc : nombre de matrice a traité
  int t, tc;
  int i, j, k;
  int nx, ny;
  int mh, pt;
  fscanf(fp, "%d", &tc);
  for(t=1;t<=tc;t++)
    {
      // n : nombre de ligne de la matrice à traiter
      // m : nombre de colonne de la matrice à traiter
      fscanf(fp, "%d%d", &n, &m);
      for(i=1;i<=n;i++)
	{
	  // a : la matrice à traiter
	  // initialisation de a
	  for(j=1;j<=m;j++) fscanf(fp, "%d", &a[i][j]);
	}
      // recherche du chemin d'écoulement pour toutes les cases
      for(i=1;i<=n;i++)
	{
	  for(j=1;j<=m;j++)
	    {
	      mh=INF;
	      path[i][j]=-1;
	      // recherche de l'altitude min autour du point i, j
	      for(k=0;k<4;k++)
		{
		  nx=i+dx[k]; ny=j+dy[k];
		  // on vérifie qu'on regarde bien dans la matrice
		  if(nx<1 || ny<1 || nx>n || ny>m) continue;
		  // on conserve la première valeur min d'altitude rencontrée
		  if(mh>a[nx][ny]){mh=a[nx][ny]; pt=k;}
		}
	      // si l'altitude min est > qu'à celle de la case considérée on passe à la cellule suivante
	      if(mh>=a[i][j]) continue;
	      path[i][j]=pt;
	    }
	}

      // initialisation des variables
      anscnt=0;
      memset(v, false, sizeof(v));
      for(i=1;i<=n;i++)
	{
	  for(j=1;j<=m;j++)
	    {
	      if(v[i][j]) continue;
	      anscnt++; f1(i, j);
	    }
	}
      // Ecriture dans le fichier .out
      fprintf(ofp, "Case #%d:\n", t);
      for(i=1;i<=n;i++)
	{
	  for(j=1;j<m;j++)
	    {
	      fprintf(ofp, "%c ", ans[i][j]+'a'-1);
	    }
	  fprintf(ofp, "%c\n", ans[i][m]+'a'-1);
	}
    }
  return 0;
}
