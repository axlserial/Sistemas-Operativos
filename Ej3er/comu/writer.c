#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define TUBE1 "/tmp/tubeA"
#define TUBE2 "/tmp/tubeB"

void crearTuberia();
int abrirTuberiaSalida();
int abrirTuberiaEntrada();

void *escritor(void *s){
    char str2[80];
    int fd1 = abrirTuberiaSalida();
    while (!feof(stdin))
    {   
        printf("introduce una cadena: ")  ;     
        fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2)+1);   
    }
	close(fd1);
    pthread_exit(NULL);
}

void *lector(void *s){ // toma datos de tub1
    int fd;
    fd = abrirTuberiaEntrada();
	char arr1[80];
    while ( read(fd, arr1, sizeof(arr1)) > 0){
        printf("User2: %s", arr1);
    }
       
	close(fd);
    pthread_exit(NULL);
}

int main()
{
    pthread_t writer, reader;
    crearTuberia();
    pthread_create(&writer, NULL, escritor, NULL);
    pthread_create(&reader, NULL, lector, NULL);
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    //  unlink(tube1);
   //  unlink(tube2);
    return 0;
}


void crearTuberia(){

    if(mkfifo( TUBE1, 0666)>0)
        fprintf(stderr, "primera tuberia no fue creada\n");
    if(mkfifo( TUBE2, 0666)>0)
        fprintf(stderr, "segunda tuberia no fue creada\n");
}

int abrirTuberiaSalida(){
    int fd = open(TUBE1, O_WRONLY);
         fprintf(stderr, "el archivo no abre\n");
   return fd;
}

int abrirTuberiaEntrada(){
    int fd = open(TUBE2, O_RDONLY );
         fprintf(stderr, "el archivo no abre\n");
   return fd;
}