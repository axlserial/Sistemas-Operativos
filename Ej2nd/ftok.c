#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>

#define SEM_HIJO 0
#define SEM_PADRE 1

int main(int argc, char *argv[]){

	int i = 1000000, semid;
	pid_t hijo;
	struct sembuf operacion;
	key_t llave;

	// Creación de llave
	llave = ftok(argv[0], 'a');
	if ((semid = semget(llave, 2, IPC_CREAT | 0600)) == -1){
		perror("semget");
		exit(-1);
	}

	semctl(semid, SEM_HIJO, SETVAL, 0);		// cerrar semáforo
	semctl(semid, SEM_PADRE, SETVAL, 1);	// abrir semáforo

	hijo = fork();

	// Erroe al crear hijo
	if (hijo == -1){
		perror("fork");
		exit(-1);
	}

	// Hijo
	if (hijo == 0){
		while (i){
			// Cerramos el semáforo del hijo
			operacion.sem_num = SEM_HIJO;
			operacion.sem_op = -1;
			operacion.sem_flg = 0;
			semop(semid, &operacion, 1);

			// Sección crítica
			printf("Proceso hijo: %d\n", i--);

			// Abrimos el semáforo del padre
			operacion.sem_num = SEM_PADRE;
			operacion.sem_op = 1;
			semop(semid, &operacion, 1);
		}

		// Borramos el semáforo
		semctl(semid, 0, IPC_RMID, 0);
	} 
	
	// Padre
	else {
		while (i){
			// Cerramos el semáforo del padre
			operacion.sem_num = SEM_PADRE;
			operacion.sem_op = -1;
			operacion.sem_flg = 0;
			semop(semid, &operacion, 1);

			// Sección crítica
			printf("Proceso padre: %d\n", i--);

			// Abrimos el semáforo del hijo
			operacion.sem_num = SEM_HIJO;
			operacion.sem_op = 1;
			semop(semid, &operacion, 1);
		}

		// Borramos el semáforo
		semctl(semid, 0, IPC_RMID, 0);
	}

	return EXIT_SUCCESS;
}