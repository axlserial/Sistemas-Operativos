/*
 *	Documentación del programa 'determinantes.c'
 *
 *	Descripción:
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
	int resultado;
	//int **cofactor;
	int FilaMin;
	int tamMatriz;
	int **matriz;
} datosMatriz;

//unsigned long long factorial(long long n);
int **creaMatriz(int tam);
int determinante2X2(int **matriz);
int **cofactor(int **matriz,int tam, int p, int q);
//int determinanteMC(int **matriz, int tam);
void *calculaDet(void *valor);

int main(int argc, char const *argv[]){
	int tam_matriz, resultado=0;

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

		//hilos[i].resultado *= hilos[i].FilaMin; 

		pthread_create(&tid[i],&attr,calculaDet,&hilos[i]);
	}

	// espera terminación de hilos
	for (int i = 0; i < tam_matriz; i++){
		pthread_join(tid[i], NULL);
	}

	for (int i = 0; i < tam_matriz; i++){
		resultado += hilos[i].resultado;
	}

	printf("El determinate de la matriz es = %d\n", resultado);
	return 0;
}

// /*Factorial de un número*/
// unsigned long long factorial(long long n){
// 	if (n > 1000)
// 		return -1;

// 	long long contador;
// 	long double suma = 0;

// 	// Caso base

// 	if (n == 0)
// 		return 1;

// 	// Calcula la suma de los logaritmos

// 	for (contador = 1; contador <= n; contador++){
// 		suma = suma + log(contador);
// 	}

// 	long double resultado = round(exp(suma)) / 2;

// 	return (unsigned long long int)resultado;
// }

/*Crea Matriz*/
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

/*Calcula determinante matriz 2x2*/
int determinante2X2(int **matriz){
	int resultado;
	resultado = matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0];
	return resultado;
}

int **cofactor(int **matriz,int tam, int p, int q){
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
		
		
	}

	datos->resultado *= datos->FilaMin;

	pthread_exit(0);
}

// int determinanteMC(int **matriz, int tam){
// 	int **auxMat;
// 	int determinante=0;
// 	int fila[tam];


// 	auxMat = creaMatriz(tam-1);
// 	if(tam == 2)
// 		determinante = determinante2X2(matriz);
		
// 	else{
// 		// Calcula el signo
// 		for(int i = 0; i<tam ; i++){
// 			if(i%2 != 0){
// 				fila[i] = -matriz[0][i];
// 			}
// 			else{
// 				fila[i] = matriz[0][i];
// 			}
// 		}
// 		// hilos agregar
// 		for(int j = 0; j<tam; j++){
// 			// Obtenemos el cofactor de la matriz
// 			auxMat = cofactor(matriz,tam,0,j);
// 			// Calculo del determinante
// 			determinante += fila[j] * determinanteMC(auxMat,tam-1);
// 		}
// 	}

// 	return determinante;
// }