#ifdef CHANGED

#include "syscall.h"

/**
 * Simple Putchar usage example
 * 
 * This method is suppose to print the character in the console 
 * and the n next character after the first according to ASCII table
 */
void print(char c, int n)
{
    int i;

    for(i = 0; i < n; i++){
        PutChar(c + i);
    }
    PutChar('\n');

}

/**
 * To test a normal usage of PutChar
 */
void normal_test()
{
    PutChar('a');
}

/**
 * To test if we try to put an empty file
 * 
 * This test is irrelevant because the empty char causes a compile error.
 * But we still kept it to understand that we think about this case
 */
void empty_test()
{
    //PutChar('');
}

/**
 * To test if we want to put a \n char
 */
void breakLine_test()
{
    PutChar('\n');
}

/**
 * To test if we want to put a EOF char
 * 
 * This test is irrelevant because the EOF char causes a compile error.
 * But include the stdio.h file don't work because compilator don't find it
 * So we still kept it to understand that we think about this case.
 */
void endOfFile_test()
{
    //PutChar(EOF);
}


/**
 * Main method, which call tests
 */
int main()
{
    //print('a', 4);
    normal_test();
    //empty_test();
    //breakLine_test();
    //endOfLine_test();

    //Halt();
}

#endif //CHANGED