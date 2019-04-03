#Reference: Note from Moodle in operating system module- Make File example
#https://2019-moodle.dkit.ie/course/view.php?id=8732

CC      = gcc
CFLAGS  = -c -Wall
LIBDIR  = lib
BINDIR  = dist
OBJECTS = dist/main.o dist/my-functions.o


default: link

link: $(OBJECTS)
	$(CC) $? -o $(BINDIR)/main

$(BINDIR)/main.o: main.c
	$(CC) $(CFLAGS) main.c -o $(BINDIR)/main.o

$(BINDIR)/my-functions.o: $(LIBDIR)/my-functions.c
	$(CC) $(CFLAGS) $(LIBDIR)/my-functions.c -o $(BINDIR)/my-functions.o

clean:
	rm -rf ./$(BINDIR) && mkdir $(BINDIR)
