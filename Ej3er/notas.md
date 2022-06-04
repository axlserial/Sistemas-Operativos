#Códigos de señales
2	- Interrumpir
9	- Terminar abruptamente
19	- Detener (stop)
18	- Continuar (continue)


#Funciones
int dup2(int oldfd, int newfd); - Cambia a donde apunta el descriptor de archivo 'oldfd' a lo que apunta 'newfd'
int execlp(const char *file, const char *arg, ... /* (char  *) NULL */); - Ejecuta el comando dado