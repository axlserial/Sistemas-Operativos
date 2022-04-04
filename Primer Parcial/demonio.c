/*
*	Documentación del programa 'demonio.c'
*
*	Descripción:	Programa que crea un Demonio y que cada medio minuto escribe en un archivo
*					llamado 'textodemonio.txt' lo siguiente: SOY EL DEMONIO Y MI PID ES: <PID>.
*
*	Modo de compilación: gcc demonio.c -o demonio
*
*	Modo de ejecución:	./demonio 
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
#include <unistd.h>
#include <sys/stat.h>

int main (){
	FILE *file = NULL;
    pid_t pid;
	pid_t demon;
	//Crear el proceso de donde iniciaremos la sesion del demonio
	pid=fork();

	// Comprueba que el proceso no se erro
    if (pid < 0) {
        exit(1);
    }
	//Verifica si nos encontramos en el proceso padre
	if (pid > 0) {
		exit (0);
	}
	//Crear una nueva sesion
	umask(0);
	demon = setsid();
	//Verifica si la sesion se realizo de manera correcta 
    if (demon < 0 ){
        exit (1);
    }

	
	//Abrir archivo y refrescar el texto; una vez hecho esto lo cerramos
	file = fopen ("textodemonio.txt","w");
	fclose(file);

	//Abrir el archivo 
	file = fopen ("textodemonio.txt","a+");
    while (1){
		//Escribimos el mensaje en el archivo
        fprintf(file,"SOY EL DEMONIO Y MI PID ES: %ld\n",(long)getpid());
		fflush (file);
        sleep (30);
    }
	
	//Cerramos el archivo
	fclose (file);
    
	return EXIT_SUCCESS; 
}