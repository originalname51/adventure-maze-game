
CFLAGS = -Wall -fpic -coverage -lm

adventure.o: adventure.h adventure.c
	gcc -c adventure.c -g  $(CFLAGS)
	
	
unit_test: unit_test.c adventure.o
	gcc -o unit_test -g unit_test.c adventure.o $(CFLAGS)		

adventure: main.c adventure.o
	gcc -o adventure -g main.c adventure.o $(CFLAGS)		
	
all: adventure

runtests: unit_test
	./unit_test
	gcov adventure.c -b > unittestresult.out
	cat adventure.c.gcov >> unittestresult.out
	
clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so unit_test adventure unittestresult.out
	rm -rf stinero.rooms.*
