all: Hilos

Hilos: Hilos.c
	gcc -o Hilos Hilos.c -lm -lpthread

run: Hilos
	./Hilos 100000000
