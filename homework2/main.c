#include <stdio.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>




void copyFile(char* source,char *destination,struct stat buff);

void copyDir(char* source,char * destination,int linkCopy);


//copy the content from a file to another
int main(int argc, char *argv[])
{
    char* source = argv[1];
    char* destination = argv[2];
    int linkCopy; //this argument is optional

    if(argv[1] == NULL)
    {
        printf("Please specify source(from where to copy).\n");
        printf("Usage:  main /source/path /destination/path\n");
        exit(1);
    }
    if(argv[2] == NULL)
    {
        printf("Please specify destination.\n");
        printf("Usage:  main /source/path /destination/path\n");
        exit(1);
    }
    
    
    printf("1.Type 1 if you want to use link instead of copy or press 0 if you don't\n");
    scanf("%d",&linkCopy);
    

    copyDir(source,destination,linkCopy);
    printf("\nDone...press enter to exit");
    getchar();
}


