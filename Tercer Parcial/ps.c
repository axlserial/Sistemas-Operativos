#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[]){
	DIR* directorio;
	struct dirent* archivo;
	long pid, ppid = (long)getppid(), ppid_leido;
	char ruta[256], nombre [256], linea[256], temp[10];

	FILE *archivo_proc;
	
	// Abre directorio
	directorio = opendir("/proc");

	if (directorio == NULL){
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	// Muestra información de procesos
	int i;
	while((archivo = readdir(directorio)) != NULL){
		
		// Obtiene PID del directorio
		pid = atol(archivo->d_name);

		// Sí es un PID válido
		i = 0;
		if (pid != 0){

			// Lee cmdline
			sprintf(ruta, "/proc/%ld/cmdline", pid);
			archivo_proc = fopen(ruta, "r");
			fgets(nombre, 256, archivo_proc);
			fclose(archivo_proc);

			// Abre achivo de status
			sprintf(ruta, "/proc/%ld/status", pid);
			archivo_proc = fopen(ruta, "r");
			while (!feof(archivo_proc)){
				fgets(linea, 256, archivo_proc);

				// Sí es la linea que tiene el PID
				if (i == 5){
					sscanf(linea, "%s\t%ld", temp, &ppid_leido);

					// Sí es el PID de la terminal desde donde se lanzó el programa
					if (ppid == ppid_leido){
						printf("\nPID: %ld\n", ppid_leido);
						printf("Nombre: %s\n", nombre);
					}
				}

				// Sí es la linea que tiene el PPID
				if (i == 6){
					sscanf(linea, "%s\t%ld", temp, &ppid_leido);

					// Sí el PPID del proceso es la terminal donde se lanzó el programa
					if (ppid == ppid_leido){
						printf("\nPID: %ld\n", pid);
						printf("Nombre: %s\n", nombre);
					}
				}

				i++;
			}

			fclose(archivo_proc);
		}
	}

	closedir(directorio);

	return 0;
}