# include <stdio.h>
# include <stdlib.h>

int main(int argc, char const *argv[], char *env[]) {
    printf("Argumentos de entrada %d\n",argc);
    
    if (argc > 1) {
        for (int i = 0; i < argc ; i++) {
            printf("Argumento[%d] = %s\n",i,argv[i]);
        }
        for (int i = 0; env[i] != NULL; i++) {
            printf("%s\n",env[i]);
        }
    }

    else {
        printf("Debe colocar argumentos\n");
    }

    return EXIT_SUCCESS;
    
}