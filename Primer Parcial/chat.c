#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
// #include <sys/wait.h>
#include <unistd.h>

#define MAX_LONG 255
/*
	fd[0]: lectura
	fd[1]: escritura

	fd1 = padre
	fd2 = hijo 
*/

int main(){

	pid_t hijo;
	int n, estado, fd1[2], fd2[2];
	char buffer1[MAX_LONG], buffer2[MAX_LONG];
	char *p;

    sem_t *sem1 = sem_open("/semaforo1", 0x0008,  0644, 0);
    sem_t *sem2 = sem_open("/semaforo2", 0x0008,  0644, 1);

	if (pipe(fd1) < 0 || pipe(fd2) < 0){
		perror("Error de Pipe");
		exit(1);
	} else {
		hijo = fork();

		// hijo
		if (hijo == 0){
			while (1){
				sem_wait(sem1);

				printf("Padre entrada: "); 
				fgets(buffer2, MAX_LONG, stdin);
				
				p = strchr(buffer2, '\n');
				if (p != NULL){
					*p = '\0';
				}

				write(fd2[1], buffer2, MAX_LONG);	// escribir en pipe

				sem_post(sem2);

				if (strstr(buffer2, "mata kondo") != NULL){
					break;
				}
			}
		}
		
		// padre
		else {
			while (1){
				sem_wait(sem2);

				n = read(fd2[0], buffer1, MAX_LONG);
				
				printf("Mi hijo dijo: %s\n", buffer1);
				printf("Chars leídos: %d\n", n);

				sem_post(sem1);

				if (strstr(buffer1, "mata kondo") != NULL){
					exit(0);
				}
			}
		}
	}

	// cerrar semaforos
    sem_close(sem1);
    sem_unlink("/semaforo1");
    sem_close(sem2);
    sem_unlink("/semaforo2");

    return EXIT_SUCCESS;
}