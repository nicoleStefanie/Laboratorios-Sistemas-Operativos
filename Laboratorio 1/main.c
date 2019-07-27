#include "extensiones.h"
/*
Funcion que asigna comprtamiento de las distintas señales dándoles el valor para cada una de ellas, 
en donde se les asigna una variable global*/
void signalHandler(int n)
{//Se va comparando el tipo de señal
  if(n == SIGUSR1)
  {
    signalReceived = 1;
  }
  else if(n == SIGUSR2)
  {
    signalReceived = 2;
  }
  else if(n == SIGINT)
  {
    //Proceso hijo indica que lo quieren matar
    printf("Soy el proceso con pid %d y estoy vivo aún. No me mates papá\n", getpid()); 
    //Resetear señal a su defecto
    signal(SIGINT,SIG_DFL);
  }
}
/*
Funcion principal. Obtiene los valores de -h y -m.
  -h : Cantidad de procesos a crear.
  -m : Bandera el cual si esta presente, se deben mostrar por terminal los procesos creados
       con sus respectivos pid. 
  Si se recibe algun parámetro que no sea correcto, se entregará el mensaje correspondiente
  al error causado*/
int main(int argc, char **argv){
  int hvalue = 0; //numero de hijos (argumento de -h)
  int mflag = 0; //bandera para -m
  opterr = 0; 
  int c;
  while((c = getopt(argc, argv, "h:m")) != -1)
  {
    switch (c)
    {
      case 'h':
        sscanf(optarg, "%d", &hvalue);
        break;
      case 'm':
        //En caso de que la bandera este presente, se le asigna el valor 1 
        mflag = 1;
        break;
      case '?':
        if(optopt == 'h'){
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
  if (hvalue <= 0)
  {//Si no hay un numero razonable de procesos    
    fprintf(stderr, "Debe ingresar argumento válido para h\n");
    return 1;
  }
  /*A partir de ahora se crean los procesos segun la variable hvalue que indica la cantidad 
  de estos que deben crearse y segun el valor de la bandera mflag se mostrarán los procesos 
  creados por la terminal.*/
  int j = 0;  
  int children_number;
  //Arreglo para guardar cada pid de los procesos hijos 
  int *childrenlist;
  childrenlist = (int*)malloc(sizeof(int)*hvalue);
  while (j < hvalue){
    if (pid > 0)
    {//Proceso padre
      pid = fork(); //Se crea nuevo proceso
      if(pid == 0)
      { //Proceso hijo 
        children_number = j + 1; //Indentificar proceso hijo con un numero
      }
      else if (pid > 0)
      {//Proceso padre
        childrenlist[j] = pid; //Se almacena pid de procesos hijos
      }
    }
    else if(pid < 0)
    {//Si no se pueden crear los procesos hijos
      printf("Error, finalizando ejecución\n");
      exit(1); 
    }
    j++;
  }
  if (mflag == 1)
  {//En caso de que la bandera se active, osea, sea igual a 1
    if(pid == 0)
    {//Proceso hijo se identificará
      printf("Soy el proceso %i y mi pid es: %i\n",children_number, getpid());
    }
  }
  /*
  A partir de ahora se realiza la ejecución principal, el cual va preguntando constantemente el número del
  proceso hijo y la señal que se quiere enviar a ese proceso hijo, las cuales son cuatro tipos de señales:

    SIGINT: Manejo de ctrl C, el cual al primer ctrl C se realiza un warning a cada proceso hijo que el pro-
            ceso padre los quiere matar. Al segundo ctrl C, mueren todos los procesos y se termina la ejecución
    SIGUSR1: El proceso crea un proceso hijo el cual por medio de execv realizara otra funcion llamada contador.c
            el cual mantendrá un contador que se irá sumando cada vez que reciba otra vez la misma señal
    SIGUSR2: El proceso crea un proceso hijo.
    SIGTERM: El proceso que reciba esta señal, morirá, antes si, debe indicar que quiere ser matado.*/
  int son;
  int status;
  char variable[2];
  int signal_number = 0;
  //A las señales se asignan un numero correspondiente llamando a la función signalHandler
  signal(SIGINT,signalHandler);
  signal(SIGUSR2,signalHandler);
  signal(SIGUSR1,signalHandler);
  //Variables para la ejecución de pipe
  int pipefd[2];
  int index;
  //Se crea el pipe para que los procesos se comuniquen, teniendo un lado de escritura y un lado de lectura
  pipe(pipefd);
  //bucle de ejecucion
  while(1)
  {
    if (pid > 0)
    {//Proceso padre
      sleep(1);
      printf("Ingresar el numero del hijo y señal a enviar (X - Y): \n");
      //Se guardan los parámetros
      scanf("%d - %d", &son, &signal_number); 
      if (son > hvalue || son <= 0)
      {
        printf("\nNo existe el hijo descrito\n");
        signal_number = 0;
      }
      else
      {//El proceso si existe
        if (signal_number == 16)//Señal SIGURS1
        {
          printf("\nLa señal %d seré enviada al hijo %d de pid %d,\n", signal_number, son, childrenlist[son-1]);
          //Comunicacion, se le envía la señal al proceso hijo indicado
          kill(childrenlist[son-1], SIGUSR1);
          printf("Soy el hijo %d y recibí SIGUSR1\n\n", childrenlist[son-1]);
          //Se resetea el valor de la señal
          signal_number = 0; 

        }
        else if(signal_number == 15)//Señal SIGTERM
        {
          printf("\nLa señal %d seré enviada al hijo %d de pid %d,\n", signal_number, son, childrenlist[son-1]);
          //Comunicacion, se le envía la señal al proceso hijo indicado
          kill(childrenlist[son-1], SIGTERM);
          printf("Soy el hijo %d y recibí SIGTERM\n\n", childrenlist[son-1]);
          printf("Soy el hijo con pid: %d y mi papá me quiere matar\n", childrenlist[son-1]);
          //Se resetea el valor de la señal 
          signal_number = 0;       
        }
        else if(signal_number == 17)//Señal SIGURS2
        {
          printf("\nLa señal %d seré enviada al hijo %d de pid %d,\n", signal_number, son, childrenlist[son-1]);
          //Comunicacion, se le envía la señal al proceso hijo indicado
          kill(childrenlist[son-1], SIGUSR2);
          printf("Soy el hijo %d y recibí SIGUSR2\n\n", childrenlist[son-1]);
          //Se resetea el valor de la señal     
          signal_number = 0;  
        }
      }
    }
    //Se identifica la acción correspondiente para la señal indicada
    if(signalReceived == 1)//Señal SIGURS1
    {
      sprintf(variable,"%d",a);
      pid = fork();
      if (pid == 0)
      {//Proceso hijo creado
        printf("Entre en el hijo\n"); 
        //Se duplica el descriptor para que contador.c pueda escribir del pipe.
        dup2(pipefd[1],STDOUT_FILENO);
        //Se cierra el lado de lectura del pipe, ya que el proceso hijo no lee si no que escribe en el pipe
        close(pipefd[0]);              
        char *args[] = {"./contador",variable, NULL};
        //Usando el execv se entrega el nombre del archivo que se va a ejecutar en el proceso hijo,
        //junto con la variable el cual actuará como contador
        execv(args[0], args);
        perror("Error en exec");
        exit(EXIT_FAILURE);
      }
      else{
        //Se lee del pipe el valor actualizado del contador
        read(pipefd[READ], &index, sizeof(index));
        printf("Padre: %d\n", index);
        //Actualizar variable global
        a = index;
        if (a == 1)
        {//Primer llamado
          printf("pid: %i, y he recibido esta llamada %d vez.\n", getpid(),a);
        }
        else{//Mas de un llamado
          printf("pid: %i, y he recibido esta llamada %d veces.\n", getpid(),a);
        }
      }
      sleep(1);
      //Matar al proceso hijo creado
      kill(pid,SIGKILL);
      //Resetear señal
      signalReceived = 0;
      pid = 0;
    }
    else if(signalReceived == 2)//Señal SIGURS2
    {
      //Se crea nuevo proceso hijo 
      pid = fork();
      //Se espera al proceso hijo creado
      waitpid(pid,&status,0);
      //Mensaje de proceso creado
      printf("Proceso creado.\n");
      //Resetear señal
      signalReceived = 0;
    }
  }
  return 0;
}