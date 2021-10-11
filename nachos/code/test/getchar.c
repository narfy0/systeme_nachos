#ifdef CHANGED

#include "syscall.h"

void testGet()
{
    int res = GetChar();

    PutChar(res);

}

int 
main()
{
    testGet();
    Halt();
}

#endif //CHANGED