#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1

using namespace std;

void menu();

vector<string> obtiene_str(string s, string del = " ") {
	vector<string> sub_cadenas;
	string sub_str;

	int inicio = 0;
	int final = s.find(del);

	while (final != -1) {
		sub_str = s.substr(inicio, final - inicio);
		if(sub_str.find_first_not_of(' ') != string::npos) {
			sub_cadenas.push_back(sub_str);
		}

		inicio = final + del.size();
		final = s.find(del, inicio);
	}

	sub_str = s.substr(inicio, final - inicio);
	if(sub_str.find_first_not_of(' ') != string::npos) {
		sub_cadenas.push_back(sub_str);
	}

	return sub_cadenas;
}

vector<string> obtiene_args(string s) {
	vector<string> args;
	args = obtiene_str(s);

	return args;
}

void proceso(string comando, vector<string> args, int *fd, 
			 int fdEntrada = STDIN, int fdSalida = STDOUT,
			 bool encause = false) {
	
	int estado;
	string arg;

	if (encause) {
		if (fdEntrada != STDIN) {
			close(fd[1]);
		}
	}

	pid_t pid = fork();

	if (pid == 0) {

		// Sí se recibe una nueva entrada
		if (fdEntrada != STDIN) {
			dup2(fdEntrada, STDIN);
			close(fdEntrada);
		}

		// Sí se recibe una nueva salida
		if (fdSalida != STDOUT) {
			dup2(fdSalida, STDOUT);
			close(fdSalida);
		}

		if (args.size() > 0) {
			if (execlp(comando.c_str(), comando.c_str(), args.at(0).c_str(), NULL) == -1) {
				cout << "Comando '" << comando << "' no encontrado" << endl;
			}
		} else {
			if (execlp(comando.c_str(), comando.c_str(), NULL) == -1) {
				cout << "Comando '" << comando << "' no encontrado" << endl;
			}
		}
	} else {
		if (wait(&estado) == pid);
	}

	// return pid;
}

void procesa_string(string cadena, int *fd, int infd = -1, int outfd = -1) {
	int fdEntrada, fdSalida;
	vector<string> cadenas, args;
	string comando, entrada, salida, proc;

	if (infd != -1) {
		fdEntrada = infd;
	}

	if (outfd != -1) {
		fdSalida = outfd;
	}

	// Obtener redireccionamientos de entrada
	cadenas = obtiene_str(cadena, "<");
		
	// Sí hay redireccionamiento de entrada
	if (cadenas.size() > 1) {

		// Se obtiene el comando y argumentos
		args = obtiene_args(cadenas.at(0));
		comando = args.at(0);

		// Se borra comando de vector de argumentos
		args.erase(args.begin());

		// Se borra del resto de la cadena
		cadenas.erase(cadenas.begin());

		// Obtener redireccionamientos de salida
		cadenas = obtiene_str(cadenas.at(0), ">");

		// Sí hay redireccionamiento de salida
		if (cadenas.size() > 1) {

			// Obtenemos archivo de entrada
			entrada = obtiene_str(cadenas.at(0)).at(0);
			cadenas.erase(cadenas.begin());

			// Obtenemos archivo de salida
			salida = obtiene_str(cadenas.at(0)).at(0);

			// Comando y ambos redireccionamientos
			fdSalida = open(salida.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);
			fdEntrada = open(entrada.c_str(), O_RDONLY, 0600);
			if (fdEntrada == -1) {
				cout << "No se encontró el archivo '" << entrada << "'" << endl;
			} else {
				proceso(comando, args, fd, fdEntrada, fdSalida);
			}
		} else {
			// Obtenemos archivo de entrada
			entrada = obtiene_str(cadenas.at(0)).at(0);

			// Comando y redireccionamiento de entrada
			fdEntrada = open(entrada.c_str(), O_RDONLY, 0600);

			if (fdEntrada == -1) {
				cout << "No se encontró el archivo '" << entrada << "'" << endl;
			} else {
				if (outfd != -1) {
					proceso(comando, args, fd, fdEntrada, fdSalida, true);
				} else {
					proceso(comando, args, fd, fdEntrada);
				}
			}
		}
	} else {
		// Obtener redireccionamientos de salida
		cadenas = obtiene_str(cadenas.at(0), ">");

		// Sí hay redireccionamiento de salida
		if (cadenas.size() > 1) {

			// Se obtiene el comando y argumentos
			args = obtiene_args(cadenas.at(0));
			comando = args.at(0);
	
			// Se borra comando de vector de argumentos
			args.erase(args.begin());
	
			// Se borra del resto de la cadena
			cadenas.erase(cadenas.begin());

			// Obtenemos archivo de salida
			salida = obtiene_str(cadenas.at(0)).at(0);

			fdSalida = open(salida.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);

			// Comando y redireccionamiento de salida
			if (infd != -1) {
				proceso(comando, args, fd, fdEntrada, fdSalida, true);
			} else {
				if (outfd != -1) {
					proceso(comando, args, fd, STDIN, fdSalida, true);
				} else {
					proceso(comando, args, fd, STDIN, fdSalida);
				}
			}

		} else {
			// Se obtiene el comando y argumentos
			args = obtiene_args(cadenas.at(0));
			comando = args.at(0);
	
			// Se borra comando de vector de argumentos
			args.erase(args.begin());

			// Solo comando y argumentos (si los hay)
			if (infd != -1 && outfd != -1) {
				proceso(comando, args, fd, fdEntrada, fdSalida, true);
			} else if (infd != -1) {
				proceso(comando, args, fd, fdEntrada, STDOUT, true);
			} else if (outfd != -1) {
				proceso(comando, args, fd, STDIN, fdSalida, true);
			} else {
				proceso(comando, args, fd);
			}
		}
	}
}

string buffer;

int main() {
	while (1){
		menu();
		if (buffer.compare("exit") == 0) {
			break;
		}
	}

	return 0;
}

void menu() {
	int tube[2], fdAnterior, tubeAnterior[2];
	vector<string> cadenas, args;
	string comando, entrada, salida, proc;

	// Lee entrada de usuario
	cout << "$> ";
	getline(cin, buffer);

	// Verifica que no sea palabra de salida
	if (buffer.compare("exit") == 0) {
		return;
	}

	// Obtener encauses
	cadenas = obtiene_str(buffer, "|");

	// Sí se encontró 1 o más encauses
	if (cadenas.size() > 1) {

		if (pipe(tube) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		procesa_string(cadenas.at(0), tube, -1, tube[1]);
		cadenas.erase(cadenas.begin());

		for (int i = 0; i < (int)cadenas.size(); i++){
			proc = cadenas.at(i);
			
			// Proceso intermedio
			if (i + 1 < (int)cadenas.size()) {
				tubeAnterior[0] = tube[0];
				tubeAnterior[1] = tube[1];
				fdAnterior = tube[0];

				if (pipe(tube) == -1) {
					perror("pipe");
					exit(EXIT_FAILURE);
				}

				procesa_string(proc, tubeAnterior, fdAnterior, tube[1]);
			} else {
				procesa_string(proc, tube, tube[0]);
			}
		}
	} else {
		procesa_string(cadenas.at(0), tube);
	}
}