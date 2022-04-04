/*
*	Documentación del programa 'hcombinaciones.c'
*
*	Descripción:	Programa que recibe desde linea de comandos el valor n correspondiente al numero 
					de combinaciones de un conjunto de n en k. Cada nCk es calculado por un hilo hijo. 
					El hilo padre, despues de crear a los n+1 hijos, calculara la suma:
								Combinacones_Totales = nCk + nC(k-1) +. .. + nC1 + nC0 
					y posteriormente mostrara el resultado en pantalla. 
*
*	Modo de compilación: gcc -Wall hcombinaciones.c -lpthread -o hcombinaciones
*
*	Modo de ejecución:	./hcombinaciones < n >
*
*	Elaborado por:	
*		Ayala Ruíz Mario Antonio
*		Elorza Velásquez Margarita
*		García González Axel Isaac
*
*	Licencia: CC BY-NC-SA 4.0
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int n;
	int k;
	unsigned long long comb;
} hiloDato;

void *combina(void *valor);
unsigned long long nCk(int n, int k);

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
	printf("Combinacones_Totales = %llu\n", suma);

	return EXIT_SUCCESS;
}

/*
*	Función:	void *combina(void *valor)
*	Descripción:	Función que obtiene la estructura con los datos a trabajar. 
*					Despues con los valores de la estrutura se calcula el número 
*					de combinaciones de n en k al llamar a la función nCk.
*	Parametros de entrada:	
*							void *valor:	Estructura de los datos a trabajar.
*	Retorno:		---	
*/
void *combina(void *valor){
	hiloDato *datos;
	datos = (hiloDato *)valor;
	datos->comb = nCk(datos->n, datos->k);
	pthread_exit(0);
}

/*
*	Función: 	unsigned long long nCk(int n, int k)
*	Descripción:	Función que calcula el conjunto de combinaciones de n en k
*	Parámetros de entrada:	
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