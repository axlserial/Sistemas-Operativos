#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define STDIN    0    
#define STDOUT   1    

int main( int argc, char *argv[] ) {
    int tube[2];
    int status, pid1, pid2;
   
    pipe ( tube );

    if ( (pid1 = fork()) == 0){
        close ( tube[ STDIN ] );   /* cerrar extremo no necesario */
        dup2 ( tube[ STDOUT ], STDOUT_FILENO );
        close ( tube[ STDOUT ] );
        execlp ( "/bin/ls", "ls", "-l", NULL);
    } else {                      
        close ( tube[ STDOUT ] );    // se debe cerrar antes de fork        
        if ( ( pid2 = fork ( )) == 0 ) {
            //close ( tube[ STDOUT ] );     <---- estaba cerrando aqui
            dup2 ( tube[ STDIN ], STDIN_FILENO );
            close ( tube[ STDIN ] );
            execlp ( "/bin/grep", "grep", "pdf", NULL);
        }
    }
    wait ( &status );
    wait ( &status );
    printf("\n");
    exit(1);

}