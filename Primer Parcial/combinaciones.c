#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned long long cnr(int n, int k);

int main(int argc, char const *argv[]){
	int n, k;
	unsigned long long comb = 0, suma = 0;
	int cant_hijos, estado;
	char *linea;

	FILE *file;

	file = fopen("Combinaciones.txt", "w");
	fclose(file);
	file = fopen("Combinaciones.txt", "a+");

	if (file == NULL){
		printf("Error al crear el archivo\n");
		exit(1);
	}

	// verifica el argumento
	if (argc == 1 || argc > 2){
		printf("Debes colocar como unico argumento el numero de n combinaciones\n");
		exit(1);
	}

	// establece el numero de hijos a crear
	cant_hijos = atoi(argv[1]) + 1;
	n = atoi(argv[1]);
	pid_t hijo;

	int i;
	for (i = 1; i <= cant_hijos; i++){
		hijo = fork();
		k = cant_hijos - i;
		if (hijo == 0){
			break;
		}
	}

	if (hijo == 0){
		comb = cnr(n, k);
		fprintf(file, "%llu\n", comb);
		fflush (file);
		fclose(file);
		exit(0);
	} else {
		do {
			hijo = wait(&estado);
		} while (hijo != -1);

		fclose(file);
		file = fopen("Combinaciones.txt", "r");

		while (!feof(file)){
			linea = (char *)malloc(sizeof(char) * 240);
			fgets(linea, 240, file);
			suma += strtoull(linea, NULL, 10);
			free(linea);
		}

		fclose(file);
			
	}

	fprintf(stdout, "suma = %llu \n", suma);

	return EXIT_SUCCESS;
}

unsigned long long cnr(int n, int k){
	unsigned long long aux = 1;

	// selecciona el valor más chico
	if ((n - k) < k){
		k = n - k;
	}

	for(int j = 1; j <= k; j++, n--) {

		if (n % j == 0) {
			aux *= n / j; 
		} else if (aux % j == 0) {
			aux = (aux / j) * n;
		} else {
			aux = (aux * n) / j;
		}
	}

	return aux;
}