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
    
    //test on a buffer without a \0
    /*char s[5];
    s[0]= 't';
    s[1]= 'e';
    s[2]= 's';
    s[3]= 't';
    s[4]= 's';
    print(s)*/

    //test on a empty buffer
    /*char s[5];
    print(s);*/

    //test on a buffer full of \0
    /*char s[5];
    s[0]= '\0';
    s[1]= '\0';
    s[2]= '\0';
    s[3]= '\0';
    s[4]= '\0';
    print(s)*/
    Halt();
}
#endif //CHANGED