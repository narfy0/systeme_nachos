#ifdef CHANGED

#ifndef USERPROCESS_H
#define USERPROCESS_H

/*
    To do the necessary action to make fork and exec 
*/
extern int doForkExec(const char *filename);

/*
    To set the new process address space
*/
extern void StartUserProc(void *arg);

#endif // USERPROCESS_H

#endif // CHANGED