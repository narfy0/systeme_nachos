#ifdef CHANGED

#include "syscall.h"

void print(char c[])
{
    PutString(c);
    PutChar('\n');

}

int 
main()
{
    char s[5];
    s[0]= 't';
    s[1]= 'e';
    s[2]= 's';
    s[3]= 't';
    s[4]= '\0';

    print(s);
    Halt();
}
#endif //CHANGED