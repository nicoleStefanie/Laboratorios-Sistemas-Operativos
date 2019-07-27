#ifndef extensiones
#define extensiones
//Librerias
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//Definicion de constantes
#define READ     0
#define WRITE    1

//Variables Globales
int a = 0;
int pid = 1;
int signalReceived = 0;

#endif