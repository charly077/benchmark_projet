/**
*  Premier benchmark pour le projet3 de sinf1252
*  Author Stephane Kimmel & Charles Jacquet
*  Le but de ce benchmark est de voir l'efficacité de readdir
*  Et de créer un graphique affichable en html.
*
**/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h> 
#include <errno.h>  

#include "benchmark.h"
#include "copy.h"

#define NOMBREDEFICHIERMAX 10000
#define STEPNUMBERFILES 2
#define LONGUEURFICHIERALLOUE 10000

void chaineCaractere(char* file, int longueur)
{
	  char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      while (longueur-- > 0 )
      {
      	size_t index = longueur%(sizeof file - 1);
        *file++ = charset[index];
      }
      *file = '\0';

}

int main(int argc, char *argv[]){

	char* nameDossier = "repertoire";

	//création du répertoire :
	if(mkdir("repertoire",S_IRWXU)){ // ! j'ai changé de './repertoire'
		perror("rmdir");
	}

	//création et allocation des structures qui permettent de calculer les temps du benchmark:
	timer *t = timer_alloc();
	recorder *record = recorder_alloc("readdir.csv");
	recorder *record2 = recorder_alloc("readdirfull.csv");

	//ouvrir un document 
	DIR *rep = NULL;
	rep = opendir(nameDossier);
	if(rep == NULL){
		perror("Problème lors de l'ouverture du fichier");
	}

	struct dirent* fichierLu = NULL; 
	//maintenant qu'on a ouvert le répertoire, il faut créer des fichier
	//de la même taille
	//ex pour 10 100 1000 10 000 100 000 fichiers
	int N;
	char nameFichierFinal[255];
	char namePrevFichier[255];
	sprintf((char *) &namePrevFichier, "%s%s",nameDossier,"/fichier");
	FILE *f;
	int i;
	// -----------------------------------------------------------------------------------------

	printf("On commence avec les fichiers vides \n");

	for(N=1;N<=NOMBREDEFICHIERMAX;N=N+STEPNUMBERFILES)
	{
		
		// On crée tous les fichiers dans un dossier
		for(i=N-STEPNUMBERFILES;i<N;i++){

			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
		
			// On n'utilise pas la fonction de 'copy.h' parce qu'elle remplit les fichiers alors qu'on ne le veut pas forcément
			create_file(nameFichierFinal, 0);

		}
		// Une fois qu'ils sont créés, on rebobine
		rewinddir(rep);
		start_timer(t);

		// On lit le dossier pour voir combien de temps ça prend
		while((fichierLu = readdir(rep))!=NULL)
		{
			//printf("Le fichier lu s'appelle '%s'\n", fichierLu->d_name);
		}
		//j'utilise le record_n parce qu'il faut diviser par le nombre d'de fichier parce que qui est
		//interessant c'est le temps par fichier
		write_record_n(record,N,stop_timer(t),1);

		printf("test fini pour N=%d\n",N);
	}

	// Effacement de tous les fichiers
	for(i=0;i<NOMBREDEFICHIERMAX;i++)
		{
			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
			
			// On supprime le fichier avec la méthode de copy.c
			rm(nameFichierFinal);
		}
	
	printf("Fini les fichiers vides \n");

	// ----------------------------------------------------------------------------------------------------------

	printf("On commence avec les fichiers remplis \n");
	char buf[LONGUEURFICHIERALLOUE];

	for(N=1;N<=NOMBREDEFICHIERMAX;N=N+STEPNUMBERFILES){
		int i;

		// On crée les N fichiers
		for(i=N-STEPNUMBERFILES;i<N;i++)
		{
			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
		
			// On crée un fichier avec la méthode de copy.c qui le remplit directement -> n'arrive pas à l'importer...
			create_file(nameFichierFinal, LONGUEURFICHIERALLOUE);

		}

		rewinddir(rep);

		// On réutilise le pointeur déjà alloué
		start_timer(t);

		while((fichierLu = readdir(rep))!=NULL)
		{
			//printf("Le fichier lu s'appelle '%s'\n", fichierLu->d_name);
		}

		//j'utilise le record_n parce qu'il faut diviser par le nombre d'de fichier parce que qui est
		//interessant c'est le temps par fichier
		write_record_n(record2,N,stop_timer(t),1);

		printf("Test full fini pour N=%d\n",N);
	}

	// Effacement de tous les fichiers
	for(i=0;i<NOMBREDEFICHIERMAX;i++)
		{
			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
			
			// On supprime le fichier avec la méthode de copy.c -> ne fonctionne pas, n'arrive pas à l'importer...
			rm(nameFichierFinal);
		}

	if(closedir(rep) == -1)
	{
		return -1; // EXIT_FAILURE
	}

	//suppression du répertoire (vide):
	if(rmdir(nameDossier))
	{
		perror("rmdir");
	}

	recorder_free(record);
	recorder_free(record2);
	timer_free(t);

	printf("Le programme a fini... \n");

	return 0;
}
