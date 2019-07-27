#include "extensiones.h"
/*
Funcion principal. Obtiene los valores de -n , -L, -X , -Y , -d , -b
  -n : Cantidad de fotones 
  -L : Distancia máxima que puede recorrer el fotón
  -X : Dimensión del eje x de la grilla
  -Y : Dimensión del eje y de la grilla
  -d : Delta, distancia entre cada límite de la grilla
  -b : Permite ver el seguimiento de fotones en la grilla

  Si se recibe algun parámetro que no sea correcto, se entregará el mensaje 
  correspondiente al error causado */

int main(int argc, char **argv){
  float d_value;
  int n_value, x_value, y_value, c;
  opterr = 0; 
  while((c = getopt(argc, argv, "n:L:X:Y:d:b")) != -1){
    switch (c){
      case 'n'://Cantidad de fotones 
        sscanf(optarg, "%d", &n_value);
        printf("fotones: %d\n", n_value);
        break;
      case 'L'://distancia MAXIMA que puede recorrer el fotón.
        sscanf(optarg, "%f", &l_value);
        printf("distancia max: %f\n", l_value);
        break;
      case 'X'://Dimensión del eje x de grilla
        sscanf(optarg, "%d", &x_value);
        printf("eje x: %d\n", x_value);
        break; 
      case 'Y'://Dimensión del eje y de grilla
        sscanf(optarg, "%d", &y_value);
        printf("eje y: %d\n", y_value);
        break; 
      case 'd'://Delta, distancia entre cada límite de la grilla
        sscanf(optarg, "%f", &d_value);
        printf("delta: %f\n", d_value);
        break;
      case 'b'://Ver el seguimiento de fotones en la grilla
        b_flag = 1;
        printf("bflag: %d\n\n", b_flag);
        break;  
      case '?':
        if(optopt == 'n' || optopt == 'L' || optopt == 'X' || optopt == 'Y' || optopt == 'd'){
          fprintf(stderr, "Opción -%c requiere un argumento\n", optopt);
        }       
        else if(isprint(optopt))
          fprintf(stderr, "Opción desconocida -%c.\n", optopt);
        else
          fprintf(stderr, "Opción con caracter desconocido x%x'\n", optopt);
        return 1;
      default:
        abort();
    }
  }
  srand(time(NULL));
  //Procedimiento principal que realiza la ejecucion del programa
  main_procedure(n_value, x_value, y_value, d_value);
  return 0;
}