#include "syscall.h"

void testActionUserPages0(){
    ForkExec("../test/putcharProcess");
    ForkExec("../test/putcharProcess");
}

void testActionUserPages1(){
    ForkExec("../test/makethreadsFinnish");
    ForkExec("../test/makethreadsFinnish");
}

main()
{
    //test action II.2
    //testActionUserPages0();

    //test action II.3
    testActionUserPages1();

    //from action II.1
    //while(1);
}