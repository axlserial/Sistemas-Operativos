#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#define RUTA 255

/*
	Programa que muestra los datos como si fuera
	el comando: ls -la, listando todos los
	subdirectorios de forma recursiva.
*/

void listaDirectorio(char *);

int main(){
	char ruta[RUTA + 1];

	// Obtener ruta del directorio actual
	if (getcwd(ruta, RUTA + 1) == NULL){
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

	// Mostrar información y archivos del directorio
	printf("Ruta inicial: %s:\n", ruta);
	listaDirectorio(ruta);

	return EXIT_SUCCESS;
}

void listaDirectorio(char *rutaDir){
	DIR* directorio;
	struct dirent* archivo;
	struct stat estado;
	char *actdir, *subdir;
	long tam_total;
	int8_t superdir;
	
	// Abre directorio
	directorio = opendir(rutaDir);

	if (directorio == NULL){
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	// Muestra información de sus archivos
	while((archivo = readdir(directorio)) != NULL){
		superdir = 0;
		
		// Obtiene ruta del archivo
		tam_total = strlen(rutaDir) + strlen(archivo->d_name) + 2;
		actdir = malloc(tam_total * sizeof(char));
		sprintf(actdir, "%s/%s", rutaDir, archivo->d_name);

		// Obtiene información del archivo
		if (lstat(actdir, &estado) == -1){
			perror("lstat");
			exit(EXIT_FAILURE);
		}

		free(actdir);

		// Muestra información del archivo
		printf("Nombre: %s\n", archivo->d_name);
		printf("Permisos: ");
    	printf(((estado.st_mode & S_IFMT) == S_IFDIR) ? "d" : "-");
    	printf((estado.st_mode & S_IRUSR) ? "r" : "-");
    	printf((estado.st_mode & S_IWUSR) ? "w" : "-");
    	printf((estado.st_mode & S_IXUSR) ? "x" : "-");
    	printf((estado.st_mode & S_IRGRP) ? "r" : "-");
    	printf((estado.st_mode & S_IWGRP) ? "w" : "-");
    	printf((estado.st_mode & S_IXGRP) ? "x" : "-");
    	printf((estado.st_mode & S_IROTH) ? "r" : "-");
    	printf((estado.st_mode & S_IWOTH) ? "w" : "-");
    	printf((estado.st_mode & S_IXOTH) ? "x\n" : "-\n");
		printf("No. link: %ld\n", (long)estado.st_nlink);
		printf("Propietario: %s\n", getpwuid(estado.st_uid)->pw_name);
		printf("Grupo: %s\n", getgrgid(estado.st_gid)->gr_name);
		printf("Tamaño: %lld bytes\n", (long long) estado.st_size);
		printf("Block size: %ld\n", (long)estado.st_blksize);
		printf("Bloques usados: %ld\n", (long)estado.st_blocks);
		printf("Ultima fecha de cambio: %s", ctime(&estado.st_ctime));
		printf("Ultima fecha de acceso: %s", ctime(&estado.st_atime));
		printf("Ultima fecha de modificacion: %s\n", ctime(&estado.st_mtime));

		// Sí el archivo actual es directorio, comprueba que no sea "." o ".."
		tam_total = strlen(archivo->d_name);

		if (tam_total == 1){
			superdir = strncmp(archivo->d_name, ".", 1) == 0;
		}

		if (tam_total == 2){
			superdir = strncmp(archivo->d_name, "..", 2) == 0;
		}

		if (tam_total == 4){
			superdir = strncmp(archivo->d_name, ".git", 4) == 0;
		}

		// Sí es directorio y no es alguno de los antes comprobados 
		if ((estado.st_mode & S_IFMT) == S_IFDIR && superdir == 0){
			// Obtiene ruta del subdirectorio a entrar
			tam_total += strlen(rutaDir) + 2;
			subdir = malloc(tam_total * sizeof(char));
			sprintf(subdir, "%s/%s", rutaDir, archivo->d_name);
			printf("-----------\nEntrando a subdirectorio: %s\n", subdir);

			//llamada recursiva
			listaDirectorio(subdir);
			
			printf("-----------\nRegresando a directorio: %s\n", rutaDir);
			free(subdir);
		}

	}

	closedir(directorio);
}