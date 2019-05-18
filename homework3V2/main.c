#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <string.h>

//// GLOBAL VARIABLES
pid_t child;
int seconds =0;
long counterRead=0;  // counter for characters read each second
long totalRead =0;
long stats[5]; // each pos will store the counter for that 1 second
int statsPos = -1; // the position where we will begin to store the counter
int descriptors1[2]; // one pipe for parent to child
int descriptors2[2]; // and one for child to parent

// functions handler prototypes
void sendBack(int sig);
void getStatistics(int sig);
void AlarmHandler(int sig);

int main(int argc,char *argv[])
{
    
    
    char *letter = "a";
    char buffer[1];

    pipe(descriptors1);
    pipe(descriptors2);
    
    if(child=fork())
    {
        
        close(descriptors1[0]); // close reading for pipe1
        close(descriptors2[1]); // close writing for pipe 2
        // printf("Current pid before kill is: %i\n",getpid());
        
        signal(SIGALRM,AlarmHandler);
        alarm(1);
        while(1)
        {
            write(descriptors1[1],letter,strlen(letter)+1);
            //printf("Writing a\n");
        }    
        printf("Process id  of parent is: %i\n",getpid());
        
    }
    else
    {
        close(descriptors1[1]); //close writing for pipe1
        close(descriptors2[0]); //close reading for pipe2

        signal(SIGUSR1,getStatistics); // prepare signal handler for getting the statistics
        signal(SIGUSR2,sendBack); //prepare signal for sending back the  statistics
        printf("Child pid inside child process: %i\n",getpid());
        while(read(descriptors1[0],buffer,sizeof(buffer))>0)
        {  
              
            // printf("Status: %i\n",parent);
            if(strcmp(buffer,letter))
                counterRead++;  
        }
        
        close(descriptors1[0]); // closing reading for pipe1
        close(descriptors2[0]); // closing reading for pipe2
        exit(0); 
    }
    
      
    printf("\n");
    return 0;
}