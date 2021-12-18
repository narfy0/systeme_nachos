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
    //if the executable file doesn't exist, print an error message
    if (executable == NULL) {
      SetColor (stdout, ColorRed);
      SetBold (stdout);
      printf ("Unable to open file %s\n", filename);
      ClearColor (stdout);
	    return -1;
    }


    //create a new address space
    //DEBUG('x', "Debug : doForkExec, before addrspace creation  \n");

    // to restore later the currentThread index because it is initialize to 0 in "new AddrSpace"
    int savedIndex = currentThread->getIndex(); 

    try{ //create a new AddrSpace could throw a threadException if create a thread is not possible
      space = new AddrSpace (executable);
      currentThread->setIndex(savedIndex); // to restore currentThread index in stack
      //DEBUG('x', "Debug : doForkExec, after addrspace creation  \n");

    }catch(threadException e){
      SetColor (stdout, ColorRed);
      SetBold (stdout);
      printf ("Error when want to create a new address space\n");
      ClearColor (stdout);

      delete executable;
      currentThread->setIndex(savedIndex); // to restore currentThread index in stack

      return -1;
    }
    
    
    //increment the count of process 
    mutex_countingProcess->P();
    processCount++;
    DEBUG('x', "Incrementation of process = %d \n", processCount);
    mutex_countingProcess->V();

    //create a kernel thread
    Thread *kernelThread = new Thread("new_process_thread");
    DEBUG('x', "Debug : doForkExec, new kernel thread created \n");
    //pass the executable space to the kernel thread
    kernelThread->space = space;
    //kernelThread->setIndex(kernelThread->space->FindIndexStack());
    kernelThread->setIndex(0);

    // close file
    delete executable;

    //Start the main thread of this process with the function which continue to set this process
    kernelThread->Start(StartUserProc, (void*) 1);

    //unreachable state 
    return 1;
}

void StartUserProc(void *arg){
  DEBUG('x', "Debug : StartUserProc, Start the function StartUserProc with the kenerl thread : %s  \n", currentThread->getName());
  (void) arg;

  // set the initial register values
  currentThread->space->InitRegisters();	
  //DEBUG('x', "Debug : StartUserProc, well initialize the register\n");

  // load page table register
  currentThread->space->RestoreState();	
  //DEBUG('x', "Debug : StartUserProc, well load the page table register\n");
  
  // jump to the user progam
  machine->Run ();		
  ASSERT (FALSE); //unreachable state
}

#endif // CHANGED