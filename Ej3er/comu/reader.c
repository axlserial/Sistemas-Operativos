#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define TUBE1 "/tmp/tubeA"
#define TUBE2 "/tmp/tubeB"

int abrirTuberiaEntrada();
int abrirTuberiaSalida();

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

int main()
{
    pthread_t reader, writer;
    pthread_create( &reader, NULL, lector, NULL);
    pthread_create( &writer, NULL, escritor, NULL);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    return 0;
}

int abrirTuberiaEntrada(){
    int fd = open(TUBE1, O_RDONLY);
         fprintf(stderr, "el archivo no abre\n");
   return fd;
}

int abrirTuberiaSalida(){
    int fd = open(TUBE2, O_WRONLY);
         fprintf(stderr, "el archivo no abre\n");
   return fd;
}
