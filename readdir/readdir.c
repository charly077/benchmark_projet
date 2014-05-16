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

#define NOMBRE_MAX_FICHIERS 2000
#define STEP_NUMBER_FILES 1
#define LONGUEUR_FICHIER_ALLOUE 10000

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
	int k;
	// -----------------------------------------------------------------------------------------

	printf("On commence avec les fichiers vides \n");

	// On crée tous les fichiers dans un dossier
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++){

		sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
		
		// On n'utilise pas la fonction de 'copy.h' parce qu'elle remplit les fichiers alors qu'on ne le veut pas forcément
		create_file(nameFichierFinal, 1);

	}

	printf("Fichiers créés \n");

	// ----------------------------------------------------------------------------------------------------------


	rewinddir(rep);
	start_timer(t);
	int a;
	int b;
	int tot = 0;

	for (k = 0; k <= NOMBRE_MAX_FICHIERS; k++)
	{
			a = stop_timer(t);
			readdir(rep);
			b = stop_timer(t);
			tot = tot+(b-a);
			write_record(record, k, tot);

	}


	printf("Fini les fichiers vides \n");

	// Effacement de tous les fichiers
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++)

		{
			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
			
			// On supprime le fichier avec la méthode de copy.c -> ne fonctionne pas, n'arrive pas à l'importer...
			rm(nameFichierFinal);
		}

	// ----------------------------------------------------------------------------------------------------------

	printf("On commence avec les fichiers remplis \n");

	// On crée tous les fichiers dans un dossier
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++){

		sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
		
		// On n'utilise pas la fonction de 'copy.h' parce qu'elle remplit les fichiers alors qu'on ne le veut pas forcément
		create_file(nameFichierFinal, LONGUEUR_FICHIER_ALLOUE);

	}

	printf("Fichiers remplis créés \n");

	// ----------------------------------------------------------------------------------------------------------

	rewinddir(rep);
	start_timer(t);
	a = 0;
	b = 0;
	tot = 0;

	for (k = 0; k <= NOMBRE_MAX_FICHIERS; k++)
	{
			a = stop_timer(t);
			readdir(rep);
			b = stop_timer(t);
			tot = tot+(b-a);
			write_record(record2, k, tot);

	}


	printf("Fini les fichiers remplis \n");

	// Effacement de tous les fichiers
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++)

		{
			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
			
			// On supprime le fichier avec la méthode de copy.c -> ne fonctionne pas, n'arrive pas à l'importer...
			rm(nameFichierFinal);
		}

	// ----------------------------------------------------------------------------------------------------------


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
