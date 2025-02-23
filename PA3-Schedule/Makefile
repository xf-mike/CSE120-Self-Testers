# Makefile to compile UMIX Programming Assignment 3 (pa3) [updated 12/31/21]

LIBDIR = $(UMIXPUBDIR)/lib
LIB = $(LIBDIR)/libumix3.a

CC 	= cc 
FLAGS 	= -g -m32 -L$(LIBDIR) -lumix3

PA3 =	pa3a pa3b pa3c test

pa3:	$(PA3)

pa3a:	pa3a.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o pa3a pa3a.c mycode3.o

pa3b:	pa3b.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o pa3b pa3b.c mycode3.o

pa3c:	pa3c.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o pa3c pa3c.c mycode3.o

mycode3.o:	mycode3.c aux.h sys.h mycode3.h
	$(CC) $(FLAGS) -c mycode3.c

testbase.o:	testbase.c aux.h testbase.h
	$(CC) $(FLAGS) -c testbase.c
	
test: test.c aux.h umix.h testbase.o $(LIB)
	$(CC) $(FLAGS) -o test test.c testbase.o

clean:
	rm -f *.o $(PA3)
