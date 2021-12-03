#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "machine.h"
#include "system.h"
#include "bitmap.h"

class PageProvider:public dontcopythis
{

  public:
    PageProvider ();	// Create a page provider
    ~PageProvider ();	// De-allocate a page provider

    int GetEmptyPage();

    void ReleasePage();

    int NumAvailPage();


  private:
    // Number of pages in the page table
    unsigned int num_pages;

    // the table of accessible section in stack
    BitMap *physicalPageTable_map;

    
};

#endif // PAGEPROVIDER_H
#endif // CHANGED
