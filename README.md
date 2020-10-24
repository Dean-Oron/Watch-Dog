# Watch-Dog
A program to watch and protect a running process with a consistent feedback. The process will load a shared object (DLL) and will be re-executed if wasn't respond for certain amount of time.    

The user shall dynamiclly link to the attached shared object and execute the watchDog executable and from than on the user process will be consiistently watched and will be re-executed if wasn't respond more than a certain amount of time. 
