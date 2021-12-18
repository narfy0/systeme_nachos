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

// mutex to controll the incrementation and decrementation of running threads
static Semaphore *mutex_countingThread;
//mutex to controll the reservation of memory page when use PageProvider
//initialize here because it is used in the "constructor"
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

    #ifdef CHANGED

    // check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory
    /*
    if (numPages > NumPhysPages)
	    throw std::bad_alloc();
    */
    //check if enough page 
    //ASSERT(numPages <= pageProvider->NumAvailPage());
    if(numPages > pageProvider->NumAvailPage()){
        throw threadException();
    }

    DEBUG ('a', "Initializing address space, num pages %d, total size 0x%x\n", numPages, size);

    // first, set up the translation 
    pageTable = new TranslationEntry[numPages];

    //To reserved page in the page provider which will be allocated
    mutex_reserved_page->P();
    pageProvider->ReservedPage(numPages);
    mutex_reserved_page->V();
    
    for (i = 0; i < numPages; i++)
    {
	  //Action I.4
        //pageTable[i].physicalPage = i + 1;	// for now, phys page # = virtual page #

      //Action I.6
        //DEBUG('x', "AddrSpace initialization : before pageprovider.getEmptyPage : loop turn = %d\n", i);
        mutex_reserved_page->P();

        int emptyPageIndex = pageProvider->GetEmptyPage();
        //DEBUG('x', "AddrSpace initialization : pageprovider.getEmptyPage returns : %d\n", emptyPageIndex);
        pageTable[i].physicalPage = emptyPageIndex;	

        mutex_reserved_page->V();

        //DEBUG('x', "AddrSpace initialization : after pageprovider.getEmptyPage : loop turn = %d\n", i);
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
	    DEBUG ('a', "Initializing code segment, at 0x%x, size 0x%x\n", noffH.code.virtualAddr, noffH.code.size);

        #ifdef CHANGED
        //executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
        //		      noffH.code.size, noffH.code.inFileAddr);
        //TranslationEntry *virtualPageTable = new TranslationEntry[numPages];        
        ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
        
        #endif //CHANGED 
    }
    if (noffH.initData.size > 0)
      {
	    DEBUG ('a', "Initializing data segment, at 0x%x, size 0x%x\n",
		 noffH.initData.virtualAddr, noffH.initData.size);

        #ifdef CHANGED 

	    //executable->ReadAt (&
		//	      (machine->mainMemory
		//	       [noffH.initData.virtualAddr]),
		//	      noffH.initData.size, noffH.initData.inFileAddr);
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
    stack_map = new BitMap(4); // 4 = number of user thread creatable + the parent thread

    // mark the section of the parent thread used
    stack_map->Mark(0);
    index_map = 0;

    //set the address space's stack index for the current thread
    currentThread->setIndex(0); 
    
    #endif //CHANGED
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace () // we will use pageProvider.releasePage()
{
    #ifdef CHANGED

    for (unsigned int i = 0; i < numPages; i++)
    {
        pageProvider->ReleasePage(i, pageTable);
    }

    #endif

    delete [] pageTable;
    pageTable = NULL;

    AddrSpaceList.Remove(this);
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
    To check if the stack is full 
        return 1 if the stack is full and other number else
*/
int AddrSpace::isFull(){
    return stack_map->NumClear() + 1;
}

/*
    To allocate the user stack in the address space 
    and check to wait for free stack section if too many user thread are already allocated
*/
int
AddrSpace::AllocateUserStack(){

    int addr;

    /* // Old version of stack section allocation (replaced in action II.4)
    waiting_stack_map->P();
    int start_stack_new_thread = (numPages * PageSize) - (256 * threadCount) - 16;
    int limit_stack = (numPages * PageSize) - 1024 - 16;
    if(start_stack_new_thread < limit_stack){ // check if a stack section still can be allocated
        DEBUG('x', "Debug AllocateUserStack going to far on the CODE (thread = %d) \n", threadCount);
        return -1; // if a user thread can't be allocated
    }*/

    // to get the index of the stack free section (normally it exist thanks to semaphore just before which wait for a section to be freed)
    int index_check = currentThread->getIndex();

    //DEBUG('x', "DEBUG Allocate a new stack with section %d\n", index_map);

    if(index_check != -1){ // if the index is correct SO if allocatiob is possible
        index_map = index_check;
        return (numPages * PageSize) - (256 * index_map) - 16;
    
    } else { //if the allocation is not possible
        return -1;
    }
    
}

/*
    Return the number of the first free index in the stack
      As a side effect, set this index as marked/used
      (In other words, find and allocate a page.)
      If no index are clear, return -1.
*/
int AddrSpace::FindIndexStack(){
    return stack_map->Find();
}

/*
 To finish a user thread, check if it is the last to do it and free stack section in the Bitmap
*/
void AddrSpace::FinishUserThreads(){

    mutex_countingThread->P(); // To protect thread counting and bitmap clearing from other thread actions

    index_map = currentThread->getIndex();
    DEBUG('x', "DEBUG BitMap.Clear will free the section %d, map %p %p \n", index_map, stack_map, mutex_countingThread);
    stack_map->Clear(index_map);

    mutex_countingThread->V();
}   

// From TD3

/*
    To get the number of thread running
*/
int AddrSpace::GetThreadCount(){
    return 4 - stack_map->NumClear();
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

#endif // CHANGED3