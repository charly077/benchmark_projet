/**
*  Premier benchmark pour le projet3 de sinf1252
*  Author Stephane Kimmel & Charles Jacquet
*  Le but de ce benchmark est de voir l'efficacité de readdir
*  Et de créer un graphique affichable en html.
*
*  ATTENTION COMPRENDRE LA COMPILATION !!! 
*  ATTENTION COMPRENDRE DANS QUEL CAS C EST PLUS LONG !!!
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

void rand_str(char *dest, int length) {
    char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}


int main(int argc, char *argv[]){
	//création du répertoire :
	if(mkdir("./repertoire",S_IRWXU)){
		perror("rmdir");
	}
	//création des variable de calcul benchmark:
	timer *t = timer_alloc();
	recorder *record = recorder_alloc("readdir.csv");
	recorder *record2 = recorder_alloc("readdirfull.csv");

	// !!! Variables à changer
	int longueurChaineGene = 10000;
	char* cheminDuDossier = "repertoire";
	int nombreDeFichierMaxCree = 10000;

	//ouvrir un document 
	DIR *rep = NULL;
	rep = opendir(cheminDuDossier);
	if(rep == NULL){
		perror("Problème lors de l'ouverture du fichier");
	}
	struct dirent* fichierLu = NULL; 
	//maintenant qu'on a ouvert le répertoire, il faut créer des fichier
	//de la même taille
	
	//ex pour 10 100 1000 10 000 100 000 fichiers
	int N;
	char fichier[255];
	char name[255]; //= strcat(cheminDuDossier, "fichier");
	sprintf((char *) &name, "%s%s",cheminDuDossier,"/fichier");
	FILE *f;
	int i;
	// -----------------------------------------------------------------------------------------

	printf("On commence avec les fichiers vides \n");

	for(N=10;N<=nombreDeFichierMaxCree;N=10*N){
		

		// On crée tous les fichiers dans un dossier
		for(i=0;i<N;i++){

			sprintf((char *) &fichier, "%s%i.txt",name,i);
		
			f = fopen(fichier,"w");
			if (f == NULL){
				perror("problème lors de l'ouverture du fichier");
			}
			else{
				if(fclose(f))
					perror("problème lors de la fermeture d'un fichier");
			}

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
		write_record_n(record,N,stop_timer(t),N);
		//write_record(recordTot, N, stop_timer(t));
		printf("test fini pour N=%d\n",N);
	}

	//supression de tous les fichiers:

	for(i=0;i<N;i++){

		sprintf((char *) &fichier, "%s%i.txt",name,i);
		
		if(remove(fichier));
	}

	printf("Fini les fichiers vides \n");

	// ----------------------------------------------------------------------------------------------------------

	printf("On commence avec les fichiers remplis \n");

	//timer *t2 = timer_alloc();
	
	char buf[longueurChaineGene]; // Chaine à placer dans le fichier

	for(N=10;N<=nombreDeFichierMaxCree;N=10*N){
		int i;

		for(i=0;i<N;i++){

			sprintf((char *) &fichier, "%s%i.txt",name,i);
		
			f = fopen(fichier,"w");
			if (f == NULL){
				perror("problème lors de l'ouverture du fichier");
			}
			
			rand_str(buf, longueurChaineGene);
			
			fprintf(f, "%s", buf);

			if(fclose(f))
			{
				perror("problème lors de la fermeture d'un fichier");
			}

		}
		rewinddir(rep);
		start_timer(t);
		while((fichierLu = readdir(rep))!=NULL)
		{
			//printf("Le fichier lu s'appelle '%s'\n", fichierLu->d_name);
		}
		//j'utilise le record_n parce qu'il faut diviser par le nombre d'de fichier parce que qui est
		//interessant c'est le temps par fichier
		write_record_n(record2,N,stop_timer(t),N);
		printf("test fini pour N=%d\n",N);
	}
	//supression de tous les fichiers:

	for(i=0;i<N;i++){

		sprintf((char *) &fichier, "%s%i.txt",name,i);
		
		if(remove(fichier));
	}

	if(closedir(rep) == -1)
	{
		return -1; // EXIT_FAILURE
	}
	//suppression du répertoire (vide):
	if(rmdir("./repertoire")){
		perror("rmdir");
	}

	recorder_free(record);
	//recorder_free(recordTot);
	timer_free(t);

	printf("Le programme a fini... \n");

	return 0;
}
