#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"
#include "noff.h"
#include "list.h"

#include "bitmap.h"

class PageProvider:public dontcopythis
{

  public:
    PageProvider (unsigned int numPages);	// Create a page provider
    ~PageProvider ();	// De-allocate a page provider

    void GetEmptyPage();

    void ReleasePage();

    void NumAvailPage();


  private:
    // Number of pages in the page table
    unsigned int num_pages;

    // the table of accessible section in stack
    BitMap *physicalPageTable_map;

    
};



extern List AddrspaceList;

#endif // PAGEPROVIDER_H
#endif // CHANGED
