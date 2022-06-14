#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LONG 255

char *buffer;

void menu();

int main() {

	buffer = malloc(MAX_LONG * sizeof(char));

	while (1){
		menu();
		if (strstr(buffer, "exit") != NULL) {
			break;
		}		
	}

	return 0;
}

void menu() {
	int cant_pipes = 0, len;
	char *p;

	printf("$> ");
	fgets(buffer, MAX_LONG, stdin);

	// reemplaza fin de linea leido por el fin de cadena
	p = strchr(buffer, '\n');
	if (p != NULL){
		*p = '\0';
	}

	len = (int)strlen(buffer);

	for (int i = 0; i < len; i++) {
		if (buffer[i] == '|'){
			cant_pipes++;
		}
	}

	if (cant_pipes > 0) {
		if (cant_pipes == 1) {

		} else {
			for (int i = 0; i < cant_pipes - 1; i++){
				
			}
		}
	}

	// char *c = strsep(&buffer, "|");
	// printf("%s, %s, %d\n", c, buffer, (int)strlen(c));
	// c = strsep(&buffer, "|");
	// printf("%s, %s, %d\n", c, buffer, (int)strlen(c));
}