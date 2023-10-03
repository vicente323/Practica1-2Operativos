CC = gcc
CFLAGS =  
LDFLAGS = -pthread -lm
TARGET = PiHilos
SOURCES = ../Hilos.c

all: $(TARGET)

$(TARGET):$(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

