#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int n;
	int k;
	unsigned long long comb;
} hiloDato;

void *combina(void *valor);
unsigned long long cnr(int n, int k);

int main(int argc, char const *argv[]){

	// verificación
	if (argc != 2){
		printf("Debes colocar como unico argumento n (nCk)\n");
		exit(1);
	}

	// establece n
	int n = atoi(argv[1]);
	int cant_hilos = n + 1;

	// para los hilos
	pthread_t tid[cant_hilos];
	pthread_attr_t attr;
	hiloDato hilos[cant_hilos];

	// crea hilos
	for (int i = 0; i < cant_hilos; i++){
		// Coloca atributo predeterminados	
		pthread_attr_init(&attr);
		
		// establece n, k
		hilos[i].n = n;
		hilos[i].k = i;

		// Crear hilo
		pthread_create(&tid[i], &attr, combina, &hilos[i]);
	}	

	// espera terminación de hilos
	for (int i = 0; i < cant_hilos; i++){
		pthread_join(tid[i], NULL);
	}

	// Realiza la suma de las combinaciones
	unsigned long long suma = 0;
	for (int i = 0; i < cant_hilos; i++){
		suma += hilos[i].comb;
	}

	// muestra resultado en pantalla
	printf("suma = %llu\n", suma);

	return EXIT_SUCCESS;
}

void *combina(void *valor){
	hiloDato *datos;
	datos = (hiloDato *)valor;
	datos->comb = cnr(datos->n, datos->k);
	pthread_exit(0);
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