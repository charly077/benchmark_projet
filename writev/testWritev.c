#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h> 
#include <errno.h>  
#include <string.h>  



int main(int argc,char *argv[])
{
	//time_t *t = timer_alloc();

	//recorder, sert à enregistrer le temps d'exécution
	//recorder *writev_rec = recorder_alloc("writev.csv");
	//recorder *write_rec = recorder_alloc("write.csv");

	// Je définis ce que seront les buffer à écrire
	char buffer1[] = "Salut tout le monde, ";
	char buffer2[] = "je suis ici pour une ";
	char buffer3[] = "(plus ou moins) bonne ";
	char buffer4[] = "raison, mais c'est compliqué...";
	char buffer5[] = "Donc à demain !!!";

	// Dans cette partie sont créés les fichiers qui vont servir d'output
	int fichierOutput;
	int fichierOutput2;


	fichierOutput = open(argv[1],O_CREAT | O_WRONLY,S_IRUSR | S_IWUSR);
    if(fichierOutput!=-1){
        printf("file has been created.\n");
    }

    fichierOutput2 = open(argv[2],O_CREAT | O_WRONLY,S_IRUSR | S_IWUSR);
    if(fichierOutput2!=-1){
        printf("file has been created.\n");
    }

    // On démarre le timer
    start_timer(t);

    // On initialise la structure iovec
    struct iovec iov[5];

    iov[0].iov_base = buffer1;
    iov[0].iov_len = strlen(buffer1);

    iov[1].iov_base = buffer2;
    iov[1].iov_len = strlen(buffer2);    

    iov[2].iov_base = buffer3;
    iov[2].iov_len = strlen(buffer3);

    iov[3].iov_base = buffer4;
    iov[3].iov_len = strlen(buffer4);

    iov[4].iov_base = buffer5;
    iov[4].iov_len = strlen(buffer5);

    ssize_t lenv;
    int longueurFlu;
    longueurFlu = sizeof(iov) / sizeof(struct iovec);

    // Appel de writev
    writev(fichierOutput1, iov, longueurFlu);

    //write_record(writev_rec, 1, stop_timer(t));

    //recorder_free(writev_rec);
    //timer_free(t);

    printf("Finished writev \n");

	return 0;
}
