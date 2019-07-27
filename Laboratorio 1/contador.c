#include "extensiones.h"

/*
Archivo que corresponde a funcion que es llamada desde execv el cual proceso hijo creado 
ejecutara esta funcion. Lo que realiza es simplemente ir sumando un contador */
int main(int argc, char **argv){
	//Pasar a valor numérico el argumento que viene del execv que es string 
	a = atoi(argv[1]);
	//Se suma uno al contador
    a++;      
    //Se escribe en el pipe el valor actualizado del contador
    write(STDOUT_FILENO, &a, sizeof(a));
   	return 0;
}




