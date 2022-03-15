#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
	int cant_hijos, estado;

	// verifica el argumento
	if (argc == 1 || argc > 2){
		printf("Debes colocar como unico argumento el numero de hijos\n");
		exit(1);
	}

	// establece el numero de hijos a crear
	cant_hijos = atoi(argv[1]);
	// pid_t hijos[cant_hijos];
	pid_t hijo;

	int i;
	for (i = 1; i <= cant_hijos; i++){
		hijo = fork();

		if (hijo == 0){
			break;
		}
	}

	if (hijo == 0){
		fprintf(stdout,"Soy el hijo numero: %d\n", i);
		exit(0);
	} else {
		hijo = wait(&estado);
		do {
			if (hijo > 0){
				fprintf(stdout,"Termino mi hijo PID: %ld\n", (long)hijo);
			}
			hijo = wait(&estado);
		} while(hijo != -1);
	}
	
}