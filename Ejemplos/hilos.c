/* Crea un hilo para calcular el factorial de un número que se pasa
*** como argumento desde línea de comandos
*******  hilos <numero>
*****modo de compilar: gcc -Wall hilos.c -lpthread -o hilos */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int id;
	int fact;
	long prod;
} hiloDato;

void *factorial (void *valor);

int main (int argc, char *argv[]){

	if (argc < 2 || argc > 10){
  		fprintf(stderr, "Uso: ./hilos <entero 1> <entero 2> ... <entero 10>\n");
		return -1;
 	}

	pthread_t tid[argc - 1];
	pthread_attr_t attr;
	hiloDato hilos[argc - 1];

	for (int j = 0; j < argc - 1; j++){
		// Coloca atributo predeterminados	
		pthread_attr_init(&attr);
		
		hilos[j].fact = atol(argv[j + 1]);

		// Crear hilo
		pthread_create(&tid[j], &attr, factorial, &hilos[j]);
	}

	for (int j = 0; j < argc - 1; j++){
		pthread_join(tid[j], NULL);
	}

	for (int j = 0; j < argc - 1; j++){
		printf("tid: %u fact: %d prod: %ld\n",
			(unsigned int)hilos[j].id, hilos[j].fact, hilos[j].prod);
	}
	return EXIT_SUCCESS;
 }

void *factorial (void *valor){
	int i = 1;

	hiloDato *item;
	item = (hiloDato*)valor;
	item->prod = 1;
	item->id = pthread_self();

	while (i <= item->fact)
		item->prod *= (i++);
 
	pthread_exit(0);
 }