/*
*	Documentación del programa 'usuario.c'
*
*	Descripción:	Programa que recibe desde linea de comandos el login de un usuario
*					y se le asigna a procesos el buscar su información en el archivo
*					passwd, mandar un correo al usuario y crear un directorio en su
*					carpeta home con el nombre dado por línea de comandos.
*
*	Modo de compilación: gcc usuario.c -o usuario
*
*	Modo de ejecución:	./usuario <nombre-usuario> <nombre-directorio> 
*
*	Elaborado por:	
*		Ayala Ruíz Mario Antonio
*		Elorza Velásquez Margarita
*		García González Axel Isaac
*
*	Licencia: CC BY-NC-SA 4.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

// Prototipo de funciones
struct passwd *busca_datos_usr(char *usuario); 

int main(int argc, char *argv[]){

	// Verifica que se pase correctamente el usuario
	if (argc != 3){
		printf("Uso: ./usuario <nombre-usuario> <nombre-directorio>\n");
		exit(1);
	}

	int estado, status;
	char directorio[100], correo[100];
	pid_t hijo;
	struct passwd *p;

	// Obtener información de usuario
	p = busca_datos_usr(argv[1]);

	// Crea procesos hijos
	int i;
	for (i = 1; i <= 3; i++){
		hijo = fork();

		if (hijo == 0){
			break;
		}
	}

	if (hijo == 0){
		// Realiza la acción correspondiente al proceso
		switch (i){
			// a) información de usuario
			case 1:		
				if (p == NULL){
					printf("Usuario no encontrado en archivo PASSWD\n");
				} else {
					printf("Datos del usuario -> %s:%s:%d:%d:%s:%s:%s\n", 
						p->pw_name, p->pw_passwd, p->pw_uid, 
						p->pw_gid, p->pw_gecos, 
						p->pw_dir, p->pw_shell);
				}
				break;

			// b) Enviar correo al usuario
			case 2:
				// Si el usuario no existe
				if (p == NULL){
					sprintf(correo, "echo \"Recordar a '%s' hacer su tarea para el parcial...\" | mail -s \"Tarea del parcial\" root", argv[1]);
					system(correo);
					printf("Correo enviado a root\n");
				} else {
					sprintf(correo, "echo \"Tienes tarea para el dia del parcial...\" | mail -s \"Tarea del parcial\" %s", argv[1]);
					system(correo);
					printf("Correo enviado a %s\n", argv[1]);
				}
				break;
				
			// c) Crear directorio en directorio home de usuario
			case 3:
				if (p == NULL){
					printf("No existe el usuario '%s', error al crear directorio en home...\n", argv[1]);
				} else {
					sprintf(directorio, "/home/%s/%s", argv[1], argv[2]);
					status = mkdir(directorio, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
					
					if (status == 0){
						printf("Directorio '%s' creado con exito\n", argv[2]);
					} else {
						printf("Error al crear directorio, puede que '%s' ya existe en home\n", argv[2]);
					}
				}
				break;
		}
	} else {
		// Espera terminación de hijos
		hijo = wait(&estado);
		do {
			if (hijo > 0){
				fprintf(stdout,"Termino mi hijo PID: %ld\n", (long)hijo);
			}
			hijo = wait(&estado);
		} while(hijo != -1);
	}

	return EXIT_SUCCESS;
}

// Busca los datos del usuario
struct passwd *busca_datos_usr(char *usuario){
	struct passwd *p;

	while (p = getpwent()){
		if (strcmp(p->pw_name, usuario) == 0){
			break;
		}
	}

	return p;
}