#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int salida = open("datosSalida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0600);

	// dup2(salida, 1);
	// close(salida);
	execlp("ls", "ls", "-la", NULL);

	return 0;
}