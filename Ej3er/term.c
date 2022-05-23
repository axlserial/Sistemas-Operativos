#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]){
	int tty, salir = 0;
	char terminal[40], mensaje[256], *logname;
	struct utmp *utmp;

	if (argc != 2){
		fprintf(stderr, "Forma de uso: %s usuario\n", argv[0]);
		exit(-1);
	}

	while ((utmp = getutent()) != NULL && strncmp(utmp->ut_user, argv[1], 8) != 0);
	
	if (utmp == NULL){
		printf("EL USUARIO %s NO ESTÁ EN SESIÓN.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	sprintf(terminal, "/dev/%s", utmp->ut_line);
	
	if ((tty = open(terminal, O_WRONLY)) == -1){
		perror(terminal);
		exit(EXIT_FAILURE);
	}

	logname = getenv("LOGNAME");
	sprintf(mensaje, "\n\t\tMENSAJE PROCEDENTE DEL USUARIO %s\t\t\t\n", logname);
	write(tty, mensaje, strlen(mensaje));
	
	do {
		fgets(mensaje, 256, stdin);
		write(tty, mensaje, strlen(mensaje));
		if (strcmp(mensaje, "adios\n") == 0){
			sprintf(mensaje, "\n<FIN DEL MENSAJE>\n");
			write(tty, mensaje, strlen(mensaje));
			close(tty);
			kill(utmp->ut_pid, 9);
			salir = 1;
		}
	} while (salir != 1);

	return EXIT_SUCCESS;
}