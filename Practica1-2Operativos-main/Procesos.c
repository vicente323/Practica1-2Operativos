#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/shm.h>

/*

Es recomendable usar segmentos de
memoria compartida, la cual la puedes solicitar al sistema con la llamada al
sistema shmget() y conectar con la llamada al sistema shmat()

*/

#define N_PROCESOS 4
#define IT 10

long double pi = 0.0L;
float n;


// f(x) = (1-x^2)^(1/2)
//double f(double y);
long double calcularPi(int start, int end);

int main(void) {
    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    struct timeval ts;
    
    pid_t pids[N_PROCESOS]; //arr de procesos
    int status;
    float *sumas;
    
    // Creamos una region de memoria compartida
    int shmid = shmget(IPC_PRIVATE, N_PROCESOS * sizeof(float), IPC_CREAT | 0666);
    
    // Adjuntar la region de memoria compartida a sumas
    sumas = (float *)shmat(shmid, NULL, 0);
    
    //pedir N 
    printf("Dame el valor de N :");
    scanf("%f",&n);

    int rang = n / N_PROCESOS; //rango * proceso
    
    gettimeofday(&ts,NULL);
    start_ts = ts.tv_sec;
    
    int i;
    for (int i = 0; i < N_PROCESOS; i++) {
        if ((pids[i] = fork()) == 0) {
            // Código ejecutado por el proceso hijo
            int start = i * rang;
            int end = (i + 1) * rang;
    
            if (i == N_PROCESOS - 1) {
                // Asegurar que el último proceso calcule los elementos restantes
                end = n;
            }
            sumas[i] = calcularPi(start, end);
            exit(0);
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < N_PROCESOS; i++) {
        waitpid(pids[i], &status, 0);
    }
    // Sumar los resultados de los procesos hijos
    for (int i = 0; i < N_PROCESOS; i++) {
        pi += sumas[i];
    }

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec;
  elapsed_time = stop_ts - start_ts;
  
  //double Pi = calcularPi(IT);
  printf("The aproximated value of PI is: %.15Lf\n",pi);
  printf("-----------------------------------------\n");
  printf("Tiempo Total: %lld segundos\n",elapsed_time);
}

long double calcularPi(int start, int end) {
  int status = (end - start);
  long double h = 1.0L / status;
  long double sum = 0.0L;
  long double area = 0.0L;
  int i;
  //printf("inicio: %d -- fin: %d valor de h: %Lf\n",start,end,h);
  for (i = 0; i < status; i++) {
  	long double x1 = i * h;
    long double x2 = (i + 1) * h;
    long double temp1 = 1.0L - x1 * x1;
    long double temp2 = 1.0L - x2 * x2;
	
    if (temp1 < 0.0L) {
        temp1 = 0.0L;
    }
	
    if (temp2 < 0.0L) {
        temp2 = 0.0L;
    }
	
    long double y1 = sqrtl(temp1);
    long double y2 = sqrtl(temp2);
	
    // Área del trapecio
    area += (y1 + y2) * h / 2.0L;
    //printf("Inicio %d y Final %d AREA= %Lf\n",start,end, area);
  }
  	//printf("Inicio %d y Final %d AREA= %Lf\n",hilosArgs->start,hilosArgs->end, area);
    return area;
  
}
