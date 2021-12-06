#ifdef CHANGED

#include "pageprovider.h"
#include "bitmap.h"

//Constructor
PageProvider::PageProvider ()
{
    //the bit map which manage physical pages
    physicalPageTable_map = new BitMap(NumPhysPages);
}

//Destructor
PageProvider::~PageProvider ()
{
    delete physicalPageTable_map;
    physicalPageTable_map = NULL;
}

int PageProvider::GetEmptyPage(){
    //get index of a free page
    int freePageIndex = physicalPageTable_map->Find();

    //initialize found page to 0 (thanks to memeset)
    DEBUG('x', "PageProvider getEmptyPage : before memeset\n");
    
    memset(&(machine->mainMemory[freePageIndex * PageSize]), 0, PageSize);

    DEBUG('x', "PageProvider getEmptyPage : after memset (returned value = %d)\n", freePageIndex);
    
    return freePageIndex;
}

void PageProvider::ReleasePage(int index_physical_page, TranslationEntry *pageTable){
    //free a page
    pageTable[index_physical_page].valid = TRUE;

    //set the bitamp to notify the page is released
    physicalPageTable_map->Clear(index_physical_page);
}

int PageProvider::NumAvailPage(){
    //get number of avaible free page reading the bitmap
    int nbFreePage = physicalPageTable_map->NumClear(); // - nulber of reserved page

    DEBUG('x', "PageProvider number of available pages = %d\n", nbFreePage);
    return nbFreePage;
}

//method to reserve a page

#endif //CHANGED