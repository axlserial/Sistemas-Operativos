/*
*	Documentación del programa 'hilos.c'
*
*	Descripción:	Programa que crea un numero n de hilos de acuerdo a la cantidad que se escribe desde
					la linea de comandos. Cada hilo imprime su numero de hilo y su id.
*
*	Modo de compilación: gcc -Wall hilos.c -o hilos -lpthread
*
*	Modo de ejecución:	./hilos <num-hilos> 
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
#include <pthread.h>

//Funcion del hilo que imprime en pantalla su ID
void *funcion(void *dato);

int main (int argc, char const *argv[]){
    int cant_hilos;
    pthread_attr_t attr;

    // Checa que el número de argumentos no pase del numero de hilos a crear 
    if (argc == 1 || argc > 2){
		printf("Debes colocar como unico argumento el numero de hilos\n");
		exit(1);
	}

    // Obtener el numero de hilos a crear
	cant_hilos = atoi(argv[1]);
	//Crear un arreglo de hilos
    pthread_t tid[cant_hilos];
    //Crear los hilos
    for (int i=0; i < cant_hilos; i++){
        pthread_attr_init(&attr); 
        pthread_create(&tid[i], &attr, funcion , NULL);
    }

    //Cerrar los hilos de acuerdo a la cantidad ingresada
    for (int i=0; i < cant_hilos; i++) {
		//Cierra los hilos
        pthread_join (tid[i], NULL);
    }
	
    return EXIT_SUCCESS;
}

/*
*	Función: 	void *funcion(void *dato)
*	Descripción:	Función imprime en pantalla un mensaje con el ID del hilo asociado
*	Parámetros de entrada:	
*							void *dato:	valor(es) de los datos a trabajar o NULL.
*	Retorno:	---
*/
void *funcion(void *dato) {
	//Imprime en consola el ID del hilo
    printf("Soy el hilo con el ID asociado:  %u\n", (unsigned int)pthread_self());
    pthread_exit(0);
}