#ifdef CHANGED

#include "syscall.h"

void print(char c[])
{
    PutString(c);
    PutChar('\n');
}

void printNormalString(){
    //test on a regular buffer
    char s[5];
    
    s[0]= 't';
    s[1]= 'e';
    s[2]= 's';
    s[3]= 't';
    s[4]= '\0';

    print(s);
}

void printEmptyString(){
    // test on a empty buffer
    char s_1[5];
    print(s_1);
}

void printUnfinishedString(){
    //test on a buffer without a \0
    char s[7];

    s[0]= 'U';
    s[1]= 'N';
    s[2]= 'S';
    s[3]= 't';
    s[4]= 'e';
    s[5]= 's';
    s[5]= 't';

    print(s);
}

void printEndForcedString(){
    //test on a buffer cut with a \0
    char s[8];

    s[0]= 'e';
    s[1]= 'n';
    s[2]= 'd';
    s[3]= '\0';
    s[4]= 'x';    
    s[5]= 'x';    
    s[6]= 'x';    
    s[7]= 'x';    
 
    print(s);
}

int 
main()
{
    printNormalString();
    printEmptyString();
    printUnfinishedString();
    printEndForcedString();
       
    Halt();
}
#endif //CHANGED