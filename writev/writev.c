#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>

#include "benchmark.h"
#include "copy.h"

/*

Le but de se programme est de comparer la vitesse de write/lseek par rapport à writev

@author Stephane Kimmel & Charles Jacquet

*/

#define NOMBRE_BUFFER_MAX 1000

int main(int argc, char *argv[]){
	//création d'un buffer de taille fixe à écrire
	char buf[] = "Bonjour, je ne sais pas quoi écrire\n";
	int buf_len = strlen(buf);
	int section_len = buf_len * 2; // on utilise a chaque fois une section pour écrire les buf et on ne rempli pas totalement les sections
	timer *t = timer_alloc();
	recorder *writev_rec = recorder_alloc("writev.csv");
	recorder *write_lseek_rec = recorder_alloc("write_lseek.csv");

	//ouverture/création des fichiers
	int fd1 = open("./writev_fd", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	int fd2 = open("./write_lseek_fd", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	

	// boucle pour copier i fois le buffer
	int i;
	int j;
	int last=0;
	for(i=1;i<NOMBRE_BUFFER_MAX;i++){ // il faut commencer à 2 
		if(lseek(fd1,0,SEEK_SET)==-1)
			perror("lseek");
		start_timer(t);
		struct iovec iov[i];
		for(j=0;j<i;j++){
			iov[j].iov_base = buf;
			iov[j].iov_len = section_len;
		}
		if(writev(fd1,iov, i)==-1)
			perror("writev");
		if(fsync(fd1)) // obliger l'écriture du buffer
			perror("fsync");
		write_record(writev_rec, i, stop_timer(t));

		// write/lseek à chaque fois il faut revenir à 0
		if(lseek(fd2,0,SEEK_SET)==-1)
			perror("lseek");
		start_timer(t);
		for(j=0;j<i;j++){
			if(lseek(fd2,j*section_len,SEEK_SET)==-1)
				perror("lseek");
			if(write(fd2,buf,buf_len)==-1)
				perror("write");
			if(fsync(fd2)) // obliger l'écriture du buffer à chaque write
				perror("fsync");
		}
		write_record(write_lseek_rec, i, stop_timer(t));
		
		int actual=100*i/NOMBRE_BUFFER_MAX;
		if (actual%10==0 && last!=actual ){
			printf("%d/100\n",actual);
			last = actual;
		}
	}
	
	
	//libération des recorder et du timer
	recorder_free(writev_rec);
	recorder_free(write_lseek_rec);
	timer_free(t);

	//fermeture des fichiers
	close(fd1);
	close(fd2);	
	//rm("./writev_fd");
	//rm("./write_lseek_fd");
}
