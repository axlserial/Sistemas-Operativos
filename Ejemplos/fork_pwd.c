#include <stdio.h>
#include <stdlib.h>
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

	struct passwd *lineapwd;

	hijo = fork();

	if (hijo == 0){
		lineapwd = getpwnam("axl");
		fprintf(file, "%s:%s:%d:%d:%s:%s:%s", 
			lineapwd->pw_name, lineapwd->pw_passwd, lineapwd->pw_uid, 
			lineapwd->pw_gid, lineapwd->pw_gecos, 
			lineapwd->pw_dir, lineapwd->pw_shell);
		fclose(file);
		exit(0);
	} else {
		if (wait(&estado) == hijo){
			file = fopen("datos.txt", "r");
			while (!feof(file)){
				linea = (char *) malloc(sizeof(char) * 240);
				fgets(linea, 240, file);
				fprintf(stdout, "Mi hijo escribio: %s\n", linea);
			}
			fclose(file);
			exit(0);
		}
	}
}