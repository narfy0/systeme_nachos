// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "consoledriver.h"
#include "userthread.h"
#include "userprocess.h" 
#include "synch.h"

/**
 * Copy a string, char by char, from MIPS world (using "from" address in args)
 * to the kernel world (using "to" address in args). Use ReadMem() method.
 * Stop when copy '\0' which have to be here in order to ensure system's security
 * 
 * Returns number of copied characters
 **/
int copyStringFromMachine(int from, char *to, unsigned size)
{
    int* value;

    //loop and check '\0' OR size
    int i = 0;
    int tmp;

	while(i < size && tmp != '\0'){
		//read string from Nachos memory, and put it into 'to'
        machine->ReadMem(from + i, 1, &tmp);
        to[i] = tmp; //sure that it's in [0; 255] so it's a char

		i++;
    }
    
    // if dont have '\0', add it (rewrite the last char)
    if(tmp != '\0'){
        to[i-1] = '\0'; 
    }
 
    //return postion of '\0' <=> number of char read or write '\0' include
    return i;
}

void copyStringToMachine(char* from, int to, unsigned size){
    int i = 0;
    for(i = 0; i < size && from[i] != '\0'; i++){
       machine->WriteMem(to + i, 1, from[i]);
    }

	//force \0
    machine->WriteMem(to + i, 1, '\0');
    
}

#endif //CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Powerdown ();
		    break;
		  }

		#ifdef CHANGED
		
		case SC_PutChar:
		  {
			//to print debug message when this exception is called
		    DEBUG ('s', "PutChar\n");
			
			//get the first arg from register 4
		    char arg = machine->ReadRegister(4);
			//call putchar() from ConsoleDriver
			consoledriver->PutChar(arg); 

		    break;
		  }
		
		//case to add PutString syscall

		case SC_PutString:
		{
			//to print debug message when this exception is called
			DEBUG('s', "PutString\n");

			//get the arg from register 4
			int argAddr = machine->ReadRegister(4);

			//kernel address where is the string
			char stringAddr[MAX_STRING_SIZE];

			//number of written char
			int writtenChar = MAX_STRING_SIZE;

			//copy a String from MIPS memory to a kernel pointer
			int returnCopy = copyStringFromMachine(argAddr, stringAddr, writtenChar);
			if(returnCopy > writtenChar){
				printf("Error : overflow memory while reading");
		    	ASSERT(FALSE);	
			}

			//call putstring() from consoleDriver
			consoledriver->PutString(stringAddr);

			break;
		}

		case SC_GetChar:
		{
			//to print debug message when this exception is called
			DEBUG('s', "GetChar\n");
			
			//get a char from the stream
			int returnedChar = consoledriver->GetChar();

			//write read char to register 2
			machine->WriteRegister (2, returnedChar);
			
			break;
		}

		case SC_GetString:
		{
			//to print debug message when this exception is called
			DEBUG('s', "GetString\n");

			//get the arg from register 4
			int argAddr = machine->ReadRegister(4);

			//kernel address where is the string
			char stringAddr[MAX_STRING_SIZE];
			consoledriver->GetString(stringAddr, MAX_STRING_SIZE);

			//number of written char
			int writtenChar = MAX_STRING_SIZE;

			//copy a String from MIPS memory to a kernel pointer
			copyStringToMachine(stringAddr, argAddr, writtenChar);
			
			break;
		}

		case SC_ThreadCreate:
		{
			//to print debug message when this exception is called
			DEBUG('s', "ThreadCreate\n");

			//to get the arg from syscall ThreadCreate()
			int f = machine->ReadRegister(4);
			int arg = machine->ReadRegister(5);

			do_ThreadCreate(f, arg);

			break;
		}

		case SC_ThreadExit:
		{
			//to print debug message when this exception is called
			DEBUG('s', "ThreadExit\n");

			do_ThreadExit();

			break;
		}
		
		case SC_Exit:
		{
			DEBUG ('s', "SC_Exit : Shutdown, initiated by user program.\n");

			/* Old version of our code, deprecated now because we choose to use ThreadExit()

			//decrement process counter
			mutex_countingProcess->P();
			processCount--;
			DEBUG('x', "Decrementation of process = %d \n", processCount);
			mutex_countingProcess->V();

			//free the process ressources
			delete currentThread->space;
			currentThread->space = NULL;

			//check if it's the last process to exit
			if(processCount == 0){
				interrupt->Powerdown ();
			}

			currentThread->Finish();
			*/
			//interrupt->Powerdown ();

		    break;
		}

		case SC_ForkExec:
		{
			DEBUG ('s', "SC_ForkExec : .\n");
		    
			int name_file = machine->ReadRegister(4);
			
			char stringAddr[MAX_STRING_SIZE];

			//number of written char
			int writtenChar = MAX_STRING_SIZE;

			//copy a String from MIPS memory to a kernel pointer
			int returnCopy = copyStringFromMachine(name_file, stringAddr, writtenChar);
			if(returnCopy > writtenChar){
				printf("Error : overflow memory while reading");
		    	ASSERT(FALSE);	
			}

			int x = doForkExec(stringAddr);

			machine->WriteRegister (2, x);

		    break;
		}	

		#endif // CHANGED

		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!address) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }
	  break;

	case ReadOnlyException:
	  printf ("Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case BusErrorException:
	  printf ("Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case AddressErrorException:
	  printf ("Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case OverflowException:
	  printf ("Overflow at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case IllegalInstrException:
	  printf ("Illegal instruction at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	default:
	  printf ("Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
	  ASSERT (FALSE);
	  break;
      }
}
