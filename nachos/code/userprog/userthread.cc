#ifdef CHANGED

#include "userthread.h"

#include "machine.h"
#include "addrspace.h"

typedef struct {
    int f;
    int arg;
} Schmurtz;

static void StartUserThread(void *schmurtz){
    AddrSpace *space;
    Machine *machine;

    int beginAddrStack = space->AllocateUserStack();
    DEBUG('x', "Debug : StartUserThread, beginAddrStack %d\n", beginAddrStack);
    
    int i;
    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, beginAddrStack);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

   
}

int do_ThreadCreate(int f, int arg){
    Schmurtz *schmurtz;
    schmurtz->f = f;
    schmurtz->arg = arg;

    //...

    StartUserThread(schmurtz); 
}

void do_ThreadExit(){
    
}



#endif // CHANGED