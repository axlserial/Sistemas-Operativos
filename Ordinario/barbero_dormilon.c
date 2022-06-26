#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

// Funciones para hilos
void *barbero(void *dato);
void *cliente(void *dato);

// Semaforos
sem_t barbero_sem; 
sem_t cliente_sem;
sem_t sillas_sem;

// Variables globales
int sillas;
int sillas_disponibles;
int clientes_totales;
int no_atendidos;

int main() {	
	// Inicializa para generar tiempos aleatorios
	srand(time(NULL));   

	// Hilo del barbero
	pthread_t barbero_hilo;

	// Inicializa semaforos
	sem_init(&barbero_sem, 0, 0);
	sem_init(&cliente_sem, 0, 0);
	sem_init(&sillas_sem, 0, 1);
	
	// Obtiene sillas y clientes
	printf("Cantidad de n sillas: ");
	scanf("%d", &sillas);
	sillas_disponibles = sillas;
	
	printf("Cantidad total de clientes: ");
	scanf("%d", &clientes_totales);
	printf("------\n");
	
	// Crea el hilo del barbero
	pthread_create(&barbero_hilo, NULL, barbero, NULL);

	// Crea hilos de clientes
	for (int i = 0; i < clientes_totales; i++) {
		pthread_t cliente_hilo;
		pthread_create(&cliente_hilo, NULL, cliente, NULL);

		// Espera un tiempo aleatorio entre 0 y 100 milisegundos
		// antes de crear un nuevo hilo cliente
		usleep((rand() % 101) * 1000);
	}
	 
	// Espera que el barbero termine de atender
	pthread_join(barbero_hilo, NULL);

	// Elimina semaforos
	sem_destroy(&barbero_sem);
	sem_destroy(&cliente_sem);
	sem_destroy(&sillas_sem);

	return EXIT_SUCCESS;
}

void *barbero(void *dato) {
	int contador = 0;
	
	while (1) {
		// Espera a que un cliente esté listo, duerme
		sem_wait(&cliente_sem);

		// Trata de acceder a la cantidad de sillas
		sem_wait(&sillas_sem);

		// Como va a atender a un cliente, aumenta
		// cantidad de sillas disponibles para espera
		sillas_disponibles++;

		// Libera semaforo de sillas
		sem_post(&sillas_sem);

		// Indica que el barbero está listo para atender
		sem_post(&barbero_sem);        

		// Atiende al cliente una cantidad de tiempo
		// aleatoria
		usleep((rand() % 1001) * 1000);
		
		printf("Cliente atendido.\n\n");
		
		// Sí ya atendió a todos los clientes
		contador++; 
		if (contador == (clientes_totales - no_atendidos)) {
			printf("Sin más clientes que atender.\n");
			break;
		}
	}

	pthread_exit(NULL);    
}

void *cliente(void *dato) {
	// Espera a que pueda acceder a las sillas
	sem_wait(&sillas_sem); 

	// Sí hay sillas disponibles
	if (sillas_disponibles >= 1) {
		// Ocupa un asiento
		sillas_disponibles--;

		printf("El cliente (tid: %lu) está esperando.\n", pthread_self());
		printf("Sillas disponibles después que entró el cliente: %d\n\n", sillas_disponibles);

		// Indica que el cliente está listo para ser atendido
		sem_post(&cliente_sem);

		// Libera sillas
		sem_post(&sillas_sem);         

		// Espera a ser atendido por el barbero
		sem_wait(&barbero_sem); 

		// Cuando llegue aquí, indica que el barbero lo está atendiendo
		printf("El cliente (tid: %lu) está siendo atendido.\n\n", pthread_self());        
	} else {

		// Libera el semaforo de sillas
		sem_post(&sillas_sem);

		// Aumenta contador de clientes no atendidos
		no_atendidos++;

		// Mensaje de despedida.
		printf("Cliente (tid: %lu) no encontró silla, se fué.\n\n", pthread_self());
	}
		
	pthread_exit(NULL);
}