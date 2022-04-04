/*
*	Documentación del programa 'combinaciones.c'
*
*	Descripción:	Programa que recibe desde linea de comandos el valor n correspondiente al numero 
					de combinaciones de un conjunto de n en k. Cada nCk es calculado por un proceso hijo. 
					El proceso padre, despues de crear a los n+1 hijos, calculara la suma:
								Combinacones_Totales = nCk + nC(k-1) +. .. + nC1 + nC0 
					y posteriormente mostrara el resultado en pantalla. 
*
*	Modo de compilación: gcc combinaciones.c -o combinaciones
*
*	Modo de ejecución:	./combinaciones < n >
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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned long long nCk(int n, int k);

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
		printf("Debes colocar como unico argumento el numero de combinaciones de < n > en k  \n");
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
		comb = nCk(n, k);
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

	fprintf(stdout, "Combinacones_Totales = %llu \n", suma);

	return EXIT_SUCCESS;
}

/*
*	Función: 	unsigned long long nCk(int n, int k)
*	Descripción:	Calcula el conjunto de combinaciones de n en k
*	Parametros de entrada:	
*							int n:	conjunto de n elemtos
*							int k:	subconjunto de k elementos
*	Retorno:	unsigned long long c:	Conjunto de combinaciones de n en k
*/

unsigned long long nCk(int n, int k){
	unsigned long long c = 1;

	// selecciona el valor más chico
	if ((n - k) < k){
		k = n - k;
	}

	for(int j = 1; j <= k; j++, n--) {

		if (n % j == 0) {
			c *= n / j; 
		} else if (c % j == 0) {
			c = (c / j) * n;
		} else {
			c = (c * n) / j;
		}
	}

	return c;
}