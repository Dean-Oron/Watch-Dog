# Watch-Dog
A program to watch and protect a running process with a consistent feedback. The process will load a shared object (DLL) and will be re-executed if wasn't respond for certain amount of time.    

The user shall dynamiclly link to the attached shared object and from than on his executable will be watched by the watchDog process.
