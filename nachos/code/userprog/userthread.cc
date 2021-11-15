#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "machine.h"
#include "addrspace.h"
#include "system.h"
#include "synch.h"

typedef struct {
    int f;
    int arg;
} Schmurtz;

//variable to count user thread
int threadCount = 0;
//Semaphore mutex_countingThread(1);

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

    mutex_countingThread
    threadCount++;
    
    
    newThread->space = currentThread->space; //here, before newThread->Start, currentThread is the parent thread
    newThread->Start(StartUserThread, schmurtz);
    //do_ThreadExit();
    
    return 0;
}

void do_ThreadExit(){
    DEBUG('x', "Debug : do_ThreadExit begin (threadCount = %d)\n", threadCount);
    currentThread->Finish();

    //if I am the last, I stop nachos process
    if(threadCount == 0){
        DEBUG('x', "Debug : do_threadExit powerdown (count = %d)", threadCount);
        //interrupt->Halt();
        interrupt->Powerdown ();
    }

    threadCount--;
    
}



#endif // CHANGED