#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>



#define HILOS 4
#define IT 10000

pthread_mutex_t mutex;
long double pi = 0.0L;

struct HilosArg {
  int start;
  int end;
};

// f(x) = (1-x^2)^(1/2)
// double f(double y);
void *calcularPi(void *args);

int main(void) {
  long long start_ts;
  long long stop_ts;
  long long elapsed_time;
  struct timeval ts;

  int rang = IT / HILOS;            // rango * hilo
  pthread_t hilos[HILOS];           // arr de hilos
  struct HilosArg hilosArgs[HILOS]; // arr de tipo HilosArg
  pthread_mutex_init(&mutex, NULL);

  // pedir N

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec;

  int i;
  for (i = 0; i < HILOS; i++) { // asignar inicio y fin de cada hilo
    hilosArgs[i].start = i * rang;
    hilosArgs[i].end = (i + 1) * rang;

    if (i ==
        HILOS - 1) { // Para que el ultimo hilo tenga los elementos restantes
      hilosArgs[i].end = IT;
    }
    pthread_create(&hilos[i], NULL, calcularPi, &hilosArgs[i]); // inicio Hilos
  }

  for (i = 0; i < HILOS; i++) {
    pthread_join(hilos[i], NULL);
  }

  pthread_mutex_destroy(&mutex);

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec;
  elapsed_time = stop_ts - start_ts;

  // double Pi = calcularPi(IT);
  printf("The aproximated value of PI is: %.15Lf\n", pi);
  printf("-----------------------------------------\n");
  printf("Tiempo Total: %lld segundos\n", elapsed_time);
}

// double f(double y) { return sqrt(1 - y * y); }

void *calcularPi(void *args) {
  struct HilosArg *hilosArgs = (struct HilosArg *)args;
  int status = (hilosArgs->end - hilosArgs->start);
  long double h = 1.0L / status;
  long double sum = 0.0L;
  long double area = 0.0L;
  int i;
  // printf("inicio: %d -- fin: %d valor de h:
  // %Lf\n",hilosArgs->start,hilosArgs->end,h);
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
    // printf("x1= %Lf -- x2= %Lf -- y1= %Lf -- y2= %Lf Inicio:%d
    // FIn:%d\n",x1,x2,y1,y2,hilosArgs->start,hilosArgs->end);

    // Área del trapecio
    area += (y1 + y2) * h / 2.0L;
    // printf("Inicio %d y Final %d AREA=
    // %Lf\n",hilosArgs->start,hilosArgs->end, area);
  }
  // printf("Inicio %d y Final %d AREA= %Lf\n",hilosArgs->start,hilosArgs->end,
  // area);
  pthread_mutex_lock(&mutex);
  pi += area;
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}
