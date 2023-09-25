CC = gcc
CFLAGS =  
LDFLAGS = -pthread -lm
TARGET = PiProcesos
SOURCES = ../Procesos.c

all: $(TARGET)

$(TARGET):$(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

