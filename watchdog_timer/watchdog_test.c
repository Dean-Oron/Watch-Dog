/******************************************************************************/
/* 					SP - WatchDog Timer Project		            			  */
/*					Description -  Source Test File                           */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   16.04.20									  */
/*					Reviewer - 	   Laura Bedah						          */
/*					Open Lab 86											  	  */
/******************************************************************************/

#include <stdio.h>
#include <string.h>     /* memset() */
#include <sys/types.h>  /* fork(), kill() */
#include <unistd.h>     /* fork(), exec() */
#include <signal.h>     /* sig_atomic_int, atomic_int sigaction(), kill(), SIGUSR1 SIGUSR2 SIGKILL */
#include <stdatomic.h>  /* atomic_int */
#include <semaphore.h>  /* sem_init */
#include "watchdog.h"

#define SIZE (20)

void StartStopTest();
void CrashingProcess();
void UserCrashingTest();
void WatchdogCrashingTest();

int main()
{
      StartStopTest(); 
     /* UserCrashingTest(); */ 

    /* Make sure to remove DND from watchdog_process file for using this function!*/
    /* WatchdogCrashingTest(); */
    return(0);
}

void StartStopTest()
{
    char *str = "watchdog_test";
    int counter = 0;

    printf("Entered to User Process\n\n");
    WatchdogStart(str);

    while(counter < SIZE)
    {
        sleep(2);
        printf("main process keep working\n");

        if (counter == 7)
        {
            WatchdogStop();
            printf("Now you don't have a watchdog to keep ur ass safe!!!\n");
        } 

        if (counter == 14)
        {
            WatchdogStart(str);
            printf("now you have watchdog again\n");
        }

        printf("main counter value is %d!!!!!!!\n", counter);
        ++counter;
    }

    WatchdogStop();
    printf("user process has finished\n");
}

void UserCrashingTest()
{
    int i = 7;
    printf("Entered to User Process\n\n");
    WatchdogStart("watchdog_test");
    printf("Going to crash user process on purpose in:\n");
    while(1)
    {
        sleep(1);
        printf("%d..\n", i);
        if (0 == i)
        {   
            printf("User Process is crashed - wait 5 seconds for the Watchdog wake it up again\n");
            printf("To end program from here, Kill the processes manually");
            CrashingProcess();
        }

        --i;
    }
}

void WatchdogCrashingTest()
{
    int counter = 0;
    WatchdogStart("watchdog_test");
    
    while(counter < SIZE)
    {
        sleep(2);
        printf("main process keep working\n");
        ++counter;
    }
}

void CrashingProcess()
{
    char *my_name = "Dean";
    char *name = "Moshe";
    
    /* Forcly crash the process */
   strcpy(my_name, name);
}