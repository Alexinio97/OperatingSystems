main : main.o myHandlers.o
	gcc -o main main.o myHandlers.o
main.o : main.c
	gcc -c main.c
myHandlers.o : myHandlers.c
	gcc -c myHandlers.c	
clean : 
	rm -f *.o
