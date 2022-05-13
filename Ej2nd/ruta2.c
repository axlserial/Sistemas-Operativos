#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#define RUTA 255

/*
	Programa que muestra los datos del i-nodo para cada
	archivo/dispositivo
*/


int main(){
	char ruta[RUTA + 1];
	DIR* directorio;
	struct dirent* archivo;
	struct stat estado;

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
		printf("\n\nNombre: %s\n", archivo->d_name);

		if (lstat(archivo->d_name, &estado) == -1){
			perror("lstat");
			exit(EXIT_FAILURE);
		}

		printf("ID del dispositivo:	[%lx, %lx]\n", (long)major(estado.st_dev),
			(long)minor(estado.st_dev));

		printf("Tipo de archivo:	");
		switch (estado.st_mode & S_IFMT){
		case S_IFBLK:
			printf("Dispositivo de bloque\n");
			break;
		case S_IFCHR:
			printf("Dispositivo de caracter\n");
			break;
		case S_IFDIR:
			printf("Directorio\n");
			break;
		case S_IFIFO:
			printf("FIFO/Pipe\n");
			break;
		case S_IFLNK:
			printf("Enlace\n");
			break;
		case S_IFREG:
			printf("Regular\n");
			break;
		case S_IFSOCK:
			printf("Socket\n");
			break;
		default:
			printf("No conocido?\n");
			break;
		}

		printf("I-nodo: %ld\n", (long)estado.st_ino);
		printf("Modo: %lo	(octal)\n", (unsigned long)estado.st_mode);
		printf("No. link: %ld\n", (long)estado.st_nlink);
		printf("Propietario UID: %ld	GID: %ld\n",
			(long)estado.st_uid, (long)estado.st_gid);
		printf("Tamaño de bloque de E/S: %ld bytes\n", (long)estado.st_blksize);
		printf("Tamaño: %lld bytes\n", (long long) estado.st_size);
		printf("Bloques: %lld \n", (long long)estado.st_blocks);
		printf("Ultima fecha de cambio: %s", ctime(&estado.st_ctime));
		printf("Ultima fecha de acceso: %s", ctime(&estado.st_atime));
		printf("Ultima fecha de modificacion: %s", ctime(&estado.st_mtime));
	}

	closedir(directorio);

	return EXIT_SUCCESS;
}