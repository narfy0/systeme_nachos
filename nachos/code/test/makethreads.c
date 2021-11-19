#ifdef CHANGED

#include "syscall.h"

void printChar(char arg)
{
    PutChar(arg);
    ThreadExit();
}

int 
main()
{
    ThreadCreate(printChar, '£');
    while(1){}

}

#endif //CHANGED