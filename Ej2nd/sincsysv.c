/**** Programa para mostrar el uso de semáforos en la sincronización de procesos,
tomado de la página 389 del libro de: UNIX Programación Avanzada. F.M. Márquez ******/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEM_HIJO 0
#define SEM_PADRE 1

int main(int argc, char *argv[])
{
	int i = 1000, semid, pid;
	struct sembuf operacion;
	key_t llave;
	llave = ftok(argv[0], 'k');

	if ((semid = semget(llave, 2, IPC_CREAT | 0600)) == -1)
	{
		perror("semget");
		exit(-1);
	}
	
	semctl(semid, SEM_HIJO, SETVAL, 0);
	semctl(semid, SEM_PADRE, SETVAL, 1);
	
	if ((pid = fork()) == -1)
	{
		perror("fork");
		exit(-1);
	}
	
	else if (pid == 0)
	{
		while (i)
		{
			// Cerramos el semáforo del proceso hijo
			operacion.sem_num = SEM_HIJO;
			operacion.sem_op = -1;
			operacion.sem_flg = 0;
			semop(semid, &operacion, 1);
			printf("Proceso hijo:%d\n", i--);
			// Abrimos el semáforo del proceso padre
			operacion.sem_num = SEM_PADRE;
			operacion.sem_op = 1;
			semop(semid, &operacion, 1);
		}

		// Borramos el semáforo
		semctl(semid, 0, IPC_RMID, 0);
	}
	
	else
	{
		while (i)
		{
			// Cerramos el semáforo del proceso padre
			operacion.sem_num = SEM_PADRE;
			operacion.sem_op = -1;
			operacion.sem_flg = 0;
			semop(semid, &operacion, 1);
			printf("Proceso padre:%d\n", i--);
			// Abrimos el semáforo del proceso hijo
			operacion.sem_num = SEM_HIJO;
			operacion.sem_op = 1;
			semop(semid, &operacion, 1);
		}

		// Borramos el semáforo
		semctl(semid, 0, IPC_RMID, 0);
	}
}