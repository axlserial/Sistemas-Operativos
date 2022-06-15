#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void funcion(int a, int b) {
	int c = 10;
	pid_t pid = fork();

	if (pid == 0) {
		printf("a: %d, b: %d, c: = %d\n", a, b, c);
	}
}

int main() {
	funcion(1, 2);
}