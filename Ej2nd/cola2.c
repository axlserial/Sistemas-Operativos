/*
Programa que crea dos procesos que envian/reciben
mensajes de tipo 1 y 2
*/

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
void send_msg(int, int);
void get_msg(int, int);

struct msgbuf {
	long mtype;
	char mtext[80];
};

int main(int argc, char *argv[]){
	int qid;
	int modo; // 1 = enviar/escribir, 2 = recibir/leer
	int msgtype = 1;
	int llave;

	llave = ftok(argv[0], 'a');
	
	if (argc > 1){
		if (strcmp(argv[1], "s") == 0)
			modo = 1;
		else if (strcmp(argv[1], "r") == 0)
			modo = 2;
		else {
			printf("Use: ./mcola s|r\n");
			exit(EXIT_FAILURE);
		}
	} else {
		printf("Use: ./mcola s|r\n");
		exit(EXIT_FAILURE);
	}

	// Crea memoria compartida
	if ((qid = msgget(llave, IPC_CREAT | 0666)) == -1){
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	if (modo == 2)
		get_msg(qid, msgtype);
	else
		send_msg(qid, msgtype);
	
	return (EXIT_SUCCESS);
}

void send_msg(int qid, int msgtype){
	struct msgbuf msg;
	time_t t;

	msg.mtype = msgtype;
	time(&t);
	snprintf(msg.mtext, sizeof(msg.mtext), "El mensaje salió el: %s", ctime(&t));

	// Enviar/Escribir en la cola
	if (msgsnd(qid, (void *)&msg, sizeof(msg.mtext), IPC_NOWAIT) == -1){
		perror("ERROR en msgsnd");
		exit(EXIT_FAILURE);
	}

	printf("Mensaje enviado: %s\n", msg.mtext);
}

void get_msg(int qid, int msgtype){
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
		printf("Mensaje recibido: %s\n", msg.mtext);
}