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

int main(int argc, char *argv[]){

	char* nameDossier = "repertoire";

	//création du répertoire :
	if(mkdir("repertoire",S_IRWXU)){
		perror("rmdir");
	}

	//création et allocation des structures qui permettent de calculer les temps du benchmark:
	timer *t = timer_alloc();
	recorder *record = recorder_alloc("readdir.csv");
	recorder *record2 = recorder_alloc("readdirfull.csv");

	//ouvrir le dossier / répertoire 
	DIR *rep = NULL;
	rep = opendir(nameDossier);
	if(rep == NULL){
		perror("Problème lors de l'ouverture du fichier");
	}

	struct dirent* fichierLu = NULL; 
	//maintenant qu'on a ouvert le répertoire, il faut créer des fichier
	//de la même taille (ici, on va d'abord en créer des vides puis des remplis de LONGUEUR_FICHIER_ALLOUE char)

	// Permet de créer la racine commune aux noms de fichiers
	char nameFichierFinal[255];
	char namePrevFichier[255];
	sprintf((char *) &namePrevFichier, "%s%s",nameDossier,"/fichier");
	
	int i;
	int k;
	int N;
	// -------------------------------------------------------------------------------------------------------

	printf("On commence avec les fichiers vides \n");

	// On crée tous les fichiers dans un dossier
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++){
		// On génère le nom de chaque fichier
		sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
		
		// On n'utilise pas la fonction de 'copy.h' parce qu'elle remplit les fichiers alors qu'on ne le veut pas forcément
		create_file(nameFichierFinal, 0);

	}

	printf("Fichiers créés \n");

	// ----------------------------------------------------------------------------------------------------------

	// Maintenant qu'on a créé les fichiers, on revient au début du dossier
	rewinddir(rep);
	// On initialise les variables qui représenteront le temps, ceci afin d'être plus exact sur les temps de calculs
	int a;
	int b;
	int tot = 0;

	// On fait une boucle pour que readdir puisse lire de plus en plus de fichiers du dossier
	for (k = 0; k <= NOMBRE_MAX_FICHIERS; k++)
	{
			// On calcule exactement le temps nécessaire
			a = stop_timer(t);
			readdir(rep);
			b = stop_timer(t);
			tot = tot+(b-a);
			// qu'on stocke dans le fichier avec la fonction fournie
			write_record(record, k, tot);

	}


	printf("Fini les fichiers vides \n");

	// Effacement de tous les fichiers
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++)
		{
			// on regénère les différents noms de fichiers
			sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
			
			// On supprime le fichier avec la méthode donnée de copy.c .
			rm(nameFichierFinal);
		}

	// ----------------------------------------------------------------------------------------------------------
	// Cette partie est exactement la-même que la précédente à la différence que les fichiers ont une place réservée

	printf("On commence avec les fichiers remplis \n");

	// On crée tous les fichiers dans un dossier
	for(i=0;i<NOMBRE_MAX_FICHIERS;i++){

		sprintf((char *) &nameFichierFinal, "%s%i.txt",namePrevFichier,i);
		
		// On utilise la fonction de 'copy.h' parce qu'elle permet d'allouer de l'espace aux fichiers
		create_file(nameFichierFinal, LONGUEUR_FICHIER_ALLOUE);

	}

	printf("Fichiers remplis créés \n");

	// ----------------------------------------------------------------------------------------------------------
	// Exactement la même chose que précédemment
	rewinddir(rep);
	start_timer(t);
	a = 0;
	b = 0;
	tot = 0;

	// Mesure du temps de readdir pour un nombre croissant de fichiers
	for (k = 0; k <= NOMBRE_MAX_FICHIERS; k++)
	{
			a = stop_timer(t);
			readdir(rep);
			b = stop_timer(t);
			tot = tot+(b-a);
			// On sauve juste les résultats dans un deuxième
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

	// On ferme le répertoire
	if(closedir(rep) == -1)
	{
		return -1; // EXIT_FAILURE
	}

	//suppression du répertoire (qui est vide):
	if(rmdir(nameDossier))
	{
		perror("rmdir");
	}

	// On désalloue les objets de benchmark afin d'éviter tout memory leak
	recorder_free(record);
	recorder_free(record2);
	timer_free(t);

	printf("Le programme a fini... \n");

	return 0;
}
