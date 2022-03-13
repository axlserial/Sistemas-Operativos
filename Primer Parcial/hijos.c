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
	pid_t hijos[cant_hijos];

	for (int i = 0; i < cant_hijos; i++){
		hijos[i] = fork();

		if (hijos[i] == 0){
			fprintf(stdout,"Soy el hijo numero: %d\n", i + 1);
			exit(0);
		} else {
			if (wait(&estado) == hijos[i]){
				fprintf(stdout,"Termino mi hijo %d PID: %ld\n", i + 1, (long)hijos[i]);
			}
		}
	}
	
}