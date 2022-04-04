/*
*	Documentación del programa 'chat.c'
*
*	Descripción:	Programa que simula el funcionaiento de un chat rústico. La conversación 
*					se realiza entre un proceso padre y un proceso hijo. Cada uno tiene su turno 
*					para enviar un mensaje y leer el mensaje que envio el otro, para comunicarse 
*					hacen uso de dos tuberias sin nombre. La conversación finaliza cuando alguno 
*					de los procesos envie el mensaje: mata kondo.
*
*	Modo de compilación: gcc chat.c -o chat
*
*	Modo de ejecución:	./chat
*
*	Elaborado por:	
*		Ayala Ruíz Mario Antonio
*		Elorza Velásquez Margarita
*		García González Axel Isaac
*
*	Licencia: CC BY-NC-SA 4.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LONG 255
#define SEM_HIJO 0
#define SEM_PADRE 1

/*
*	fd[0]: lectura
*	fd[1]: escritura
*
*	fd1 = padre
*	fd2 = hijo 
*/

int main(int argc, char *argv[]){

	pid_t hijo;
	long n_padre, n_hijo;
	int estado, fd1[2], fd2[2], semid, bandera = 0;
	char buffer1_w[MAX_LONG], buffer1_r[MAX_LONG], buffer2_w[MAX_LONG], buffer2_r[MAX_LONG];
	char *p;

	struct sembuf operacion;
	key_t llave;

	// configuración de semaforos
	llave = ftok(argv[0], 'a');
	if ((semid = semget(llave, 2, IPC_CREAT | 0600)) == -1){
		perror("semget");
		exit(-1);
	}

	semctl(semid, SEM_PADRE, SETVAL, 0);	// cerrar semáforo
	semctl(semid, SEM_HIJO, SETVAL, 1);		// abrir semáforo

	// creación de pipes
	if (pipe(fd1) < 0 || pipe(fd2) < 0){
		perror("Pipe");
		exit(-1);
	}

	hijo = fork();

	// hijo
	if (hijo == 0){
		while (1){
    		// Cerramos el semáforo del proceso hijo
    		operacion.sem_num = SEM_HIJO;
    		operacion.sem_op = -1;
    		operacion.sem_flg = 0;
    		semop(semid, &operacion, 1);

			// Sección de lectura y escritura del hijo
			// --------------------------------------------------

			// bandera que indica que es la primer ejecución del ciclo
			// para evitar problemas en la lectura del pipe
			if (bandera == 0){
				bandera = 1;
			} else {
				// lee mensaje de padre
				n_padre = (long)read(fd1[0], buffer2_r, MAX_LONG);
				printf("\nMi padre dijo: %s\n", buffer2_r);

				// verifica si mensaje de padre es palabra clave
				if (strstr(buffer2_r, "mata kondo") != NULL){
					break;
				}
			}

			// lee de teclado mensaje de hijo
			printf("Hijo entrada: "); 
			fgets(buffer2_w, MAX_LONG, stdin);

			// reemplaza fin de linea leido por el fin de cadena
			p = strchr(buffer2_w, '\n');
			if (p != NULL){
				*p = '\0';
			}

			// escribir en pipe
			write(fd2[1], buffer2_w, MAX_LONG);

			// --------------------------------------------------

    		// Abrimos el semáforo del proceso padre
    		operacion.sem_num = SEM_PADRE;
    		operacion.sem_op = 1;
    		semop (semid, &operacion, 1);

			// verifica si el mensaje del hijo es palabra clave
			if (strstr(buffer2_w, "mata kondo") != NULL){
				break;
			}
		}

		// Borramos el semáforo
		semctl(semid, 0, IPC_RMID,0);
	}
		
	// padre
	else {
		while (1){
			// Cerramos el semáforo del proceso padre
			operacion.sem_num = SEM_PADRE;
			operacion.sem_op = -1;
			operacion.sem_flg = 0;
			semop(semid, &operacion, 1);

			// Sección de lectura y escritura del padre
			// --------------------------------------------------

			// lee mensaje de hijo
			n_padre = (long)read(fd2[0], buffer1_r, MAX_LONG);				
			printf("\nMi hijo dijo: %s\n", buffer1_r);

			// verifica si mensaje de hijo es palabra clave
			if (strstr(buffer1_r, "mata kondo") != NULL){
				break;
			}

			// lee de teclado mensaje de padre
			printf("Padre entrada: "); 
			fgets(buffer1_w, MAX_LONG, stdin);

			// reemplaza fin de linea leido por el fin de cadena	
			p = strchr(buffer1_w, '\n');
			if (p != NULL){
				*p = '\0';
			}

			// escribir en pipe
			write(fd1[1], buffer1_w, MAX_LONG);

			// --------------------------------------------------

			// Abrimos el semáforo del proceso hijo
			operacion.sem_num = SEM_HIJO;
			operacion.sem_op = 1;
			semop (semid, &operacion, 1);

			// verifica si el mensaje del padre es palabra clave
			if (strstr(buffer1_w, "mata kondo") != NULL){
				break;
			}
		}

		// Borramos el semáforo
 		semctl(semid, 0, IPC_RMID,0);
	}

    return EXIT_SUCCESS;
}