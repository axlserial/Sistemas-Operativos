#include <stdio.h>
#include <semaphore.h>
#include <signal.h>

int cont = 3;
sem_t sem;

void control_c(int signal) {
	cont--;

	if (cont == 0){
		sem_post(&sem);
	}
}

int main() {
	sem_init(&sem, 0, 0);
	signal(SIGINT, control_c);

	sem_wait(&sem);

	printf("\nEjecución terminada\n");

	return 0;
}