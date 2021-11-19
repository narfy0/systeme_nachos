#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    volatile int i;
    for(i = 0; i < 5; i++){
        PutChar('a' + i); //we add i to 'a' value to know what is the "loop nulber" corresponding to the printed char
    }
    PutChar('|'); // to notice the end of the thread print execution
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