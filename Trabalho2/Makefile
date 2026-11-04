CC = gcc
CFLAGS = -Wall -Wextra -g
PROG = t2
FONTES = main.c mymemory.c
OBJETOS = $(FONTES:.c=.o)

all: $(PROG)

$(PROG): $(OBJETOS)
	$(CC) $(CFLAGS) $(OBJETOS) -o $(PROG)

main.o: main.c mymemory.h
	$(CC) $(CFLAGS) -c main.c

mymemory.o: mymemory.c mymemory.h
	$(CC) $(CFLAGS) -c mymemory.c

run: $(PROG)
	./$(PROG)

# Verificacao de vazamento de memoria (macOS)
leaks: $(PROG)
	leaks --atExit -- ./$(PROG)

# Pacote de entrega
entrega:
	zip T2-mymemory.zip mymemory.h mymemory.c main.c Makefile

clean:
	rm -f $(OBJETOS) $(PROG)
