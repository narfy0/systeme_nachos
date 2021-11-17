#ifdef CHANGED

#include "syscall.h"

void printChar()
{
    char s[5];
    
    s[0]= 't';
    s[1]= 'e';
    s[2]= 's';
    s[3]= 't';
    s[4]= '\0';

    PutString(s);
    ThreadExit();
}

int 
main()
{
    char s1[5];
    
    s1[0]= 'm';
    s1[1]= 'a';
    s1[2]= 'i';
    s1[3]= 'n';
    s1[4]= '\0';

    ThreadCreate(printChar, 'a');
    //ThreadCreate(printChar, 'b');
    
    //PutChar('&');
   

    PutString(s1);
    ThreadExit();
    
    //while(1){}

}

#endif //CHANGED