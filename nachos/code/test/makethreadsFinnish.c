#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    PutChar('£');
    ThreadExit();
}

int 
main()
{
    ThreadCreate(printChar, 'a');

    PutChar('&');
    ThreadExit();
}

#endif //CHANGED