CC = gcc
LDFLAGS = -lreadline

all: trash

install: trash
	install -v -m755 trash /usr/bin
trash: main.o builtins.o
	$(CC) $(LDFLAGS) -o trash main.o builtins.o
main.o: main.c builtins.h
	$(CC) $(LDFLAGS) -c main.c -o main.o
builtins.o: builtins.c builtins.h
	$(CC) $(LDFLAGS) -c builtins.c -o builtins.o
