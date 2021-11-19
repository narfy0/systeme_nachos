#ifdef CHANGED

#include "syscall.h"

void printChar(char arg)
{
    PutChar(arg);
    PutChar('|');
    ThreadExit();
}

int 
main()
{
    ThreadCreate(printChar, 'a');
    ThreadCreate(printChar, 'b');
    ThreadCreate(printChar, 'c');
    ThreadCreate(printChar, 'd');
    ThreadCreate(printChar, 'e');
    ThreadCreate(printChar, 'f');
    ThreadCreate(printChar, 'g');
    ThreadCreate(printChar, 'h');
    ThreadCreate(printChar, 'i');

    PutChar('&');
    
    ThreadExit();
}

#endif //CHANGED