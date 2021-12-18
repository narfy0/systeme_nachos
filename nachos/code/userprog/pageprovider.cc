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

    delete mutex_lockReserved;
    mutex_lockReserved = NULL;

    nbReserved = NULL;
}

int PageProvider::GetEmptyPage(){

    int freePageIndex = -1;
    
    freePageIndex = physicalPageTable_map->Find();

    //DEBUG('x', "PageProvider getEmptyPage : before memeset\n");
    if(freePageIndex != -1){ // if a free page exist 

        mutex_lockReserved->P();
        nbReserved--;
        mutex_lockReserved->V();

        //initialize found page to 0 (thanks to memeset)
        memset(&(machine->mainMemory[freePageIndex * PageSize]), 0, PageSize);
    }

    //DEBUG('x', "PageProvider getEmptyPage : after memset (returned value = %d)\n", freePageIndex);

    return freePageIndex;
}

void PageProvider::ReservedPage(int nbToReserved){
    int nbAvail = NumAvailPage();

    //check if there is enough page to reserve this number
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
    int nbFreePage = physicalPageTable_map->NumClear();

    //DEBUG('x', "PageProvider number of available pages = %d\n", nbFreePage);
    return nbFreePage - nbReserved;
}

#endif //CHANGED