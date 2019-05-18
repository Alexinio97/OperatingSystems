main : main.o FileOperations.o
	gcc -o main main.o FileOperations.o
main.o : main.c
	gcc -c main.c
FileOperations.o : FileOperations.c
	gcc -c FileOperations.c
clean : 
	rm -f *.o