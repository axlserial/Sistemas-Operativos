#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>
#include <sys/stat.h>
#include <sys/types.h>

void login_usuario(char *);

int main(int argc, char* argv[]){
	char *usuario;

	if (argc != 2){
		printf("Necesitas pasar el login del usuario\n");
		exit(EXIT_FAILURE);
	}

	usuario = argv[1];
	login_usuario(usuario);

	return EXIT_SUCCESS;
}

void login_usuario(char *usuario){
	struct utmp *actual;
	int encontrado = 0;
	char *nombre;

	setutent();
	actual = getutent();
	while (actual != NULL){
		nombre = malloc(32 * sizeof(char));
		sprintf(nombre, "%s", actual->ut_user);
		if (actual->ut_type == USER_PROCESS && 
			strcmp(nombre, usuario) == 0){
			
			encontrado = 1;
			break;
		}
		free(nombre);
		actual = getutent();
	}

	if (encontrado){
		printf("Usuario '%s' conectado a: %s\n", actual->ut_user, actual->ut_id);
	} else {
		printf("Usuario no conectado\n");
	}
}