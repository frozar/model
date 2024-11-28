#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<gmp.h>
#include<unistd.h>
#include<fcntl.h>

char D[100];	//recevra la sequence decrypte
char c[100];	//recevra la sequence crypte passé par le script


//procédure de dechiffrement, dechiffre c et le range dans D.
void dechiffrement(mpz_t d, mpz_t n)
{
 mpz_t m; mpz_init(m);      //recevra le bloc a dechiffrer, en appliquant la fonction.
 mpz_t temp; mpz_init(temp);//variable de calcul 
 int i,j,k;		    //variables de calcul
 int taille=300;	    //taille tableaux
 int dec,dec1;	 	    //recevera la taille d'un bloc.
 long conv;		    //recevra le bloc a dechiffrer en format long
 char ch[taille],str[taille],bloc[100]; //tableaux de traitement
 
 strcpy(D,"\0");
 printf("valeur de D au début après initialisation : %s \n",D);
 dec=mpz_get_ui(n);	//convertir n(mpz_t) en dec (entier long).
 sprintf(ch,"%d",dec);	//convertir dec (entier long) en ch (chaine de caracteres).
 dec1=strlen(ch);	//on obtient la taille des blocs.

 strcpy(bloc,"\0");
 for(i=0;i<strlen(c);i++)
 {
  strncpy(bloc,c+i,dec1);	//bloc recevera un Ã  un tous les blocs.
  printf("valeur de bloc: %s\n",bloc);
  //bloc[dec]='\0';
  printf("valeur de bloc après \\0: %s\n",bloc);
  conv=strtol(bloc,NULL,10);	//convertir le bloc(string) en entier long pour pouvoir le dechiffrer.
 printf("valeur de bloc après strtol: %s\n",bloc);
  printf("valeur de conv après strtol: %d\n",conv);
  mpz_set_ui(m,conv);	//convertir conv (entier long) en m (mpz_t).
  mpz_powm(temp,m,d,n);	//fonction de dechiffrement.
  mpz_get_str(bloc,10,temp);//convertier temp (mpz_t) en bloc (string)

  i=i+(dec1-1);
  printf("valeur de D avant if : %s\n",D);
  if(i<strlen(c)-1)	//verifier si on est pas a la fin de la chaine.
  {
   for(j=0;j<(dec1)-strlen(bloc);j++)	//si taille  bloc<n, alors combler avec des 0.
   {
     printf("valeur de D avant 1er strcat: %s\n",D); 
    strcat(D,"0");
    printf("valeur de D après 1er strcat: %s\n",D); 
   }
  }
  else //si fin de la chaine, verifier si la sequence dechiffrée est un multiple de 3, car code ascii d'un char sur 3 nombres.
  {
   k=(strlen(D)+strlen(bloc))%3;
   if(k==0) {k=3;}
   for(j=0;j<(3-k);j++)
   {
    strcat(D,"0");  //on comble avec des 0.
    printf("valeur de D : %s\n",D);
   }
  }
  printf("valeur de D avant 2eme strcat  : %s\n",D);
  strcat(D,bloc); //D recevra a la fin le code ascii de la sequence dechiffrée.
  printf("valeur de D après 2eme strcat  : %s\n",D);
 }//fin for

 //interpréter le code ascii de la sequence et mettre dans D les caracteres dechiffrés.
 strcpy(ch,"\0");
 strcpy(str,D);  
printf("valeur de str après strcpy: %s \n",str);
 j=0;
 for(i=0;i<strlen(D);i++)
 {
  strncpy(bloc,str,3);  //bloc recevra le caractere courant (code ascii sur 3 nombres)
  printf("valeur de bloc après strncpy: %s \n",bloc);
  bloc[i+3]='\0';
  k=atoi(bloc);		//convertir le chaine en int.
  ch[j]=k;		//affecter le code ascii (int) au caractere => sera interprété.
  strcpy(str,str+3);    //caractÃ¨re suivant
  printf("valeur de str après strcpy(str,str+3): %s \n",str);
  j++;
  i=i+2;
 }
 ch[j]='\0';
  printf("valeur de D fin dechiffrement %s \n",D);
 printf("valeur de ch avant strcpy: %s \n",ch);
 strcpy(D,ch);		//D recevra la sequence dechiffrer.

}//end dechiffrement




int main(int argc,char* argv[])
{
 mpz_t d; mpz_init(d); //le script passe le d précédemment calculé (GenKey)
 mpz_t n; mpz_init(n); //le script passe le n précédemment calculé (GenKey)
 int lu, ecrit; 	// valeur retourné par le read and write
 int filechiffrer;	// descripteur du fichier chiffrer
 int fileclaire;	// Descripteur du fichier claire

mpz_init_set_str (d, argv[1], 10);
mpz_init_set_str (n, argv[2], 10);

//strcpy(c,argv[3]);

//le fichier à dechiffrer se trouve en 4ème argument sur la ligne de commande
filechiffrer=open(argv[3],O_RDWR, 0600);
if (filechiffrer < 0)
		{
		perror("erreur d'ouverture du fichier chiffrer-source");
		exit(2);
		} 

// le fichier dechiffrer se trouve en 5ème argument sur la ligne de commande
fileclaire=open(argv[4],O_RDWR|O_CREAT|O_TRUNC, 0600);
if (fileclaire < 0)
		{
		perror("erreur d'ouverture du fichier en claire-destination");
		exit(2);
		} 

printf ("les 2 fichiers:claire et chiffrer sont bien ouvert en lecture/ecriture \n");

do
{
	lu=read(filechiffrer,c,100);
	if (lu<0) 
			{
			perror("Erreur de lecture");
			exit(2);
		        } 
                printf("valeur de c : %s \n",c);
		printf ("ajout de \\0 %d\n ",lu);
                c[lu]='\0';
printf("\n\t\t****** DECHIFFREMENT ******\n\n"); 
dechiffrement(d,n);	//dechiffrer la sequence c et le ranger dans D.
//gmp_printf("%s =====> %s\n",c,D);
ecrit=write(fileclaire,D,100);
if (ecrit<0)
			{
			perror("Erreur d'écriture");
			exit(2);
			} 
//strcpy(D,"\0");
}
while(lu!=0);	//fin du while


printf("dechiffrement OK \n");
return 0;
}
