#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/msg.h>
#include <pthread.h>

void *funcion1(void *valor);
void *funcion2(void *valor);
void *funcion3(void *valor);
void *funcion4(void *valor);
void send_msg(int, int, char*);
void get_msg(int, int, char*);

#define CANT_PROCESOS 4

struct msgbuf {
	long mtype;
	char mtext[80];
};

typedef struct {
	int qid;
	int type;
} msginfo; 

int p1_listo = 0;
int p2_listo = 0;

int main(int argc, char *argv[]){
	int qid;
	int msgtype1 = 1, msgtype2 = 2;
	int llave;

	pthread_attr_t attr;
	pthread_t tid[CANT_PROCESOS];
	msginfo hilos[CANT_PROCESOS];

	llave = ftok(argv[0], 'a');

	// Crea memoria compartida
	if ((qid = msgget(llave, IPC_CREAT | 0666)) == -1){
		perror("msgget");
		exit(EXIT_FAILURE);
	}

    //Crear los hilos
	hilos[0].qid = qid;
	hilos[0].type = msgtype1;
	pthread_attr_init(&attr); 
	pthread_create(&tid[0], &attr, funcion1, &hilos[0]);

	hilos[1].qid = qid;
	hilos[1].type = msgtype1;
	pthread_attr_init(&attr); 
	pthread_create(&tid[1], &attr, funcion2, &hilos[1]);

	hilos[2].qid = qid;
	hilos[2].type = msgtype2;
	pthread_attr_init(&attr); 
	pthread_create(&tid[2], &attr, funcion3, &hilos[2]);

	hilos[3].qid = qid;
	hilos[3].type = msgtype2;
	pthread_attr_init(&attr); 
	pthread_create(&tid[3], &attr, funcion4, &hilos[3]);

    //Cerrar los hilos
    for (int i = 0; i < CANT_PROCESOS; i++) {
        pthread_join (tid[i], NULL);
    }
	
	return (EXIT_SUCCESS);
}

void *funcion1(void *valor){
	int i = 0;
	while (i < 10){
			msginfo *dato;
			dato = (msginfo *)valor;
			send_msg(dato->qid, dato->type, "p1");
	}

	pthread_exit(0);
}

void *funcion2(void *valor){
	int i = 0;
	while (i < 10){
		msginfo *dato;
		dato = (msginfo *)valor;
		get_msg(dato->qid, dato->type, "p1");
	}

	pthread_exit(0);
}

void *funcion3(void *valor){
	int i = 0;
	while (i < 10){	
		msginfo *dato;
		dato = (msginfo *)valor;
		send_msg(dato->qid, dato->type, "p2");
	}

	pthread_exit(0);
}

void *funcion4(void *valor){
	int i = 0;
	while (i < 10){		
		msginfo *dato;
		dato = (msginfo *)valor;
		get_msg(dato->qid, dato->type, "p2");
	}

	pthread_exit(0);
}

void send_msg(int qid, int msgtype, char* nom){
	struct msgbuf msg;
	time_t t;

	msg.mtype = msgtype;
	time(&t);
	snprintf(msg.mtext, sizeof(msg.mtext), "%s: El mensaje salió el: %s", nom, ctime(&t));

	// Enviar/Escribir en la cola
	if (msgsnd(qid, (void *)&msg, sizeof(msg.mtext), IPC_NOWAIT) == -1){
		perror("ERROR en msgsnd");
		exit(EXIT_FAILURE);
	}
}

void get_msg(int qid, int msgtype, char* nom){
	struct msgbuf msg;
	// Recibir/Leer de la cola
	if (msgrcv(qid, (void *)&msg, sizeof(msg.mtext), msgtype, MSG_NOERROR | IPC_NOWAIT) == -1){
		if (errno != ENOMSG){
			perror("ERROR en msgrcv");
			exit(EXIT_FAILURE);
		}
		printf("No hay mensajes disponibles para leer/recibir con msgrcv()\n");
	}
	else
		printf("%s: Mensaje recibido: %s\n", nom, msg.mtext);
}