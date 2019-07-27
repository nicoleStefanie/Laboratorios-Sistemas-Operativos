#include "extensiones.h"
/*
Procedimiento que se encarga de crear la representacion de la grilla el cual tiene dos
dimensiones y un delta que corresponde a la distancia que hay entre las celdas dentro de
la grilla. Inicialmente todas las celdas tiene unidad de energia igual a 0. Tambien se 
crean la cantidad de fotones correspondientes para poder lanzarlos concurrentemente (eje-
cutandose el procedimiento "radiative_transfer"), una vez lanzado todos los fotones, se 
procede a escribir en el archivo de salida los valores de unidades energias finales.
	n: Cantidad de fotones a lanzar.
	x: Dimension del eje x de la grilla.
	y: Dimension del eje y de la grilla.
	d: Delta, distancia entre celdas. */
void main_procedure(int n, int x, int y, float d)
{ 
	int i, j, aux, k = 0;
	float min1, max1, min2, max2;
	//Los valores de las dimensiones de la grilla son numeros pares
	if(x % 2 == 0 && y % 2 == 0){
		//Creacion de la representacion de la grilla
		grilla = (Grid*)malloc(sizeof(Grid)); 
		grilla->matrix = (int**)malloc(sizeof(int*) * x);
		//Eje x de la grilla
		grilla->x_axis = x; 
		//Eje y de la grilla
		grilla->y_axis = y; 
		//Cantidad total de celdas de la grilla
		cell_total = x*y;
		//Crear las celdas de la grilla 
		grilla->cell = (Cell*)malloc(x*y*sizeof(Cell));
		if (grilla->matrix != NULL){
			for (i = 0; i < x; i++){
				grilla->matrix[i] = (int*)calloc(y, sizeof(int));
				for (j = 0;j < y; j++){
					//Cada celda de la grilla parte con energia igual a 0
					grilla->matrix[i][j]= 0; 
					//Columna asociada a la celda
					grilla->cell[k].x = j;
					//Fila asociada a la celda
					grilla->cell[k].y = i;
					//Pasar a la siguiente celda
					k++;
				}
			}	
		}	
		//Actualizar valor de k
		k = 0;
		//Distancia maxima de la grilla (eje x)
		max1 = d*(x/2);
		//Distancia minima de la grilla (eje x)
		min1 = max1 - (max1*2);
		//Distancia maxima de la grilla (eje y)
		max2 = d*(y/2); 
		//Distancia maxima de la grilla (eje y)
		min2 = max2 - (max2*2); 
		//Recorrer las celdas dentro de la grilla
		for (i = 0; i < x; i++){
			for (j = 0; j < y; j++){
				//Limite izquierdo
				grilla->cell[k].y_left_limit = min2;
				//Limite derecho
				grilla->cell[k].y_right_limit = min2 + d;
				//Limite de arriba
				grilla->cell[k].x_up_limit = min1;
				//Limite de abajo
				grilla->cell[k].x_down_limit = min1 + d;
				//Se muestran la celda con sus respectivos limites 
				printf("\ncelda: %d : ", k);
				printf("	 limite izq: %f\n", grilla->cell[k].y_left_limit);
				printf("		 limite der: %f\n", grilla->cell[k].y_right_limit);
				printf("		 limite arriba: %f\n",grilla->cell[k].x_up_limit);
				printf("		 limite abajo %f\n\n", grilla->cell[k].x_down_limit);
				//Ahora el limite izquierdo es el limite derecho del anterior
				min2 = grilla->cell[k].y_right_limit;
				//Pasar a la siguiente celda
				k++;
			}
			//Actualizar valores de las celdas de la primera columna en grilla
			min2 = grilla->cell[k-y].y_left_limit;
			min1 = grilla->cell[k-y].x_down_limit;
		}
		//Arreglo de hebras que corresponden a la cantidad de fotones 
		pthread_t hebra[n];
		//Se crean las n hebras que lanzarán fotones
		for(aux = 0; aux < n; aux++){
			//Se pasa como parámetro el valor correpondiente a la hebra del arreglo
	      	pthread_create(&hebra[aux], NULL, (void*)radiative_transfer, &aux); 
	    }
	    //Se espera a que finalicen las n hebras que estan lanzando fotones en la grilla
	    for(j = 0; j < n; j++){
	      pthread_join(hebra[j], NULL);
	    } 
	    //Se escribe en el archivo de salida la grilla con los valores finales de energia
	    writeinfile();
	}
	//Si las dimensiones no son pares, no se puede crear la grilla
	else{
		printf("Las dimensiones del eje x e y de la grilla deben ser numeros pares\n");
	}
}

/*
Procedimiento que se encarga de realizar la transferencia radiativa en disco protoplanetarios,
en donde los fotones realizan Absorcion o Difusion. Los n fotones realizan este procedimiento,
en donde para cada uno de manera concurrente se calcula iterativamente una posicion dentro de la
grilla (en base a una distancia a recorrer y una direccion, ambas aleatorias) a realizar un even-
to determinado. Esto se realiza en base a una distancia maxima que puede recorrer cada foton, es
decir, esto se realiza hasta que el foton haya completado esa distancia maxima. 
Cuando el evento es Absorcion, el foton agrega una unidad de energia en la celda la cual esta po-
sicinado.
Cuando el evento es Difusion, el foton no realiza nada y se calcula una nueva posicion a posicio-
narse dentro de la grilla.*/
void radiative_transfer(void *params)
{
	int j = *((int*) params);
	int i, row, column, event, absorcion; 
	float actual_distance = 0.0, position_x, position_y, actual_x = 0, actual_y = 0;
	float xx , yy;
	//Mientras la distancia que lleva el foton sea menor o igual a la distancia maxima
	while(actual_distance <= l_value){
		float  r, h, distance, e, f, magnitude, unit_x, unit_y, dd;
		//Se calcula la distancia a recorrer del foton
		r = (float)rand()/((float)RAND_MAX + 1);
		h = 1 - r;
		distance = -log(h);
		//Se calcula el vector unitario
		float *vector = malloc(2 *sizeof(float));
		//Dos puntos aleatorios
		e = (double)rand()/RAND_MAX*1.5-0.5;
		f = (double)rand()/RAND_MAX*1.5-0.5;
		magnitude = sqrt(pow(e,2)+pow(f,2));
		unit_x = e / magnitude;
		unit_y = f / magnitude;
		//Puntos del vector unitarioe
		vector[0] = unit_x; 
		vector[1] = unit_y;
		//Distancia actualizada del foton
		dd = actual_distance + distance;
		//Si la distancia del foton es mayor a la distancia maxima a recorrer
		if (dd > l_value){
			float index = l_value - actual_distance;
			/*Se calcula una posicion en base a la distancia que le falta por recorrer
			para llegar a la distancia maxima que puede recorrer*/
			position_x = index + vector[0];
			position_y = index + vector[1];
			xx = position_x;
			yy = position_y;
			//El ultimo evento (Absorcion) que realiza antes de terminar su trayectoria
			absorcion = 2;
			position_x = position_x + actual_x;
			position_y = position_y + actual_y;
		}
		//Si todavia no llega a la distancia maxima
		else{
			//Se calcula una posicion en base a una distancia aleatoria y un vector unitario
			position_x = distance + vector[0];
			position_y = distance + vector[1];
			xx = position_x;
			yy = position_y;
			position_x = position_x + actual_x;
			position_y = position_y + actual_y;
		}
		//Recorrer celdas de la grilla
		for(i = 0; i < cell_total; i++){
			if (position_x >= grilla->cell[i].x_up_limit && position_x < grilla->cell[i].x_down_limit){
				if (position_y >= grilla->cell[i].y_left_limit && position_y < grilla->cell[i].y_right_limit){

					actual_x = position_x;
					actual_y = position_y;
					//printf("Foton %d en (%f, %f)\n", j,actual_x, actual_y);
					//Se llega a la celda correspondiente
					pthread_mutex_lock(&mutex);
					//Se obtiene fila y columna de la celda 
					row = grilla->cell[i].x;
					column = grilla->cell[i].y;
					//Se calcula el evento a ocurrir			
					event = (rand() / (double)RAND_MAX) < 0.5;
					//Evento Absorcion
					if (event == 1 || absorcion == 2){
						//Se suma 1 unidad de energia en la celda que esta el foton
						grilla->matrix[row][column] = grilla->matrix[row][column] + 1;
						//Actualizar la distancia que ha recorrido el foton
						actual_distance = actual_distance + distance;
					}
					//Evento Difusion
					else{
						//Actualizar la distancia que ha recorrido el foton
						actual_distance = actual_distance + distance;
					}
					//Salirse del for y buscar otra posicion para el foton
					i = cell_total; 
					//Imprimir seguimiento de fotones
					if (b_flag == 1){
						if(actual_distance > l_value){
							printf("Foton %d en celda [%d][%d] , ", j, row, column);
							printf("Posicion calculada: (%f, %f)\n",xx, yy);
							printf("en posicion (%f, %f): Absorcion.  ", position_x , position_y);
							printf("Distancia calculada: %f\n",distance);
							printf("Distancia recorrida: %f\n", l_value);
							printf("-----------------------------------------------------------\n");
						}
						else if (event == 1){
							printf("Foton %d en celda [%d][%d] , ", j, row, column);
							printf("Posicion calculada: (%f, %f)\n",xx, yy);
							printf("en posicion (%f, %f): Absorcion.  ", position_x , position_y);
							printf("Distancia calculada: %f\n",distance);
							printf("Distancia recorrida: %f\n", actual_distance);
							printf("-----------------------------------------------------------\n");
						}
						else{
							printf("Foton %d en celda [%d][%d] , ", j, row, column);
							printf("Posicion calculada: (%f, %f)\n",xx, yy);
							printf("en posicion (%f, %f): Difusion.  ", position_x , position_y);
							printf("Distancia calculada: %f\n",distance);
							printf("Distancia recorrida: %f\n", actual_distance);
							printf("-----------------------------------------------------------\n");
						}
					}
					pthread_mutex_unlock(&mutex);
				}
			}	
		}
	}
}
/*
Procedimiento que se encarga de escribir en un archivo de texto los valores de energia en cada 
posicion de la grilla. Este procedimiento se realiza luego de que todas las hebras hayan termi-
nado su ejecucion del prcedimiento "radiative_transfer". Este procedimiento se realiza una vez
que todos los fotones hayan sido lanzado y hayan terminado su trayectoria.*/
void writeinfile(){
	int i, j;
  	//Se crea un archivo llamado "out.txt" para escribir la grilla
  	FILE* f = fopen("out.txt", "wb");
  	if(f != NULL){
  		for (i = 0; i < grilla->x_axis; i ++){
  			for (j = 0; j < grilla->y_axis; j++){
  				//Se escribe la posicion de la grilla con su valor de energia correspondiente
  				fprintf(f, "< %d [%d][%d]>\n", grilla->matrix[i][j], i, j);
  			}
  		}
  	}
  	else{
    	printf("Error al crear el archivo de salida.\n");
  	}
  	fclose(f);
}