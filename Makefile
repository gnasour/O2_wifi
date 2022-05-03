CC=gcc
CFLAGS=-I.
DEPS = dbcon.h client.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compile: dbcon.c server.c client.c $(DEPS)
	gcc server.c client.c dbcon.c -o server -DDEBUG -lsqlite3 -Wall

debug:  dbcon.c server.c client.c $(DEPS)
	gcc server.c client.c dbcon.c -o server -DDEBUG -lsqlite3 -gstabs
