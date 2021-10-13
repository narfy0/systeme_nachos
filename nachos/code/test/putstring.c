#ifdef CHANGED

#include "syscall.h"

/**
 * Simple PutString usage example
 */
void print(char c[])
{
    PutString(c);
    PutChar('\n');
}

/**
 * To test a normal usage of PutString, with a regular buffer
 */
void printNormalString(){
    char s[6];
    
    s[0]= 't';
    s[1]= 'e';
    s[2]= 's';
    s[3]= 't';
    s[4] = '1';
    s[5]= '\0';

    PutString(s);
    PutChar('\n');
}

/**
 * To test when the string is not initialised, an empty buffer
 */
void printEmptyString(){
    char s_1[5];
    PutString(s_1);
    PutChar('\n');
}

/**
 * To test when the string is not finished at the last char
 * when the buffer doesn't contain a \0
 */
void printUnfinishedString(){
    char s[7];

    s[0]= 'U';
    s[1]= 'N';
    s[2]= 'S';
    s[3]= 't';
    s[4]= 'e';
    s[5]= 's';
    s[5]= 't';

    PutString(s);
    PutChar('\n');
}

/**
 * To test when the buffer contain a \0 until the end
 * expectation : print the text with a \0 at the end
 */
void printEndForcedString(){
    char s[8];

    s[0]= 'e';
    s[1]= 'n';
    s[2]= 'd';
    s[3]= '\0';
    s[4]= 'x';    
    s[5]= 'x';    
    s[6]= 'x';    
    s[7]= 'x';    
 
    PutString(s);
    PutChar('\n');
}

/**
 * Test if the string is only \0 
 * expectation : print nothing and return 1 (sure?)
 */
void printOnlyNullChar(){
    char s[1];

    s[0]= '\0';
    PutString(s);
    PutChar('\n');
}

/**
 * test if the strinf is too long to test memory overflow
 * expect : ...
 */
void printTooLongString(){
    char s[11]; //to test it, we set MAX_STRING_SIZE to 10

    s[0]= 't';
    s[1]= 'h';
    s[2]= 'i';
    s[3]= 's';
    s[4]= 'i';    
    s[5]= 's';    
    s[6]= 'a';    
    s[7]= 't';
    s[8]= 'e';
    s[9]= 's';
    s[10]= 't';     
 
    PutString(s);
    PutChar('\n');
}

/**
 * test if the string contains space " " | ...
 */
void printStringWithSpace(){
    char s[8];

    s[0]= 's';
    s[1]= 'p';
    s[2]= 'a';
    s[3]= 'c';
    s[4]= 'e';    
    s[5]= ' ';    
    s[6]= 'o';    
    s[7]= 'k';    
 
    PutString(s);
    PutChar('\n');
}

/**
 * test if the string contains \n | ...
 */
  void printBreakLine(){
    char s[8];

    s[0]= 'b';
    s[1]= 'r';
    s[2]= 'e';
    s[3]= 'a';
    s[4]= 'k';    
    s[5]= '\n';    
    s[6]= 'o';    
    s[7]= 'k';    
 
    PutString(s);
    PutChar('\n');
}

/**
 * test if the string contains EOF | ...
 */
  void printEndOfLine(){
    /*
    char s[8];

    s[0]= 'f';
    s[1]= 'i';
    s[2]= 'l';
    s[3]= 'e';
    s[4]= EOF;    
    s[5]= 'x';    
    s[6]= 'x';    
    s[7]= 'x';    
 
    PutString(s);
    PutChar('\n');
    */ //TODO fix it
  }

int main()
{
    //printNormalString();
    //printEmptyString();
    //printUnfinishedString();
    //printEndForcedString();
    //printOnlyNullChar();
    //printTooLongString();
    printStringWithSpace();
    //printBreakLine();
    //printEndOfLine();
       
    Halt();
}
#endif //CHANGED