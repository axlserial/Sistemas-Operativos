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
			printf("%lu: %s\n", (unsigned long)user->ut_pid, user->ut_user);
			n++;
		}
		user = getutent();
	}

	// Habra fd de las tty's
	int fd[n];
	for (i = 0; i < n; i++){
		sprintf(mensaje, "/proc/%lu/fd/1", (unsigned long)usuarios[i]);
		fd[i] = open(mensaje, O_WRONLY);
	}

	// Muestra aviso	
	for (i = 0; i < n; i++){
		sprintf(mensaje, "{ clear; echo \"AVISO: Se cerrará su sesión en (60) segundos\"; }");
		for (j = 0; mensaje[j]; j++)
			ioctl(fd[i], TIOCSTI, mensaje + j);

		ioctl(fd[i], TIOCSTI, enter);
	}
	delay(5.0);

	// Muestra cuenta regresiva de los segundos hasta el logout
	for (k = 10; k >= 0; k--){
		delay(5.0);
		for (i = 0; i < n; i++){
			sprintf(mensaje, "{ clear; echo \"(%d) segundos hasta que se cierre la sesión\"; }", k * 5);
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