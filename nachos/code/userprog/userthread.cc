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
    
    DEBUG('x', "Debug : StartUserThread, Schurmtz.f val = %d\n", tmp.f);
    machine->WriteRegister (PCReg, tmp.f);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // To give the argument of the program
    machine->WriteRegister(4, tmp.arg);

    //To allocate the user stack in the address space
    int beginAddrStack = currentThread->space->AllocateUserStack();
    machine->WriteRegister (StackReg, beginAddrStack);
    DEBUG('x', "Debug : StartUserThread, beginAddrStack %d\n", beginAddrStack);
    
    free(schmurtz);
    
    machine->Run();
}

/*
 To create a user thread
*/
int do_ThreadCreate(int f, int arg){
    
    if(currentThread->space->isFull() == 1){
        DEBUG('x', "Debug : do_ThreadCreateend because stack is full : %d\n", arg  -'a');
        return -1;
    }

    DEBUG('x', "Debug : do_ThreadCreate, f = %d\n", f);

    Thread *newThread;
    Schmurtz *schmurtz;
    
    schmurtz = (Schmurtz *) malloc(sizeof(schmurtz));
    schmurtz->f = f;
    schmurtz->arg = arg;

    newThread = new Thread ("userThread");  
    
    // To initialise user thread's address space as the same than parent thread
    newThread->space = currentThread->space; //here, before newThread->Start, currentThread is the parent thread
    newThread->setIndex(newThread->space->FindIndexStack());
    newThread->Start(StartUserThread, schmurtz);
    
    return 0;
}

void do_ThreadExit(){
    int threadCount = currentThread->space->GetThreadCount();

    DEBUG('x', "Debug : do_ThreadExit begin / processCount = %d, currentThread = %d, threadCount = %d\n",
         processCount, currentThread, threadCount);

    //if I am the last thread
    if(threadCount == 1){

        //check if it's the last process to exit
        //DEBUG('x', "DEBUG : before check if it is the last process to ending all / processCount = %d  by currentThread = %d\n", processCount, currentThread);
        
        //decrement process counter
        mutex_countingProcess->P();
        processCount--;
        DEBUG('x', "Decrementation of process = %d / currentThread = %d\n", processCount, currentThread);
        mutex_countingProcess->V();

        if(processCount == 0){ //if I'am the last process
            interrupt->Powerdown ();

        } else { // if I'm the last thread of my process but not the last process
            //DEBUG('x', "DEBUG : Finnish last User Thread, before curretnThread->Finnish() / currentThread = %d\n", currentThread);     

            delete currentThread->space;
            currentThread->space = NULL;
            
            currentThread->Finish();
        }

    } else {
       currentThread->space->FinishUserThreads();
       currentThread->Finish();
    }
}

#endif // CHANGED