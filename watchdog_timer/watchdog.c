/******************************************************************************/
/* 					SP - WatchDog Timer Project		            			  */
/*					Description -  Source File of Watchdog Shared Library     */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   16.04.20									  */
/*					Reviewer - 	   Laura Bedah						          */
/*					Open Lab 86											  	  */
/******************************************************************************/

#define _XOPEN_SOURCE 700
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system */
#include <string.h>     /* memset() */
#include <pthread.h>    /* pthread_create , pthread_join */
#include <sys/types.h>  /* fork(), kill() */
#include <unistd.h>     /* fork(), exec() */
#include <signal.h>     /* sig_atomic_int, atomic_int sigaction(), kill(), SIGUSR1 SIGUSR2 SIGKILL */
#include <stdatomic.h>  /* atomic_int */
#include <semaphore.h>  /* sem_open */
#include <fcntl.h>

#include "task.h"
#include "scheduler.h"
#include "uid.h"
#include "watchdog.h"
#include "watchdog_inner.h"

/* #define DND */ 
sig_atomic_t flag1 = 0;
atomic_int kill_process1 = 0;
pid_t child_pid = -1;
char *args[3];
static sem_t *sem;
atomic_int is_child = 0;
static pthread_t sig_thread;
struct sigaction act;
static scheduler_t *global_scheduler;
extern char *__progname;

char user_program_name[20];

void __attribute__((constructor)) ThreadInit();
void __attribute__((destructor)) ThreadDestruction();

int WatchdogStart(char *program)
{
    strcpy(user_program_name, program);

    args[0] = "watchdog_process";
    args[1] = user_program_name;
    args[2] = NULL;
    child_pid = fork();

    if (0 > child_pid)
	{
		printf("ERROR\n"); 

        return (1);
	}

	else if (0 == child_pid) /* The child process is running */
	{
        setsid();
        execv("./bin/watchdog_process", args); 
	}
        
    return (0);
}

void WatchdogStop()
{
    SchedulerStop(global_scheduler);
    kill(child_pid, SIGSTOP);
}

void SignalHandler(int sig)
{
    (void)sig;
    flag1 = 1;
}

void *WatchDogThread(void *arg)
{
    size_t receive_interval = 1; 
    size_t send_interval = 1; 
    
    (void)arg;

    global_scheduler = SchedulerCreate();

    SchedulerInsertTask(global_scheduler, receive_interval, RecieveSignal, NULL);
    SchedulerInsertTask(global_scheduler, send_interval, SendSignal, NULL); 

    if (is_child)
    {
        SchedulerRun(global_scheduler);
    }

    else
    {
        while(1)
        {
            sem_wait(sem);
            SchedulerRun(global_scheduler);
        }
    }

    return (NULL);
} 

int RecieveSignal(void *param)
{
    static int counter = 0;
    (void)param;
    if (counter >= 5)
    {   
        counter = 0;

        /* if parent */
        if(!is_child)
        {
            /* Stop user scheduler and kill watchdog_process */
            WatchdogStop();
            WatchdogStart(user_program_name);
        }

        else if(is_child)
        {
            kill(getppid(), SIGSTOP);
            system(user_program_name); 
            abort();
        }
    }

    else if (0 == flag1)
    {
        ++counter;
    }

    else if (1 == flag1)
    {
        flag1 = 0;
        counter = 0;
        #ifndef DND
        if(is_child)
        {
            printf("Child recieved signal\n");
        }
        else
        {    
            printf("Parent recieved signal\n");
        }
        #endif
    }

    return (0);
}

int SendSignal(void *param)
{
    (void)param; 
    if (!is_child)
    { 
        kill(child_pid, SIGUSR1);
        #ifndef DND
        printf("parent sent signal\n");
        #endif
    }

    else if (is_child)
    {
        kill(getppid(), SIGUSR1);
        #ifndef DND
        printf("child sent signal\n");
        #endif
    }

    return (0);
}

int ReleaseSem(char *prog_name)
{   
    strcpy(user_program_name, prog_name);
    sem_post(sem);
    pthread_join(sig_thread, NULL);

    return (0);
}

void ThreadInit()
{
    char sem_name[] = "sem";

    memset(&act, 0, sizeof(act));
    act.sa_handler = &SignalHandler;
	sigaction(SIGUSR1, &act, NULL);

    if (0 == strcmp(__progname, "watchdog_process"))
    {
        is_child = 1;
    }

    sem = sem_open(sem_name, O_CREAT, 0666, 0);

    pthread_create(&sig_thread, NULL, &WatchDogThread, NULL);
}

void ThreadDestruction()
{
    sem_destroy(sem);
    SchedulerDestroy(global_scheduler);
}
