/* Crea un hilo para calcular el factorial de un número que se pasa
*** como argumento desde línea de comandos
*******  hilos <numero>
*****modo de compilar: gcc -Wall hilos.c -lpthread -o hilos */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long prod = 1;

void *factorial (void *valor);

int main (int argc, char *argv[]){

	if (argc != 2){
  		fprintf(stderr, "Uso: ./hilos <entero>\n");
		return -1;
 	}

	pthread_t tid;
	pthread_attr_t attr;

	// Coloca atributo predeterminados	
	pthread_attr_init(&attr);
		
	// Crear hilo
	pthread_create(&tid, &attr, factorial, argv[1]);
		
	// Esperar finalizado de hijo --- semejante a wait
	pthread_join(tid, NULL);

	printf("Factorial: %ld\n", prod);

	return EXIT_SUCCESS;
 }

void *factorial (void *valor){
	int i = 1;

	while (i <= atol(valor))
		prod *= (i++);
 
	pthread_exit(0);
 }