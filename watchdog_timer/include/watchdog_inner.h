/******************************************************************************/
/* 					SP - WatchDog Timer Project		            			  */
/*					Description -  Inner API                                   */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   16.04.20									  */
/*					Reviewer - 	   Laura Bedah						          */
/*					Open Lab 86											  	  */
/******************************************************************************/

/* A function to handle with signal that recieve from different process */
void SignalHandler(int sig);

/* A function to build a signaling scheduler to create inter process communication */
void *WatchDogTread(void *arg);

/* A function to send signal to another process */
int SendSignal(void *param);

/* A function to ask if signal was not send after a certain time */
int RecieveSignal(void *param);

/* A function to release the global semaphore to release the waiting thread */
int ReleaseSem(char *user_program_name);

/* A constructor function to create the constructor thread */
void ThreadInit();

/* A destructor function to destruct the thread and scheduler at the end of a process */
void ThreadDestruction();

/* A function to free the scheduler and semaphore from memory */
void FreeMem();