# Makefile to compile UMIX Programming Assignment 2 (pa2)

LIBDIR = $(UMIXPUBDIR)/lib
LIB = $(LIBDIR)/libumix2.a

CC 	= cc 
FLAGS 	= -g -m32 -L$(LIBDIR) -lumix2

PA2 =	pa2a pa2b pa2c test

pa2:	$(PA2)

pa2a:	pa2a.c aux.h umix.h $(LIB)
	$(CC) $(FLAGS) -o pa2a pa2a.c

pa2b:	pa2b.c aux.h umix.h mycode2.h mycode2.o $(LIB)
	$(CC) $(FLAGS) -o pa2b pa2b.c mycode2.o

pa2c:	pa2c.c aux.h umix.h mycode2.h mycode2.o $(LIB)
	$(CC) $(FLAGS) -o pa2c pa2c.c mycode2.o

test:	test.c aux.h umix.h mycode2.h mycode2.o $(LIB)
	$(CC) $(FLAGS) -o test test.c mycode2.o

mycode2.o:	mycode2.c aux.h umix.h mycode2.h
	$(CC) $(FLAGS) -c mycode2.c

clean:
	rm -f *.o $(PA2)
