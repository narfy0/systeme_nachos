#include "syscall.h"

main()
{
    ForkExec("../test/putcharProcess");
    ForkExec("../test/putcharProcess");
    while(1);
}