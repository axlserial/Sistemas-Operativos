#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[]){
	DIR* directorio;
	struct dirent* archivo;
	long pid;
	char ruta[256], nombre [256], linea[256], temp[10];
	FILE *archivo_proc;
	
	// Abre directorio
	directorio = opendir("/proc");

	if (directorio == NULL){
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	printf("PID\t\tNombre\n\n");

	// Muestra información de procesos
	while((archivo = readdir(directorio)) != NULL){
		
		// Obtiene PID del directorio
		pid = atol(archivo->d_name);

		// Sí es un PID válido
		if (pid != 0){

			// Abre achivo de status
			sprintf(ruta, "/proc/%ld/status", pid);
			archivo_proc = fopen(ruta, "r");
			
			// Guarda nombre del proceso
			fgets(linea, 256, archivo_proc);
			sscanf(linea, "%s\t%s", temp, nombre);

			// Imprime PID y nombre del proceso
			printf("%ld\t\t%s\n", pid, nombre);

			fclose(archivo_proc);
		}
	}

	closedir(directorio);

	return 0;
}