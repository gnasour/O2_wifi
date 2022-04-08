CC=gcc
CFLAGS=-I.
DEPS = dbcon.h 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compile: dbcon.c server.c $(DEPS)
	g++ dbcon.c -o dbcon -lsqlite3
	g++ server.c -o server
