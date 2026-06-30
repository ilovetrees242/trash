CC = gcc
LDFLAGS = -lreadline
PREFIX = /usr/local

all: trash

trash: main.o builtins.o utils.o
	$(CC) $(LDFLAGS) -o trash main.o builtins.o utils.o
main.o: main.c builtins.h utils.h
	$(CC) -c main.c -o main.o
builtins.o: builtins.c builtins.h
	$(CC) -c builtins.c -o builtins.o
utils.o:
	$(CC) -c utils.c -o utils.o

clean:
	rm -f *.o
install: trash
	install -v -m755 trash $(PREFIX)/bin
