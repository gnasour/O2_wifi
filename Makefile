CC=gcc
CFLAGS=-I.
DEPS = dbcon.h client.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compile: dbcon.c server.c client.c $(DEPS)
	g++ dbcon.c -o dbcon -lsqlite3
	g++ server.c client.c -o server -DDEBUG
