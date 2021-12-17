#ifdef CHANGED

#include "userprocess.h"
#include "machine.h"
#include "addrspace.h"
#include "system.h"
#include "synch.h"
#include "threadException.h"

int doForkExec(const char *filename){

    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    DEBUG('x', "Debug : doForkExec, executable value = %p\n", executable);
    if (executable == NULL)
      {
	  SetColor (stdout, ColorRed);
	  SetBold (stdout);
	  printf ("Unable to open file %s\n", filename);
	  ClearColor (stdout);
	  return -1;
      }

    DEBUG('x', "Debug : doForkExec, before addrspace creation  \n");
    //create a new address space
    try{
      space = new AddrSpace (executable);
      DEBUG('x', "Debug : doForkExec, after addrspace creation  \n");
    }catch(threadException e){
      delete executable;

      return -1;
    }
    
    
    
    //create a kernel thread
    Thread *kernelThread = new Thread("new_process_thread");
    DEBUG('x', "Debug : doForkExec, new kernel thread created \n");
    //pass the executable space to the kernel thread
    kernelThread->space = space;

    // close file
    delete executable;

    //increment the count of process 
    mutex_countingProcess->P();
    processCount++;
    DEBUG('x', "Incrementation of process = %d \n", processCount);
    mutex_countingProcess->V();
    

    kernelThread->Start(StartUserProc, NULL);

    return 0;
}

void StartUserProc(void *arg){
    DEBUG('x', "Debug : StartUserProc, Start the function StartUserProc with the kenerl thread : %s  \n", currentThread->getName());
    
    (void) arg;

    currentThread->space->InitRegisters();	// set the initial register values
    DEBUG('x', "Debug : StartUserProc, well initialize the register\n");
    currentThread->space->RestoreState();	// load page table register
    DEBUG('x', "Debug : StartUserProc, well load the page table register\n");
    
    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);
}

#endif // CHANGED