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
	pid_t usuarios[100];
	char mensaje[200];
	char *enter = "\n";
	int n = 0, i, j, k;

	// Obtiene datos del usuario actual
	char *actual = getenv("SUDO_USER");

	if (actual == NULL){
		printf("EL programa debe ejecutarse con SUDO\n");
		exit(EXIT_FAILURE);
	}

	// Obtiene len del nombre de usuario
	int tam = strlen(actual);

	// Obtiene usuarios conectados
	setutent();
	user = getutent();
	while (user != NULL){
		if (user->ut_type == USER_PROCESS && 
			strncmp(user->ut_user, actual, tam) != 0){
			
			usuarios[n] = user->ut_pid;
			n++;
		}
		user = getutent();
	}

	// Sí no hay usuarios conectados
	if (n == 0){
		printf("Sin usuarios conectados a las TTY's\n");
		exit(EXIT_FAILURE);
	}

	// Habra fd de las tty's
	int fd[n];
	for (i = 0; i < n; i++){
		sprintf(mensaje, "/proc/%lu/fd/1", (unsigned long)usuarios[i]);
		fd[i] = open(mensaje, O_WRONLY);
	}

	// Muestra aviso	
	time_t horario = time(NULL);;
	for (i = 0; i < n; i++){
		sprintf(mensaje, 
			"{ clear; echo \"AVISO: Se terminará su sesión en (60) segundos. Fecha y hora del sistema: %.24s.\"; }",
			ctime(&horario));
		for (j = 0; mensaje[j]; j++)
			ioctl(fd[i], TIOCSTI, mensaje + j);

		ioctl(fd[i], TIOCSTI, enter);
	}
	delay(5.0);

	// Muestra cuenta regresiva de los segundos hasta el logout
	for (k = 10; k >= 0; k--){
		delay(5.0);
		horario = time(NULL);
		for (i = 0; i < n; i++){
			sprintf(mensaje, 
				"{ clear; echo \"Fecha y hora del sistema: %.24s. (%d) segundos hasta que se termine la sesión.\"; }", 
				ctime(&horario), k * 5);
			for (j = 0; mensaje[j]; j++)
				ioctl(fd[i], TIOCSTI, mensaje + j);

			ioctl(fd[i], TIOCSTI, enter);
		}
	}

	// Cierra sesión de usuarios en las tty's
	for (i = 0; i < n; i++){
		kill(usuarios[i], SIGKILL);
	}

	return EXIT_SUCCESS;
}

/*
*	Función: 		void delay(double segundos)
*	Descripción:	Función que crea un delay a partir los segundos dados por parámetro.
*	Parametro de entrada:	double segundos:	Cantidad de segundos a realizar el delay 				
*	Retorno:				---
*/
void delay(double segundos){

	// Guarda tiempo de inicio
    const time_t inicio = time(NULL);

    time_t actual;

	// Mientras que la diferencia de tiempo
	// sea menor a la cantidad de segundos dada
    do{
		// Obtiene el tiempo actual
        time(&actual);
    }while(difftime(actual, inicio) < segundos);

}