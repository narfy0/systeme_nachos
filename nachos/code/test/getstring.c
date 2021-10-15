#ifdef CHANGED

#include "syscall.h"

/**
 * Tests have to be done by user in the console
 *
 * - 1st test : enter a simple char like 'a' to test the simpliest way to use GetChar()
 *              exceptation : print the char
 * - 2nd test : enter without enter a char
 *              exceptation : nothing is print
 * - 3rd test : enter many char like a string
 *              exceptation : show only the first char
 * - 4th test : enter the EOF char using CRTL-D
 *              exceptation : print EOF char because it's recognize as end of a file
 * 
 */

void testGet()
{
    char* s;
    int size = 10;
    GetString(s, size);
    PutString(s);

}

int 
main()
{
    testGet();
    Halt();
}

#endif //CHANGED