#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    PutChar('£');
}

int 
main()
{
    ThreadCreate(printChar, 'a');

    while(1){
        
    }

    Halt();
}

#endif //CHANGED