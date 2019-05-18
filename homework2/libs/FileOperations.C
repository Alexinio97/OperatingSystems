#pragma once
#include <stdio.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// this file includes copy file function
void copyFile(char* source,char *destination,struct stat buff)
{
    int fdDestination,fdSource;
    char buffer[1];

    fdSource = open(source,O_RDONLY);

    stat(source,&buff);
     // will use the same rights for the destination
    fdDestination = open(destination,O_CREAT | O_RDWR );
    chmod(destination,buff.st_mode);
    printf("%d\n",fdDestination);
    if(fdSource == -1)
    {
        printf("File source couldn't open.");
        exit(0);
    }

    if(fdDestination == -1)
    {
        printf("File destination couldn't open.");
        exit(0);
    }

    while(read(fdSource,buffer,1))
    {
        write(fdDestination,buffer,1);
    }
    close(fdSource);
    close(fdDestination);
}

void copyDir(char* source,char * destination)
{
    struct stat buff;   
    struct dirent* entry;
    DIR* src = opendir(source);
    DIR* dest = opendir(destination);
    char pathSrc[2048];
    char pathDest[2048];

    
    while((entry = readdir(src)) != NULL)
    {
        
        stat(source,&buff);
        if(strcmp(entry->d_name,"..") == 0 || strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,".vscode")==0)
            continue;
        
        if(entry->d_type == DT_DIR)
        {
            strcpy(pathSrc, source);
            
            //Construct new path from our base path
            
            strcat(pathSrc, "/");
            strcat(pathSrc, entry->d_name);
            printf("%s\n",pathSrc);
            //add absolute file to destination
            strcpy(pathDest,destination);
            strcat(pathDest, "/");
            strcat(pathDest,entry->d_name);
            printf("%s\n",pathDest);
            if(mkdir(pathDest,buff.st_mode)==0)
            {
                printf("Exists.");
            }
            copyDir(pathSrc,pathDest);
        }
        else
        {
            strcpy(pathSrc, source);
            
            //add absolute pathSrc to source file
            
            strcat(pathSrc, "/");
            strcat(pathSrc,entry->d_name);
            printf("%s\n", pathSrc);

            //add absolute pathSrc to destination file
            strcpy(pathDest,destination);
            strcat(pathDest, "/");
            strcat(pathDest,entry->d_name);
            printf("%s",pathDest);
            printf("\nCopy file.\n");
            copyFile(pathSrc,pathDest,buff);
        }
    
    }
    closedir(src);
    closedir(dest);
}