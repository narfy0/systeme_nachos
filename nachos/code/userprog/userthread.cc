#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "machine.h"
#include "addrspace.h"
#include "system.h"

typedef struct {
    int f;
    int arg;
} Schmurtz;



static void StartUserThread(void *schmurtz){
    DEBUG ('x', "StartUserThread begin\n");

    int i;
    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    Schmurtz tmp = *(Schmurtz *) schmurtz;
    free(schmurtz);
    DEBUG('x', "Debug : StartUserThread, Schurmtz.f val = %d\n", tmp.f);
    machine->WriteRegister (PCReg, tmp.f);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    int beginAddrStack = currentThread->space->AllocateUserStack();
    DEBUG('x', "Debug : StartUserThread, beginAddrStack %x\n", beginAddrStack);

    machine->Run();
}

int do_ThreadCreate(int f, int arg){
    DEBUG('x', "Debug : do_ThreadCreate, f = %d\n", f);
    
    Thread *newThread;
    Schmurtz *schmurtz;
    
    schmurtz = (Schmurtz *) malloc(sizeof(schmurtz));
    schmurtz->f = f;
    schmurtz->arg = arg;

    newThread = new Thread ("userThread");
    newThread->space = currentThread->space; //here, before newThread->Start, currentThread is the parent thread
    newThread->Start(StartUserThread, schmurtz);
    do_ThreadExit();
    
    return 0;
}

void do_ThreadExit(){
    DEBUG('x', "Debug : do_ThreadExit begin \n");
    currentThread->Finish();
}



#endif // CHANGED