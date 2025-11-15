/*-----------------------------------------------------+
 |     U S H. C                                        |
 +-----------------------------------------------------+
 |     Versión :                                       |                      
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |                               
 |     Descripción :                                   |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "colors.h"

//
// Declaraciones de funciones locales
//
void visualizar(void);
int leerLinea(char *linea, int maxLinea);


//
// Prog. ppal.
// 
int main(int argc, char * argv[])
{
    
  char line[255];
  int res;
  char **m_ordenes;
  char ***m_argumentos;
  int *m_num_arg;
  int m_n;

  while(1)
  {
    
    do
    {
        res=leerLinea(line,MAXLINE);    
        if (res==-2) {
      		fprintf(stdout,"logout\n");
		      exit(0);
        }
	
    	  if (res==-1){
		      fprintf(stdout,"linea muy larga\n");
	      }
     }while(res<=0);

     if (analizar(line)==OK){
       m_n=num_ordenes();
	     m_num_arg=num_argumentos();
	     m_ordenes=get_ordenes();
	     m_argumentos=get_argumentos();
	     if(m_n>0)
	     {
          if (pipeline(m_n,fich_entrada(),fich_salida(),es_append(),es_background())==OK) 
              if(ejecutar(m_n,m_num_arg,m_ordenes,m_argumentos,es_background()) == OK)
                  visualizar();
        }
     }
 }    

  return 0;
}





/****************************************************************/
/*                       leerLinea                             
  --------------------------------------------------------------
                                                               
   DESCRIPCIÓN:                                                 
   Obtiene la línea de órdenes para el mShell.    
   Util para depuracion.                                        
                                                                
   ENTRADA:                                                 
    linea - puntero a un vector de carácteres donde se almancenan los caracteres 
   leídos del teclado
    tamanyLinea - tamaño máximo del vector anterior

   SALIDA:
    -- linea - si termina bien, contiene como último carácter el retorno de carro.
    -- leerLinea -  Entero que representa el motivo de finalización de la función:
     > 0 - terminación correcta, número de caracteres leídos, incluído '\n'
     -1 - termina por haber alcanzado el número máximo de caracteres que se 
    espera leer de teclado, sin encontrar '\n'.
     -2 - termina por haber leído fin de fichero (EOF).
*/
/****************************************************************/
//char * getline(void)
int leerLinea(char *linea, int maxLinea)
{  
  printf(rosa"%s, %s", getcwd(NULL, 1024), PROMPT);
  printf(RESET);
	int i = 0;
	
  while(1){
    char c = getchar();
    linea[i] = c;
		if(i > maxLinea) return -1;
		if(linea[i] == EOF) return -2;
    if(linea[i] == '\n'){ 
      printf("Número de caracteres: %d\n", i);
      break;
    }
		i++;
	}		
  return OK; 
}




/****************************************************************/
/*                       visualizar                             */
/*--------------------------------------------------------------*/
/*                                                              */
/* DESCRIPCIÓN:                                                 */
/* Visualiza los distintos argumentos de la orden analizada.    */
/* Util para depuracion.                                        */
/*                                                              */
/* ENTRADA: void                                                */
/*                                                              */
/* SALIDA: void                                                 */
/*                                                              */
/****************************************************************/
void visualizar(void)
{  
  printf(magenta"\n************************************************************\n*");
  printf(blauClaret"                   VISUALIZADOR ALI                       "RESET);
  printf(magenta"*\n************************************************************\n\n"RESET);

  printf("Número de ordenes: ");
  printf(verdClaret"%d\n\n", num_ordenes());
  printf(RESET);
  for (int i = 0; i < num_ordenes(); i++)
  {
    printf("Orden nº %d: ", i + 1);
    printf(roig"%s\n"RESET, get_ordenes()[i]);
    printf("Número de argumentos: ");
    printf(verdClaret"%d\n"RESET, num_argumentos()[i]);

    for (int j = 0; j < num_argumentos()[i]; j++)
    {
      printf("Argumento nº %d: ", j + 1);
      printf(roig"%s\n"RESET, get_argumentos()[i][j]);
    }
    printf("\n");
  }
  
  if(fich_entrada()[0] != '\0')
  {
    printf("Fichero de Entrada: ");
    printf(groc"%s\n"RESET, fich_entrada());
  }
  else printf("No hay fichero de entrada\n");

  if(fich_salida()[0] != '\0'){
    printf("Fichero de Salida: ");
    printf(groc"%s\n"RESET, fich_salida());

    printf("La redirección de salida se ha realizado en modo ");
    printf(verdClaret);

    if(es_append()) printf("Append\n\n");
    else printf("Trunk\n\n");
    
    printf(RESET);
  } else printf("No hay fichero de salida\n\n");
  
  printf(cyan);
  if(es_background())printf("Se ha lanzado en background\n\n");
  else printf("Se ha lanzado en foreground\n\n");
  
  printf(RESET);
} // Fin de "visualizar"



