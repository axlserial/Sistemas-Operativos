/*
*	Documentación del programa 'hilos.c'
*
*	Descripción:	Programa que crea un numero n de hilos de acuerdo a la cantidad que se escribe desde
					la linea de comandos. Cada hilo imprime su numero de hilo y el padre su id cuando termina.
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

typedef struct {
	int num;
	unsigned long id;
} hiloDato;

//Funcion del hilo que imprime en pantalla su numero de hilo
void *funcion(void *valor);

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
    hiloDato hilos[cant_hilos];

    //Crear los hilos
    for (int i = 0; i < cant_hilos; i++){
        pthread_attr_init(&attr); 

        // establece numero de hilo
        hilos[i].num = i + 1;

        pthread_create(&tid[i], &attr, funcion , &hilos[i]);
    }

    //Cerrar los hilos de acuerdo a la cantidad ingresada
    for (int i = 0; i < cant_hilos; i++) {
		//Cierra los hilos
        pthread_join (tid[i], NULL);
    }

    // imprime ID de los hilos
    for (int i = 0; i < cant_hilos; i++){
        fprintf(stdout,"Termino el hilo ID: %lu\n", hilos[i].id);
    }
	
    return EXIT_SUCCESS;
}

/*
*	Función: 	void *funcion(void *valor)
*	Descripción:	Función imprime en pantalla el numero de hilo del hilo actual
*                   y guarda su ID en la estructura.
*	Parámetros de entrada:	
*							void *valor:	Estructura que contiene los datos del hilo.
*	Retorno:	---
*/
void *funcion(void *valor) {
	hiloDato *datos;
	datos = (hiloDato *)valor;
    
    printf("Soy el hilo numero:  %d\n", datos->num);
    datos->id = (unsigned long)pthread_self();

    pthread_exit(0);
}