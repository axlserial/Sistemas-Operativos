#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void nombre_archivo(char *);

int main(int argc, char *argv[]){
	char *nombre;

	if (argc != 2){
		printf("Necesitas pasar el nombre del archivo\n");
		exit(EXIT_FAILURE);
	}

	nombre = argv[1];
	nombre_archivo(nombre);

	return EXIT_SUCCESS;
}

void nombre_archivo(char *nombre){
	DIR* directorio;
	struct dirent* archivo;
	
	directorio = opendir(nombre);

	if (directorio != NULL){
		printf("Archivos en el directorio:\n");
		while((archivo = readdir(directorio)) != NULL){
			printf("%s\n", archivo->d_name);
		}
	} else {
		printf("El archivo no es directorio\n");
	}

	closedir(directorio);
}