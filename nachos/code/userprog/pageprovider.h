#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "machine.h"
//TODO see if we keep it 
//#include "system.h"
#include "bitmap.h"
#include "synch.h"

class System;
extern Machine *machine;	// user program memory and registers

class PageProvider:public dontcopythis
{

  public:
    PageProvider ();	// Create a page provider
    ~PageProvider ();	// De-allocate a page provider

    int GetEmptyPage();

    void ReleasePage(int index_physical_page, TranslationEntry *pageTable);

    int NumAvailPage();

    void ReservedPage(int nbToReserved);


  private:
    // Number of pages in the page table
    unsigned int num_pages;

    // Number of reserved page which will be allocate
    unsigned int nbReserved;

    // the table of accessible section in stack
    BitMap *physicalPageTable_map;

    // mutex to lock setting reserved's page number value
    Semaphore *mutex_lockReserved;

    
};

#endif // PAGEPROVIDER_H
#endif // CHANGED
