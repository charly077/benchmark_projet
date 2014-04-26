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

#include "benchmark.h"

int main(int argc, char *argv[]){
	//création des variable de calcul benchmark:
	timer *t = timer_alloc();
	recorder *record = recorder_alloc("readdir.csv");
	
	//ouvrir un document 
	DIR *rep = NULL;
	rep = opendir("repertoire");
	if(rep == NULL){
		perror("Problème lors de l'ouverture du fichier");
	}
	//maintenant qu'on a ouvert le répertoire, il faut créer des fichier
	//de la même taille
	
	//ex pour 10 100 1000 10 000 100 000 10000000 fichiers
	int N;
	for(N=10;N<10000000;N=10*N){
		int i;
		char fichier[255];
		char name[]= "repertoire/Fichier";
		FILE *f;
		for(i=0;i<10;i++){

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
		rewinddir(rep);
		start_timer(t);
		while(readdir(rep)!=NULL){
		}
		//j'utilise le record_n parce qu'il faut diviser par le nombre d'de fichier parce que qui est
		//interessant c'est le temps par fichier
		write_record_n(record,N,stop_timer(t),N);
		printf("test fini pour N=%d\n",N);
	}
	recorder_free(record);
	timer_free(t);
	return 0;
}
