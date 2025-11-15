/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :                                    |
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |                              
 |     Descripcion:                                    |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include <unistd.h>


REDIRECCION_ORDENES red_ordenes;

void redireccion_ini(void)
{
    for (int i = 0; i < PIPELINE; i++) {
        red_ordenes[i].entrada = 0;
        red_ordenes[i].salida = 1;
    }
}//Inicializar los valores de la estructura cmdfd

int pipeline(int nordenes, char *infile, char *outfile, int append, int bgnd)
{
    redireccion_ini();

    int tubo[2], fd;
    for (int i = 0; i < nordenes - 1; i++)
    {
        if(pipe(tubo) == -1) return ERROR;
        red_ordenes[i].salida = tubo[1];
        red_ordenes[i + 1].entrada = tubo[0];
    }
    if (infile[0] != '\0')
    {
        fd = open(infile, O_RDONLY);
        red_ordenes[0].entrada = fd;
    }
    if (outfile[0] != '\0')
    {    
        if (append) fd = open(outfile, O_WRONLY|O_CREAT|O_APPEND, 0666);
        else fd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        
        red_ordenes[nordenes - 1].salida = fd;
    }
    if (bgnd)
    {
        fd = open("/dev/null", O_WRONLY);
        red_ordenes[0].entrada = fd;
    }
    
    return OK;
} // Fin de la función "pipeline"

int redirigir_entrada(int i)
{
    if (red_ordenes[i].entrada != 0)
    {
        if(dup2(red_ordenes[i].entrada, STDIN_FILENO)== -1) return ERROR;
    } 
    return OK;
} // Fin de la función "redirigir_entrada"

int redirigir_salida(int i)
{
    if (red_ordenes[i].salida != 1)
    {
        if(dup2(red_ordenes[i].salida, STDOUT_FILENO) == -1) return ERROR;
    } return OK;
} // Fin de la función "redirigir_salida"

int cerrar_fd()
{
    for (int i = 0; i < PIPELINE; i++)
    {
        if (red_ordenes[i].entrada != 0) if(close(red_ordenes[i].entrada) == -1) return ERROR;
        if (red_ordenes[i].salida != 1 ) if(close(red_ordenes[i].salida) == -1 ) return ERROR;
    }
    return OK;
} // Fin de la función "cerrar_fd"


