#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define VALOR 1000

void *funcion1(void *valor);
void *funcion2(void *valor);

int contador = 0;

// semaforo
sem_t semaforo;

/*
	sem_init:	crea el semaforo

	valor de semaforo:
		1: habilitado
		0: deshabilitado

	sem_wait: 	pone en 0 el semaforo
				(bloquea semaforo)

	sem_post:	pone en 1 el semaforo
				(libera el semaforo)
*/

int main (){

	pthread_t tid1, tid2;
	pthread_attr_t attr;

	// crea el semaforo
	sem_init(&semaforo, 0, 1);

	// Coloca atributo predeterminados	
	pthread_attr_init(&attr);
		
	// Crear hilos
	pthread_create(&tid1, &attr, funcion1, NULL);
	pthread_create(&tid2, &attr, funcion2, NULL);
		
	// Esperar finalizado de hijos --- semejante a wait
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	printf("Valor de contador: %d\n", contador);

	return EXIT_SUCCESS;
}

void *funcion1(void *valor){
	for (int i = 0; i < VALOR; i++){
		sem_wait(&semaforo);
		contador += 1;
		sem_post(&semaforo);
	}

	pthread_exit(0);
}

void *funcion2(void *valor){
	for (int i = 0; i < VALOR; i++){
		sem_wait(&semaforo);
		contador -= 1;
		sem_post(&semaforo);
	}

	pthread_exit(0);
}