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
    while(1){}

}

#endif //CHANGED