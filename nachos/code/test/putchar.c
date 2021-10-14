#ifdef CHANGED

#include "syscall.h"

/**
 * Simple Putchar usage example
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
 */
void empty_test()
{
    //PutChar('');
    //TODO think about it (dont compile because it's null)
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
 */
void endOfFile_test()
{
    //PutChar(EOF);
    //TODO think about it
}


/**
 * Main method, which call tests
 */
int main()
{
    //int n = 101;
    //print('a', 4);
    normal_test();
    //empty_test();
    breakLine_test();
    //endOfLine_test();

    //Halt();
    //return n;
}

#endif //CHANGED