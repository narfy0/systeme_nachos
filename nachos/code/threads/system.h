// system.h 
//      All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef CHANGED

#include "../userprog/consoledriver.h"
#include "../userprog/pageprovider.h"
// We set the include path like that beacause it would causes errors
//#include "consoledriver.h"
//#include "pageprovider.h"

#endif // CHANGED

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"



// Initialization and cleanup routines
extern void Initialize (int argc, char **argv);	// Initialization,
						// called before anything else
extern void Cleanup ();		// Cleanup, called when
						// Nachos is done.

extern Thread *currentThread;	// the thread holding the CPU
extern Thread *threadToBeDestroyed;	// the thread that just finished
extern Scheduler *scheduler;	// the ready list
extern Interrupt *interrupt;	// interrupt status
extern Statistics *stats;	// performance metrics
extern Timer *timer;		// the hardware alarm clock

#ifdef USER_PROGRAM
#include "machine.h"
extern Machine *machine;	// user program memory and registers
#endif

#ifdef FILESYS_NEEDED		// FILESYS or FILESYS_STUB
#include "filesys.h"
extern FileSystem *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"
extern PostOffice *postOffice;
#endif

#ifdef CHANGED

#ifdef USER_PROGRAM
extern ConsoleDriver *consoledriver;
extern PageProvider *pageProvider;

extern Semaphore *mutex_countingProcess;
//TODO see if we let the variable in extern
extern int processCount;
#endif // USER_PROGRAM

//constant for string buffer max size (action V.3)
const int MAX_STRING_SIZE = 30; 

#endif // CHANGED

#endif // SYSTEM_H
