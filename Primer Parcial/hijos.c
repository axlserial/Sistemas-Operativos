/*
*	Documentación del programa 'hijos.c'
*
*	Descripción:	Programa que recibe desde linea de comandos la cantidad de procesos
*					hijos a crear, el proceso padre despues de crear a los n hijos
*					espera su conclusión e imprime el PID del hijo que terminó, además
*					el hijo debe de imprimir su número de hijo (no PID).
*
*	Modo de compilación: gcc hijos.c -o hijos
*
*	Modo de ejecución:	./hijos <num-hijos> 
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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
	int cant_hijos, estado;

	// verifica el argumento
	if (argc == 1 || argc > 2){
		printf("Debes colocar como unico argumento el numero de hijos\n");
		exit(1);
	}

	// establece el numero de hijos a crear
	cant_hijos = atoi(argv[1]);
	// pid_t hijos[cant_hijos];
	pid_t hijo;

	int i;
	for (i = 1; i <= cant_hijos; i++){
		hijo = fork();

		if (hijo == 0){
			break;
		}
	}

	if (hijo == 0){
		fprintf(stdout,"Soy el hijo numero: %d\n", i);
		exit(0);
	} else {
		hijo = wait(&estado);
		do {
			if (hijo > 0){
				fprintf(stdout,"Termino mi hijo PID: %ld\n", (long)hijo);
			}
			hijo = wait(&estado);
		} while(hijo != -1);
	}
	
}