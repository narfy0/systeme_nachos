#ifdef CHANGED

#include "syscall.h"

int 
main()
{
    char s[6];
    
    s[0]= 't';
    s[1]= 'e';
    s[2]= 's';
    s[3]= 't';
    s[4] = '1';
    s[5]= '\0';

    PutString(s);

    // PutChar('a');
    // PutChar('b');
    // PutChar('c');
    // PutChar('d');
    // PutChar('e');
    // PutChar('f');
    // PutChar('g');

    ThreadExit();
}

#endif //CHANGED