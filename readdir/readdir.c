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


int main(int argc, char *argv[]){
	//ouvrir un document 
	DIR *rep = NULL;
	rep = opendir("repertoire");
	if(rep == NULL){
		perror("Problème lors de l'ouverture du fichier");
	}
	//maintenant qu'on a ouvert le répertoire, il faut créer des fichier
	//de la même taille
	
	//ex pour 10 fichiers
	int i;
	char fichier[255];
	char name[]= "Fichier";
	FILE *f;
	for(i=0;i<10;i++){

		if (sprintf(&fichier, "%s%i.txt",name,i)){
			perror("Il y a une erreur avec le sprintf");
		}
		f = fopen(fichier,"w");
		if (f == NULL){
			perror("problème lors de l'ouverture du fichier");
		}
		else{
			if(fclose(f))
				perror("problème lors de la fermeture d'un fichier");
			printf("Un fichier à été écrit ... \n");
		}

	}
	return 0;
}
