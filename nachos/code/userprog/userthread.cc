#ifdef CHANGED

#include "userthread.h"
#include "threads.h"
#include "machine.h"
#include "addrspace.h"

typedef struct {
    int f;
    int arg;
} Schmurtz;

static void StartUserThread(void *schmurtz){
    AddrSpace *space;
    Machine *machine;

    int i;
    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    Schmurtz tmp = *(Schmurtz *) schmurtz;
    DEBUG('x', "Debug : StartUserThread, Schurmtz.f val%d\n", tmp.f);
    machine->WriteRegister (PCReg, tmp.f);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);


    int beginAddrStack = space->AllocateUserStack();
    DEBUG('x', "Debug : StartUserThread, beginAddrStack %d\n", beginAddrStack);
}

int do_ThreadCreate(int f, int arg){
    DEBUG('x', "Debug : do_ThreadCreate, f = %d\n", f);
    Schmurtz *schmurtz;
    schmurtz->f = f;
    schmurtz->arg = arg;

    //Thread *newThread;
    //newThread->Start(StartUserThread, schmurtz);
}

void do_ThreadExit(){
    
}



#endif // CHANGED