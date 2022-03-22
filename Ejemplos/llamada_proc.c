#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){

	pid_t hijo;
	int estado;

	hijo = fork();

	if (hijo == 0){

		// llama al binario del programa 'var', si execl regresa un -1, hubo error
		if (execl("/home/axl/Uni/SO/Sistemas-Operativos/bin/var", "var", "uno", "2", NULL) < 0){
			perror("Error en excl");
			exit(1);
		}
		exit(0);
	} else {
		if (wait(&estado) != hijo){
			perror("Error de terminación del hijo");
			exit(1);
		}
	}
}