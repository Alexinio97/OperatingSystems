#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <string.h>

//// GLOBAL VARIABLES
extern pid_t child;
extern int seconds;
extern long counterRead;  // counter for characters read each second
extern long totalRead;
extern long stats[5]; // each pos will store the counter for that 1 second
extern int statsPos; // the position where we will begin to store the counter
extern int descriptors1[2]; // one pipe for parent to child
extern int descriptors2[2]; // and one for child to parent
//

void sendBack(int sig)
{
    if(sig == SIGUSR2)
    {
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
        printf("Received sigusr1");
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
        
        while(read(descriptors2[0],&stats[counter],sizeof(long)))
        {
            printf("Numbers of characters at second %i is %li\n",counter,stats[counter]);
            totalCheck+= stats[counter];
            counter++;
        }
        printf("Total number to see if it's corrent is: %li",totalCheck);
        close(descriptors2[0]);
        exit(0);
    }
    else
    {
        kill(child,SIGUSR1); 
        seconds++;
        alarm(1);   
    }
}