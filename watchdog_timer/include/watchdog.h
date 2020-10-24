/******************************************************************************/
/* 					SP - WatchDog Timer Project		            			  */
/*					Description -  User API                                   */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   16.04.20									  */
/*					Reviewer - 	   Laura Bedah						          */
/*					Open Lab 86											  	  */
/******************************************************************************/

/* 
 * A function to start the WatchDog process to start watching 
 * Arguments: A pointer to the user program name including the relative path 
 * Return Value: Return zero for success             
 */ 
int WatchdogStart(char *program);
 
/* A function to stop the WatchDog process to stop watching the user process */
void WatchdogStop();
