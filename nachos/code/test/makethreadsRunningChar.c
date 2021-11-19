#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    PutChar('£');
    ThreadExit();
}

void printGetChar()
{
    int result = GetChar();
    PutChar(result);
    ThreadExit();
}

int 
main()
{
    //ThreadCreate(printChar, 'a');
    //PutChar('&'); 
    
    //ThreadCreate(printChar, 'a');
    //GetChar();
    
    ThreadCreate(printGetChar, 'a');
    PutChar('&');
    
    //ThreadCreate(printGetChar, 'a');
    //GetChar();

    while(1){}

}

#endif //CHANGED