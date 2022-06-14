#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t capacidadComedor;
sem_t tenedor[5];

char *pensadores[] = {"socrates", "platon", "aristoteles", "pitagoras", "heraclito"};

void pensar(int phil);
void comer(int phil);

void *filosofo(void *phil) {
	int comelon = *((int *)phil);
	
	while (1) {
		pensar(comelon);

		sem_wait(&capacidadComedor); // se revisa la capacidad del comedor
									 // y se disminuye en uno

		sem_wait(&tenedor[comelon]);		   // el filosofo toma el tenedor izquierdo
		sem_wait(&tenedor[(comelon + 1) % 5]); // el filosofo toma el tenedor derecho

		comer(comelon); // el filosofo come

		sem_post(&tenedor[(comelon + 1) % 5]); // el filosofo libera el tenedor de la derecha
		sem_post(&tenedor[comelon]);		   // el filosofo libera el tenedor de la izquierda

		sem_post(&capacidadComedor); // se aumenta en uno la capacidad del comedor
	}
}

int main() {
	pthread_t filosofos[5];
	int aux[5];

	sem_init(&capacidadComedor, 0, 4); // la capacidad del comedor se inicializa en cuatro

	for (int k = 0; k < 5; k++) {
		sem_init(&tenedor[k], 0, 1); // cada tenedor solamente se puede asignar a un filosofo
	}

	for (int k = 0; k < 5; k++) {
		aux[k] = k;
		// se crean los filosofos (procesos o hilos)
		pthread_create(&filosofos[k], NULL, filosofo, (void *)&aux[k]);
	}

	for (int k = 0; k < 5; k++) {
		// el hilo principal a que todos los subhilos terminen.
		pthread_join(filosofos[k], NULL);
	}
}

//------------------------------
void pensar(int phil) {
	printf("%d. %s esta pensando\n", phil + 1, pensadores[phil]);
}

void comer(int phil) {
	printf("%d. %s esta comiendo\n\n", phil + 1, pensadores[phil]);
	sleep(1);
}