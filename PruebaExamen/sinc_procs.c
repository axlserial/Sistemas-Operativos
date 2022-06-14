#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem1, sem2;

void *msg1(void *dato) {
	printf("Linux ");
	sem_post(&sem1);
	pthread_exit(NULL);
}

void *msg2(void *dato) {
	sem_wait(&sem1);
	printf("es un sistema operativo ");
	sem_post(&sem2);
	pthread_exit(NULL);
}

void *msg3(void *dato) {
	sem_wait(&sem2);
	printf("hermoso\n");
	pthread_exit(NULL);
}

int main() {
	pthread_t t1, t2, t3;

	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);

	pthread_create(&t1, NULL, msg1, NULL);
	pthread_create(&t2, NULL, msg2, NULL);
	pthread_create(&t3, NULL, msg3, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	sem_destroy(&sem1);
	sem_destroy(&sem2);

	return 0;
}