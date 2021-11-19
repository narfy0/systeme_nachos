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
    while(1){}

}

#endif //CHANGED