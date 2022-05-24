#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	pid_t t = fork();

	printf("Hola mundo\n");

	if (t == 0){
		printf("Proceso hijo:\n");
		printf("pid = %ld\n", (long)getpid());
		printf("pid = %lu\n", (long)getppid());
	} else {
		printf("Proceso padre:\n");
		printf("pid = %ld\n", (long)getpid());
		printf("pid = %lu\n", (long)getppid());		
	}
}