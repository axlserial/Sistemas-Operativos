/*
 *	Documentación del programa 'terminal.c'
 *
 *	Descripción:		Programa que avisa a los usuarios conectados en las TTy's que en x tiempo (60 segundos)
 *						se cerrará su sesión, mandandoles mensaje cada x/y tiempo (30/6 = 5 segundos) en forma de recordatorio. 
 *						Cuando el tiempo indicado se cumpla, la sesión de todos los usuarios en las TTy's terminará.
 *
 *	Modo de compilación: gcc -Wall terminal.c -o terminal
 *
 *	Modo de ejecución:	sudo ./terminal
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
#include <string.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>


void delay(double dly);

int main(){
	struct utmp *user;

	// Obtiene datos del usuario actual
	char *actual = "axl";

	// Obtiene len del nombre de usuario
	int tam = strlen(actual);

	// Obtiene usuarios conectados
	setutent();
	user = getutent();
	while (user != NULL){
		if (user->ut_type == USER_PROCESS && 
			strncmp(user->ut_user, actual, tam) != 0){
				
			printf("Conectado a: %s\n", user->ut_id);
		}
		user = getutent();
	}

	return EXIT_SUCCESS;
}