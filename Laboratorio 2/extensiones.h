#ifndef extensiones
#define extensiones
//Librerias
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//Estructura para representar una celda dentro de la grilla
typedef struct {
	int x; //columna de la matriz 
	int y; //fila de la matriz 
	float x_up_limit; // Limite de arriba de una celda en la grilla (eje x)
	float x_down_limit;// Limite de abajo de una celda en la grilla (eje x)
	float y_right_limit;// Limite derecho de una celda en la grilla (eje y)
	float y_left_limit; // Limite izquierdo de una celda en la grilla (eje y)
}Cell;

//Estructura para representar la grilla
typedef struct{
	int **matrix;
	int x_axis;// Distancia eje x
	int y_axis;// Distancia eje y
	Cell *cell; // Celdas dentro de la grilla
}Grid;

//Variables Globales
int b_flag;
Grid *grilla;
float l_value;
int cell_total;
pthread_mutex_t mutex;

//Funciones
void writeinfile();
void radiative_transfer(void *params);
void main_procedure(int n, int x, int y, float d);

#endif