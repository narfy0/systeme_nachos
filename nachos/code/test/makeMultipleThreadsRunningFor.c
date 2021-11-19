#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    volatile int i;
    for(i = 0; i < 5; i++){
        PutChar('a' + i);
    }
    PutChar('|');
    ThreadExit();
}

int 
main()
{
    ThreadCreate(printChar, 'a');
    ThreadCreate(printChar, 'b');
    ThreadCreate(printChar, 'c');

    PutChar('&');
    ThreadExit();
}

#endif //CHANGED