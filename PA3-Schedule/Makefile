# Add this content to your Makefile

testbase.o:	testbase.c aux.h testbase.h
	$(CC) $(FLAGS) -c testbase.c
	
test: test.c aux.h umix.h testbase.o $(LIB)
	$(CC) $(FLAGS) -o test test.c testbase.o
