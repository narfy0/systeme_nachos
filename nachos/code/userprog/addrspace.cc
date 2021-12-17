// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "syscall.h"
#include "new"

#ifdef CHANGED
#include "synch.h"
#include "bitmap.h"

#include "threadException.h"
#endif //CHANGED

#ifdef CHANGED

// variable to count user thread
int threadCount = 0;
// mutex to controll the incrementation and decrementation of running threads
static Semaphore *mutex_countingThread;
//mutex to controll the reservation of memory page when use PageProvider
static Semaphore *mutex_reserved_page  = new Semaphore("page reservation mutex", 1);

//FROM TD3
/*
 Read numBytes octets from postion in executable
 And write in virtual addr space define by page table "pageTable" size of "numPages"

 Declared here but implemants at the end of the file in order to make it cleaner for us
*/
void ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes, int position, TranslationEntry *pageTable, unsigned numPages);
#endif //CHANGED


//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------
static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->uninitData.virtualAddr = WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpaceList
//      List of all address spaces, for debugging
//----------------------------------------------------------------------
List AddrSpaceList;

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    unsigned int i, size;

    executable->ReadAt (&noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    /* Check that this is really a MIPS program */
    ASSERT (noffH.noffMagic == NOFFMAGIC);

    // how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStacksAreaSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    // check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory
    /*
    if (numPages > NumPhysPages)
	    throw std::bad_alloc();
    */
   //check if enough page 
   if (numPages > pageProvider->NumAvailPage())
	    throw std::bad_alloc(); //TODO fix this case

    DEBUG ('a', "Initializing address space, num pages %d, total size 0x%x\n",
	   numPages, size);
    // first, set up the translation 
    pageTable = new TranslationEntry[numPages];

    mutex_reserved_page->P();
    pageProvider->ReservedPage(numPages);
    mutex_reserved_page->V();
    
    for (i = 0; i < numPages; i++)
    {
        #ifdef CHANGED
	  //Action I.4
        //pageTable[i].physicalPage = i + 1;	// for now, phys page # = virtual page #

      //Action I.6
        DEBUG('x', "AddrSpace initialization : before pageprovider.getEmptyPage : loop turn = %d\n", i);

        mutex_reserved_page->P();
        int emptyPageIndex = pageProvider->GetEmptyPage();
        DEBUG('x', "AddrSpace initialization : pageprovider.getEmptyPage returns : %d\n", emptyPageIndex);
        pageTable[i].physicalPage = emptyPageIndex;	
        mutex_reserved_page->V();

        DEBUG('x', "AddrSpace initialization : after pageprovider.getEmptyPage : loop turn = %d\n", i);
        #endif //CHANGED 

	    pageTable[i].valid = TRUE;
	    pageTable[i].use = FALSE;
	    pageTable[i].dirty = FALSE;
	    pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
    }

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size 0x%x\n",
		 noffH.code.virtualAddr, noffH.code.size);
	  //executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
	  //		      noffH.code.size, noffH.code.inFileAddr);
        #ifdef CHANGED
        //TranslationEntry *virtualPageTable = new TranslationEntry[numPages];        
        ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
        #endif //CHANGED 
    }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size 0x%x\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
	  //executable->ReadAt (&
		//	      (machine->mainMemory
		//	       [noffH.initData.virtualAddr]),
		//	      noffH.initData.size, noffH.initData.inFileAddr);
        #ifdef CHANGED
        //TranslationEntry *virtualPageTable = new TranslationEntry[numPages];        
        ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
        #endif //CHANGED
      }

    DEBUG ('a', "Area for stacks at 0x%x, size 0x%x\n",
	   size - UserStacksAreaSize, UserStacksAreaSize);

    pageTable[0].valid = FALSE;			// Catch NULL dereference

    AddrSpaceList.Append(this);


    #ifdef CHANGED

    // To initialise semaphores
    mutex_countingThread = new Semaphore("thread counter mutex", 1);
    
    // a bitmap object to count the free section in stack
    stack_map = new BitMap(4); // 4 = number of user thread creatable and the parent thread

    // mark the section of the parent thread used
    stack_map->Mark(0);
    index_map = 0;
    
    #endif //CHANGED
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace () // we will use pageProvider.releasePage()
{
    #ifdef CHANGED
    DEBUG('x', "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo\n");
    for (unsigned int i = 0; i < numPages; i++)
    {
        //if(pageTable[i].valid == FALSE){
            pageProvider->ReleasePage(i, pageTable);
        //}
    }
    #endif

    delete [] pageTable;
    pageTable = NULL;

    AddrSpaceList.Remove(this);
    DEBUG('x', "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, USER_START_ADDRESS);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   numPages * PageSize - 16);
}

#ifdef CHANGED

/*
 To allocate the user stack in the address space 
 and check to wait for free stack section if too many user thread are already allocated
*/
int
AddrSpace::AllocateUserStack(){

    int addr;

    mutex_countingThread->P(); // to protect thread counting
    // increment the counter of created threads
    threadCount++;
    DEBUG('x', "Incrementation of threads = %d \n", threadCount);
    mutex_countingThread->V();

    /* // Old version of stack section allocation (replaced in action II.4)
    waiting_stack_map->P();
    int start_stack_new_thread = (numPages * PageSize) - (256 * threadCount) - 16;
    int limit_stack = (numPages * PageSize) - 1024 - 16;
    if(start_stack_new_thread < limit_stack){ // check if a stack section still can be allocated
        DEBUG('x', "Debug AllocateUserStack going to far on the CODE (thread = %d) \n", threadCount);
        return -1; // if a user thread can't be allocated
    }*/

    

    // to get the index of the stack free section (normally it exist thanks to semaphore just before which wait for a section to be freed)
    int index_check = stack_map->Find();
    DEBUG('x', "DEBUG Allocate a new stack with section %d\n", index_map);

    if(index_check != -1){
        index_map = index_check;
        // Set location of our new user thread's stack start
        machine->WriteRegister (StackReg, (numPages * PageSize) - (256 * index_map) - 16);
        DEBUG ('a', "Initializing stack register to 0x%x\n", numPages * PageSize - (256 * index_map) - 16);
        addr = machine->ReadRegister(StackReg);

        // return the address of the beginning of the user thread's stack
        return addr;
    }
    else{
        return -1;
    }
    
    
}

/*
 To finish a user thread, check if it is the last to do it and free stack section in the Bitmap
*/
void 
AddrSpace::FinishUserThreads(){
    /*
    DEBUG('x', "Debug : finish user thread begin (count = %d)\n", threadCount);

    //if I am the last, I stop nachos process
    if(threadCount == 0){


        DEBUG('x', "----------------------------\n");


        

        //free the process ressources
        //delete currentThread->space;
        //currentThread->space = NULL;

        //check if it's the last process to exit
        DEBUG('x', "DEBUG : before check if it is the last process to ending all / processCount = %d \n", processCount);
        if(processCount == 0){
            interrupt->Powerdown ();
        } else {
            DEBUG('x', "DEBUG : Finnish last User Thread, before curretnThread->Finnish() / currentThread = %d\n", currentThread);
            //currentThread->Finish();



            //decrement process counter
            mutex_countingProcess->P();
            processCount--;
            DEBUG('x', "Decrementation of process = %d / currentThread = %d\n", processCount, currentThread);
            mutex_countingProcess->V();


            delete currentThread->space;
            currentThread->space = NULL;
        }

    } else {

        mutex_countingThread->P(); // To protect thread counting and bitmap clearing from other thread actions

        stack_map->Clear(index_map);
        DEBUG('x', "DEBUG BitMap.Clear free the section %d\n", index_map);
        threadCount--;

        mutex_countingThread->V();

        waiting_stack_map->V(); // to notify that a thread is finnished ( and awake a new one if it is waiting to be allocated )
    }
     
    DEBUG('x', "DEBUG FinishUserThreads last step \n");
    */
        mutex_countingThread->P(); // To protect thread counting and bitmap clearing from other thread actions
        DEBUG('x', "DEBUG BitMap.Clear will free the section %d\n", index_map);
        stack_map->Clear(index_map);
        
        threadCount--;

        mutex_countingThread->V();
}   

// From TD3

int AddrSpace::GetThreadCount(){
    return threadCount;
    //return stack_map->NumClear();
}

#endif //CHANGED

//----------------------------------------------------------------------
// AddrSpace::Dump
//      Dump program layout as SVG
//----------------------------------------------------------------------

static void
DrawArea(FILE *output, unsigned sections_x, unsigned virtual_x,
	 unsigned y, unsigned blocksize,
	 struct segment *segment, const char *name)
{
    if (segment->size == 0)
	return;

    ASSERT((segment->virtualAddr % PageSize == 0));
    ASSERT((segment->size % PageSize == 0));
    unsigned page = segment->virtualAddr / PageSize;
    unsigned end = (segment->virtualAddr + segment->size) / PageSize;

    fprintf(output, "<rect x=\"%u\" y=\"%u\" width=\"%u\" height=\"%u\" "
		    "fill=\"#ffffff\" "
		    "stroke=\"#000000\" stroke-width=\"1\"/>\n",
		    sections_x, y - end * blocksize,
		    virtual_x - sections_x, (end - page) * blocksize);

    fprintf(output, "<text x=\"%u\" y=\"%u\" fill=\"#000000\" font-size=\"%u\">%s</text>\n",
	    sections_x, y - page * blocksize, blocksize, name);
}

unsigned
AddrSpace::Dump(FILE *output, unsigned addr_x, unsigned sections_x, unsigned virtual_x, unsigned virtual_width,
		unsigned physical_x, unsigned virtual_y, unsigned y,
		unsigned blocksize)
{
    unsigned ret = machine->DumpPageTable(output, pageTable, numPages,
	    addr_x, virtual_x, virtual_width, physical_x, virtual_y, y, blocksize);

    DrawArea(output, sections_x, virtual_x, virtual_y, blocksize, &noffH.code, "code");
    DrawArea(output, sections_x, virtual_x, virtual_y, blocksize, &noffH.initData, "data");
    DrawArea(output, sections_x, virtual_x, virtual_y, blocksize, &noffH.uninitData, "bss");

    DumpThreadsState(output, this, sections_x, virtual_x, virtual_y, blocksize);

    return ret;
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpacesRoom
//      Return how much room is needed for showing address spaces
//----------------------------------------------------------------------

unsigned
AddrSpacesRoom(unsigned blocksize)
{
    ListElement *element;
    unsigned room = 0;

    for (element = AddrSpaceList.FirstElement ();
	 element;
	 element = element->next) {
	AddrSpace *space = (AddrSpace*) element->item;
	room += machine->PageTableRoom(space->NumPages(), blocksize);
    }

    return room;
}

//----------------------------------------------------------------------
// AddrSpace::DumpAddrSpaces
//      Dump all address spaces
//----------------------------------------------------------------------

void
DumpAddrSpaces(FILE *output,
	       unsigned addr_x, unsigned sections_x, unsigned virtual_x, unsigned virtual_width,
	       unsigned physical_x, unsigned y, unsigned blocksize)
{
    ListElement *element;
    unsigned virtual_y = y;

    /* TODO: sort by physical page addresses to avoid too much mess */
    for (element = AddrSpaceList.FirstElement ();
	 element;
	 element = element->next) {
	AddrSpace *space = (AddrSpace*) element->item;
	virtual_y -= space->Dump(output, addr_x, sections_x, virtual_x, virtual_width, physical_x, virtual_y, y, blocksize);
    }
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->currentPageTable = pageTable;
    machine->currentPageTableSize = numPages;
}

#ifdef CHANGED
//FROM TD3
//TODO change its
/*
 Read numBytes octets from postion in executable
 And write in virtual addr space define by page table "pageTable" size of "numPages"
*/
void ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes, int position, TranslationEntry *pageTable, unsigned numPages){

    char tmp[numBytes];

    TranslationEntry *tmp_pageTable = machine->currentPageTable;
    int tmp_numPages = machine->currentPageTableSize;

    machine->currentPageTable = pageTable;
    machine->currentPageTableSize = numPages;

    executable->ReadAt(&tmp, numBytes, position);
    //copy the content of the executable file in a tampon
    for(int i = 0; i < numBytes; i++){
        machine->WriteMem(virtualaddr + i, 1, tmp[i]);    
    }

    machine->currentPageTable = tmp_pageTable;
    machine->currentPageTableSize = tmp_numPages;
}
#endif // CHANGED