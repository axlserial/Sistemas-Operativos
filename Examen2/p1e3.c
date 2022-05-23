#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/statvfs.h>

/*
	Agregar que muestre el tipo del sistema de archivo
	tambien, darle formato
*/

void inodos_libres(char *);

int main(int argc, char *argv[]){
	char *ruta;

	if (argc != 2){
		printf("Necesitas pasar el nombre del directorio\n");
		exit(EXIT_FAILURE);
	}

	ruta = argv[1];
	inodos_libres(ruta);

	return EXIT_SUCCESS;
}

void inodos_libres(char *directorio){
	struct statvfs vfs;

	if (statvfs(directorio, &vfs) != 0){
		perror("statvfs");
		exit(EXIT_FAILURE);
	}

	printf("Para el directorio %s:\n", directorio);
	printf("Número de inodos libres: %lu\n", (unsigned long)vfs.f_ffree);
}