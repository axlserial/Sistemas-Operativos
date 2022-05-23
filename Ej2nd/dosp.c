#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(){

	pid_t hijo;
	int file, i = 0;
	char mensaje[20];

	hijo = fork();

	if (hijo == 0){
		fprintf(stdout, "Soy el hijo, ID = %ld\n", (long)getpid());
		if ((file = open("archivo", O_WRONLY)) == -1){
			perror("Error en open");
			exit(-1);
		}
		

		while(i < 10){
			write(file, "Soy el hijo, ID...\n", 20);
			i++;
		}

		close(file);
		getchar();
		exit(0);
	} else {
		fprintf(stdout, "Soy el padre, ID = %ld\n", (long)getpid());
		if ((file = open("archivo", O_RDONLY)) == -1){
			perror("Error en open");
			exit(-1);
		}
		
		while(i < 10){
			read(file, mensaje, 20);
			fprintf(stdout, "lect: %s\n", mensaje);
			i++;
		}

		close(file);
		getchar();
	}

}