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
//
void sendBack(int sig)
{
    if(sig == SIGUSR2)
    {
        printf("Received SIGUSR2 \n");
        printf("Sending back the statistics\n");
        write(descriptors2[1],&totalRead,sizeof(long)); // sending back the total number of characters
        for(int i = 0; i < 5;i++)
        {
            write(descriptors2[1],&stats[i],sizeof(long)); // send total read for each second
        }
        close(descriptors2[1]); // closing writing for this pipe
        exit(0);
    }
}

void getStatistics(int sig)
{
    if(sig == SIGUSR1)
    {
        printf("Received sigusr1\n");
        statsPos++;
        stats[statsPos] = counterRead;
        totalRead+=counterRead;
        counterRead = 0;
    }
}

void AlarmHandler(int sig)
{   
    int counter =0;
    long totalCheck=0;
    if(seconds == 5)
    {
        kill(child,SIGUSR2); // sending signal for sending back the statistics

        printf("Process son terminated reading\n");
        printf("Parent process will begin to receive all the character.\n");
        read(descriptors2[0],&totalRead,sizeof(long));
        printf("Number of total character read is: %li\n",totalRead);
        
        while(read(descriptors2[0],&stats[counter],sizeof(long)))  // read each element inside stats array
        {
            printf("Numbers of characters at second %i is %li\n",counter,stats[counter]);
            totalCheck+= stats[counter];
            counter++;
        }
        printf("Total number to see if it's corrent is: %li\n",totalCheck);
        close(descriptors2[0]); // close pipe1 reading part
        exit(0);
    }
    else
    {
        kill(child,SIGUSR1); // send SIGUSR1 signal to child process
        seconds++;
        alarm(1);   // AlarmHandler will be called again after 1 second
    }
}




int main(int argc,char *argv[])
{
    
    
    char *letter = "a";
    char buffer[1];

    pipe(descriptors1); // instantiate pipe 1 from parent to child
    pipe(descriptors2); // instantiate pipe 2 from child to parent
    
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