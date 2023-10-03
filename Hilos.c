#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define HILOS 4

pthread_mutex_t mutex;
long double pi = 0.0L;

struct HilosArg {
  int start;
  int end;
};

void *calcularPi(void *args);

int main(int argc, char *argv[]) {

  if (argc < 2)
    exit(1);

  int iteraciones = atoi(argv[1]);

  long long start_ts;
  long long stop_ts;
  long long elapsed_time;
  struct timeval ts;

  int rang = iteraciones / HILOS;   // rango * hilo
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
      hilosArgs[i].end = iteraciones;
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

  printf("%d %.15Lf %.15f segundos: %lld\n", iteraciones, pi, M_PI,
         elapsed_time);
}

void *calcularPi(void *args) {
  struct HilosArg *hilosArgs = (struct HilosArg *)args;
  int status = (hilosArgs->end - hilosArgs->start);
  long double h = 1.0L / status;
  long double sum = 0.0L;
  long double area = 0.0L;
  int i;

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
  }

  pthread_mutex_lock(&mutex);
  pi += area;
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}
