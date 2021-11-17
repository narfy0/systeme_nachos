#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    PutChar('a');
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

    PutChar('&');
    
    ThreadExit();
    //while(1){}

}

#endif //CHANGED