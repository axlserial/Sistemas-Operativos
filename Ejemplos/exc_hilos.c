/* 
	Crea un par de hilos para
	realizar sin exclusion mutua
	*****modo de compilar: gcc -Wall exc_hilos.c -lpthread -o ../bin/exc_hilos
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

void *funcion1(void *dato);
void *funcion2(void *dato);

pthread_mutex_t EM;

int main (int argc, char *argv[]){

	pthread_t hilo1, hilo2;
	pthread_attr_t attr;

	//
	pthread_attr_init(&attr);

	//
	pthread_create(&hilo1, &attr, funcion1, NULL);
	pthread_create(&hilo2, &attr, funcion2, NULL);

	printf("Padre-Proceso(%d) y cree dos hilos\n", getpid());

	//
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);

	return EXIT_SUCCESS;
 }

void *funcion1(void *dato){
	char buffer[100];
	char *c;

	sprintf(buffer, "Soy el hilo1: %u y estoy escribiendo un mensaje a pantalla", 
		(unsigned int)pthread_self());
	c = buffer;

	// Inicio de sección critica
	pthread_mutex_lock(&EM);
	// Sección critica
	while (*c != '\0'){
		fputc(*c, stdout);
		c++;
	}
	fputc('\n', stdout);
	// fin seccion critica
	pthread_mutex_unlock(&EM);
	
	pthread_exit(0);

}

void *funcion2(void *dato){
	char buffer[100];
	char *c;

	sprintf(buffer, "Soy el hilo2: %u y estoy escribiendo un mensaje a pantalla", 
		(unsigned int)pthread_self());
	c = buffer;

	// Inicio de sección critica
	pthread_mutex_lock(&EM);
	// Sección critica
	while (*c != '\0'){
		fputc(*c, stdout);
		c++;
	}
	fputc('\n', stdout);
	// fin seccion critica
	pthread_mutex_unlock(&EM);
	
	pthread_exit(0);
}