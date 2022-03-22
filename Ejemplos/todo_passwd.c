/* 
	Recuperar todos los usuarios dados
	de alta en el archivo passwd por
	medio de la estructura passwd.
	El hijo pasará cada linea a linea
	los datos al padre.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

int main(){
	FILE *file;
	file = fopen("datos.txt", "w+");

	if (file == NULL){
		printf("Error al crear el archivo\n");
		exit(1);
	}

	pid_t hijo;
	int x = 1, estado;
	char* linea;

	struct passwd *p;

	hijo = fork();

	if (hijo == 0){
		while (p = getpwent()){
			if (strcmp(p->pw_name, "axl") == 0){
					fprintf(file, "%s:%s:%d:%d:%s:%s:%s\n", 
						p->pw_name, p->pw_passwd, p->pw_uid, 
						p->pw_gid, p->pw_gecos, 
						p->pw_dir, p->pw_shell);	
			}
		}
		fclose(file);
		exit(0);
	} else {
		if (wait(&estado) == hijo){
			file = fopen("datos.txt", "r");

			printf("Mi hijo escribio:\n");
			while (!feof(file)){
				linea = (char *) malloc(sizeof(char) * 240);
				fgets(linea, 240, file);
				fprintf(stdout, "%s", linea);
			}
			fclose(file);
			exit(0);
		}
	}
}