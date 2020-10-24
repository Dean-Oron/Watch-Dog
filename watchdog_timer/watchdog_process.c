/******************************************************************************/
/* 					SP - WatchDog Timer Project		            			  */
/*					Description -  Source file for the Watchdog executable    */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   16.04.20									  */
/*					Reviewer - 	   Laura Bedah						          */
/*					Open Lab 86											  	  */
/******************************************************************************/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>     /* system() */
#include <string.h>     /* memset() */
#include <pthread.h>
#include <sys/types.h>  /* fork(), kill() */
#include <unistd.h>     /* fork(), exec() */
#include <signal.h>     /*sig_atomic_int, atomic_int sigaction(), kill(), SIGUSR1 SIGUSR2 SIGKILL*/
#include <stdatomic.h>  /*atomic_int */

#include "task.h"
#include "scheduler.h"
#include "uid.h"
#include "watchdog.h"
#include "watchdog_inner.h"

#define DND 

void *CrashTest(void *arg);
void CrashingProcess();

int main(int argc, char **argv)
{
    #ifndef DND
    pthread_t thread1;
    #endif

    char prefix[] = "./";
    char user_program_name[10];
    (void)argc;

    strcpy(user_program_name, argv[1]);
    strcat(prefix, user_program_name);

    #ifndef DND
    pthread_create(&thread1, NULL, CrashTest, NULL);
    #endif

    ReleaseSem(prefix);

    printf("reached to the end of WD_process\n");
    return (0);
}

void *CrashTest(void *arg)
{
    int i = 7;
    (void)arg;
   
    printf("Going to crash Watchdog process on purpose in:\n");
    while(1)
    {
        sleep(2);
        printf("%d..\n", i);
        if (0 == i)
        {   
            printf("Dogwatch Process is crashed - wait 5 seconds for the User process to wake it again\n");
            CrashingProcess();
        }

        --i;
    }
}

void CrashingProcess()
{
    char *my_name = "Dean";
    char *name = "Moshe";
    
    /* Forcly crash the process */
   strcpy(my_name, name);
}

