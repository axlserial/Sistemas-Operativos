/*
 *	Documentación del programa 'determinantes.c'
 *
 *	Descripción:		Programa que se encarga de calcular la determinante de una matriz  de tamaño n por medio del método de 
 *						menores y cofactores, donde cada menor y cofactor será calculado por medio de procesos ligeros que
 *						regresarán su resultado individual a su proceso padre, el cuál se encargará de unir los resultados
 *						para obtener el valor final del determinante. El tamaño de la matríz y el nombre del archivo que 
 *						contiene a la matriz se pasan como argumentos al ejecutar el programa.
 *
 *	Modo de compilación: gcc -Wall determinantes.c -lpthread -o determinantes
 *
 *	Modo de ejecución:	./determinates < n > <archivo.txt>
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
	double resultado;
	int FilaMin;
	int tamMatriz;
	int **matriz;
} datosMatriz;

int **creaMatriz(int tam);
double determinante2X2(int **matriz);
int **cofactor(int **matriz,int tam, int p, int q);
void *calculaDet(void *valor);

int main(int argc, char const *argv[]){
	int tam_matriz; 
	double resultado = 0;

	if (argc == 1 || argc > 3){
		printf("Debes colocar como argumento el tam de la matriz cuadrada < n > seguido del nombre del archivo <archivo.txt>\n");
		exit(1);
	}

	tam_matriz = atoi(argv[1]);

	int **matriz, num;
	matriz = creaMatriz(tam_matriz);
	FILE *fptr;

	fptr = fopen(argv[2], "r");

	for (int i = 0; i < tam_matriz; i++){
		for (int j = 0; j < tam_matriz; j++){
			fscanf(fptr, "%d", &num);
			matriz[i][j] = num;
		}
	}

	fclose(fptr);

	pthread_t tid[tam_matriz];
	pthread_attr_t attr;
	datosMatriz hilos[tam_matriz];

	for(int i = 0; i < tam_matriz; i++){
		pthread_attr_init(&attr);

		// Establecemos los cofactores
		hilos[i].matriz = cofactor(matriz,tam_matriz,0,i);
		hilos[i].tamMatriz = tam_matriz-1;
		if(i%2 != 0)
			hilos[i].FilaMin = -matriz[0][i];
		else
			hilos[i].FilaMin = matriz[0][i];

		pthread_create(&tid[i],&attr,calculaDet,&hilos[i]);
	}

	// espera terminación de hilos
	for (int i = 0; i < tam_matriz; i++){
		pthread_join(tid[i], NULL);
	}

	// Suma resultados de hilos
	for (int i = 0; i < tam_matriz; i++){
		resultado += hilos[i].resultado;
		free(hilos[i].matriz);
	}

	printf("El determinate de la matriz es = %f\n", resultado);
	return 0;
}

/*
*	Función: 		int **creaMatriz(int tam)
*	Descripción:	Crea y devuelve una matriz de 0's cuadrada
*	Parametro de entrada:	int tam:		Tamaño de la matriz a crear
*	Retorno:				int ** matriz:	Matriz creada
*/
int **creaMatriz(int tam){
	int **matriz;
	matriz = malloc(tam * sizeof(int*));

	for (int i = 0; i < tam; i++){
		matriz[i] = malloc(tam * sizeof(int));
	}

	for (int i = 0; i < tam; i++){
		for (int j = 0; j < tam; j++){
			matriz[i][j] = 0;
		}
	}

	return matriz;
}

/*
*	Función: 		double determinante2X2(int **matriz)
*	Descripción:	Calcula el determinante de una matriz de 2x2
*	Parametro de entrada:	int ** matriz:		Matriz de 2x2 a calcular su determinante
*	Retorno:				double resultado:	Determinante de la matriz dada
*/
double determinante2X2(int **matriz){
	double resultado;
	resultado = (double)matriz[0][0] * (double)matriz[1][1] - (double)matriz[0][1] * (double)matriz[1][0];
	return resultado;
}

/*
*	Función: 		int **cofactor(int **matriz,int tam, int p, int q)
*	Descripción:	
*	Parametro de entrada:	int tam:			
*							int p:				
*							int q:				
*	Retorno:				int **matrizAux:	matriz de cofactor
*/
int **cofactor(int **matriz, int tam, int p, int q){
	int a,b;
	int **matrizAux;

	matrizAux = creaMatriz(tam-1);

	a=b=0;

	for(int i = 0; i<tam; i++){
		for(int j = 0; j<tam; j++){
			if(i!=p && j!=q ){
				matrizAux[a][b++] = matriz[i][j];
				if(b == (tam -1)){
					b = 0;
					a++;
				}
			}
		}	
	}

	return matrizAux;
}

/*
*	Función: 		void *calculaDet(void *valor)
*	Descripción:	
*	Parametro de entrada:	void *valor:	Estructura de los datos de la matriz a trabajar
*	Retorno:				---
*/
void *calculaDet(void *valor){
	datosMatriz *datos;
	datos = (datosMatriz *)valor;

	pthread_t tid[datos->tamMatriz];
	pthread_attr_t attr;
	datosMatriz hilos[datos->tamMatriz];

	if(datos->tamMatriz == 2){
		datos->resultado = determinante2X2(datos->matriz);
	}
	else{

		datos->resultado = 0;

		for(int i = 0; i < datos->tamMatriz; i++){
			pthread_attr_init(&attr);

			// Establecemos los cofactores
			hilos[i].matriz = cofactor(datos->matriz,datos->tamMatriz,0,i);
			hilos[i].tamMatriz = datos->tamMatriz-1;

			if(i%2 != 0)
				hilos[i].FilaMin = -datos->matriz[0][i];
			else
				hilos[i].FilaMin = datos->matriz[0][i];

			
			pthread_create(&tid[i],&attr,calculaDet,&hilos[i]);
		}


		// espera terminación de hilos
		for (int i = 0; i < datos->tamMatriz; i++){
			pthread_join(tid[i], NULL);
		}

		// Realiza suma
		for (int i = 0; i < datos->tamMatriz; i++){
			datos->resultado += hilos[i].resultado;
			free(hilos[i].matriz);
		}
		
	}

	datos->resultado *= (double)datos->FilaMin;

	pthread_exit(0);
}