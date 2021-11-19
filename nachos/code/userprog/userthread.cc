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

/*
 To Initiate user thread's registers, stack and run the thread
*/
static void StartUserThread(void *schmurtz){
    DEBUG ('x', "StartUserThread begin\n");

    int i;
    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    Schmurtz tmp = *(Schmurtz *) schmurtz; // schmurtz contains the wanted function's address and arguments
    free(schmurtz);
    DEBUG('x', "Debug : StartUserThread, Schurmtz.f val = %d\n", tmp.f);
    machine->WriteRegister (PCReg, tmp.f);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // To give the argument of the program
    machine->WriteRegister(4, tmp.arg);

    //To allocate the user stack in the address space
    int beginAddrStack = currentThread->space->AllocateUserStack();
    DEBUG('x', "Debug : StartUserThread, beginAddrStack %d\n", beginAddrStack);

    // To run the wanted function by the thread 
    machine->Run();
}

/*
 To create a user thread
*/
int do_ThreadCreate(int f, int arg){
    DEBUG('x', "Debug : do_ThreadCreate, f = %d\n", f);
    
    Thread *newThread;
    Schmurtz *schmurtz;
    
    schmurtz = (Schmurtz *) malloc(sizeof(schmurtz));
    schmurtz->f = f;
    schmurtz->arg = arg;

    newThread = new Thread ("userThread");  
    
    // To initialise user thread's address space as the same than parent thread
    newThread->space = currentThread->space; //here, before newThread->Start, currentThread is the parent thread
    newThread->Start(StartUserThread, schmurtz);
    
    return 0;
}

void do_ThreadExit(){
    DEBUG('x', "Debug : do_ThreadExit begin \n");
    currentThread->space->FinishUserThreads();
    currentThread->Finish();
    
}

#endif // CHANGED