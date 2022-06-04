#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int tube[2], estado;
	pid_t pid1, pid2;

	if (argc != 2) {
		printf("Debes pasar el argumento para GREP\n");
		exit(EXIT_FAILURE);
	}
	
	if (pipe(tube) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	// Proceso 1
	if ((pid1 = fork()) == 0) {
		dup2(tube[1], 1);
		close(tube[1]);
		close(tube[0]);
		
		execlp("ls", "ls", NULL);
	}

	// Proceso 2
	else if ((pid2 = fork()) == 0) {
		dup2(tube[0], 0);
		close(tube[0]);
		close(tube[1]);

		execlp("grep", "grep", argv[1], NULL);
	}

	else {
		waitpid(pid1, &estado, 0);
		waitpid(pid2, &estado, 0);
	}

	return EXIT_SUCCESS;
}