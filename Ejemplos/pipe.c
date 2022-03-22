#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
	fd[0]: lectura
	fd[1]: escritura 
*/

int main(){

	pid_t hijo;
	int n, estado, fd[2];
	char buffer[13];

	if (pipe(fd) < 0){
		perror("Error de Pipe");
		exit(1);
	} else {
		hijo = fork();
		if (hijo == 0){
			close(fd[0]);	// cerrar lectura
			write(fd[1], "Hola, padre!", 13);	// escribir en pipe
			exit(0);
		} else {
			if (wait(&estado) != hijo){
				perror("Error en cierre de hijo");
				exit(1);
			} else {
				close(fd[1]);	// cierra escritura
				n = read(fd[0], buffer, 13);
				printf("Mi hijo dijo: %s\n", buffer);
				printf("Chars leídos: %d\n", n);
				exit(0);
			}
		}
	}
}