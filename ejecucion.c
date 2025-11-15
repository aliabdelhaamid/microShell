/*-----------------------------------------------------+      
 |                 E J E C U C I Ó N . C               |
 +-----------------------------------------------------+
 |     Asignatura :  SOP-GIIROB                        |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/
#include "defines.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "colors.h"

int ejecutar (int nordenes , int *nargs , char **ordenes , char ***args , int bgnd)
{
    static char *prev = NULL;
   
    for (int i = 0; i < nordenes; i++) {
        if (strcmp(ordenes[i], "cd") == 0) 
        {
            char *aux = getcwd(NULL, 0);

            if (args[i][1] == NULL) chdir(getenv("HOME"));
            else if (strcmp(args[i][1], "-") == 0) chdir(prev);
            else chdir(args[i][1]);

            prev = aux;
        }

        if (strcmp(ordenes[i], "exit") == 0) exit(0);
        
        pid_t ret = fork();
       
        if (ret == 0) { 
            if(redirigir_entrada(i) == ERROR)
            { 
                printf("Error en la redirección de entrada\n\n");
                return ERROR;
            }
            if(redirigir_salida(i) == ERROR)
            { 
                printf("Error en la redirección de salida\n\n");
                return ERROR;
            }
            if(cerrar_fd() == ERROR)
            { 
                printf("Error cerrando descriptores\n\n");
                return ERROR;
            }
            if(execvp(ordenes[i], args[i]) == -1){
                printf("microshell_Ali : ");
                printf(magenta"%s"RESET, ordenes[i]);
                printf(": commando no encontrado\n");
                return ERROR;
            }
        }
    }

    cerrar_fd();
    int status;
    while (wait(&status) != -1);

    // implementar lo del background i lo de les señals
    if (bgnd)
    {
        if (status) printf("Killed");
        else if(status == 0) printf("Done");
        else if(status == -1) printf("Exit");
    }
    
    return OK;
} // Fin de la funcion "ejecutar"
