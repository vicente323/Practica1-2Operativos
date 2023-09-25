#include <math.h>
#include <stdio.h>

#define IT 1000
// f(x) = (1-x^2)^(1/2)
double f(double y);
double calcularPi(int n);
int main(void) {

  double Pi = calcularPi(IT);
  printf("The aproximated value of PI is: %f", Pi);
}

double f(double y) { return sqrt(1 - y * y); }

double calcularPi(int n) {
  double h = 1.0 / n;
  double sum = 0.0;
  int i;

  for (i = 0; i < n; i++) {
    double x1 = i * h;       // base
    double x2 = (i + 1) * h; // Xn -Xn+1
    double y1 = f(x1);
    double y2 = f(x2);

    // Área del trapecio
    double area = (y1 + y2) * h / 2.0;

    sum += area;
  }

  return sum * 4.0;
}