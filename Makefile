CC = gcc 
CFLAGS = -Wall -Iinclude `pkg-config --cflags gtk4`
LIBS = `pkg-config --libs gtk4` -lm
SRC = src/main.c src/signal.c src/calc.c src/engine.c
OUT = nit-calculator

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)
clean : 
	rm -f $(OUT)
