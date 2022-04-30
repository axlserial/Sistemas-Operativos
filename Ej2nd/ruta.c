#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

#define RUTA 255

/*
	Programa 2:
	Realizar el comando 'ls' con los parametros
	'la' para cada directorio de forma recursiva.
*/


int main(){
	char ruta[RUTA + 1];
	DIR* directorio;
	struct dirent* archivo;

	if (getcwd(ruta, RUTA + 1) == NULL){
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

	printf("Ruta actual: %s\nDatos del directorio:\n", ruta);

	directorio = opendir(ruta);

	if (directorio == NULL){
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	while((archivo = readdir(directorio)) != NULL){
		printf("%s: ", archivo->d_name);

		switch (archivo->d_type){
			case DT_DIR:
				printf("directorio\n");
			break;

			default:
				printf("Otro\n");
			break;
		}
	}

	closedir(directorio);

	return EXIT_SUCCESS;
}