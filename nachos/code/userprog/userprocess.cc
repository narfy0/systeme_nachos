#ifdef CHANGED

#include "userprocess.h"
#include "machine.h"
#include "addrspace.h"
#include "system.h"

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
    space = new AddrSpace (executable);
    DEBUG('x', "Debug : doForkExec, after addrspace creation  \n");
    
    
    //create a kernel thread
    Thread *kernelThread = new Thread("new_process_thread");
    DEBUG('x', "Debug : doForkExec, new kernel thread created \n");
    //pass the executable space to the kernel thread
    kernelThread->space = space;

    // close file
    delete executable;

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