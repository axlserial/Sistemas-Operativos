#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

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
	vector<string> cadenas, sin_espacios;

	// Lee entrada de usuario
	cout << "$> ";
	getline(cin, buffer);

	// Obtener encauses
	cadenas = obtiene_str(buffer, "|");

	// Sí se encontró 1 o más encauses
	if (cadenas.size() > 1) {
		for (string s : cadenas) {
			
		}
	} else {
		// Obtener comando/programa y sus parámetros
		sin_espacios = obtiene_str(cadenas.at(0));

		// Sí tiene al menos 1 parámetro
		if (sin_espacios.size() > 1) {
			for (string s : sin_espacios) {
				
			}
		} else {

		}
	}
}