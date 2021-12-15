#ifdef CHANGED

#include "pageprovider.h"
#include "bitmap.h"

//Constructor
PageProvider::PageProvider ()
{
    //the bit map which manage physical pages
    physicalPageTable_map = new BitMap(NumPhysPages);

    // mutex to lock setting reserved's page number value
    mutex_lockReserved = new Semaphore("mutex lock reserved page value", 1);

    mutex_lockReserved->P(); //maybe useless
    nbReserved = 0;
    mutex_lockReserved->V();
}

//Destructor
PageProvider::~PageProvider ()
{
    delete physicalPageTable_map;
    physicalPageTable_map = NULL;
}

int PageProvider::GetEmptyPage(){

    int freePageIndex = -1;
    int nbAvail = NumAvailPage();

    if(nbAvail > 0){

        //get index of a free page
        freePageIndex = physicalPageTable_map->Find();

        mutex_lockReserved->P();
        nbReserved--;
        mutex_lockReserved->V();

        //initialize found page to 0 (thanks to memeset)
        DEBUG('x', "PageProvider getEmptyPage : before memeset\n");

        if(freePageIndex != -1){
            memset(&(machine->mainMemory[freePageIndex * PageSize]), 0, PageSize);
        }

        DEBUG('x', "PageProvider getEmptyPage : after memset (returned value = %d)\n", freePageIndex);

    }
    
    return freePageIndex;
}

void PageProvider::ReservedPage(int nbToReserved){
    int nbAvail = NumAvailPage();

    ASSERT(nbAvail >= nbToReserved);

    mutex_lockReserved->P();
    nbReserved += nbToReserved;
    mutex_lockReserved->V();

}

void PageProvider::ReleasePage(int index_physical_page, TranslationEntry *pageTable){
    //set the bitamp to notify the page is released
    physicalPageTable_map->Clear(index_physical_page);
}

int PageProvider::NumAvailPage(){
    //get number of avaible free page reading the bitmap
    int nbFreePage = physicalPageTable_map->NumClear(); // - nulber of reserved page

    DEBUG('x', "PageProvider number of available pages = %d\n", nbFreePage);
    return nbFreePage - nbReserved;
}

//method to reserve a page

#endif //CHANGED