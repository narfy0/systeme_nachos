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
    ThreadCreate(printChar, 'b');
    
    PutChar('&');
    
    while(1){}

    Halt();
}

#endif //CHANGED