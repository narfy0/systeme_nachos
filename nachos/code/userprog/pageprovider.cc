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
}

int PageProvider::GetEmptyPage(){
    //get index of a free page
    int freePageIndex = physicalPageTable_map->Find();
    //TODO think about find() side effect !

    //initialize found page to 0 (thanks to memeset)
    //TODO
    /*
    for each addrs in physicalPage{
        memset(addr, 0, pageSize);
    }
    */

    return freePageIndex;
}

void PageProvider::ReleasePage(){
    //get an empty page index
    int emptyPageIndex = GetEmptyPage();

    //free a page
    //TODO

    //set the bitamp to notify the page is released
    physicalPageTable_map->Clear(emptyPageIndex);
}

int PageProvider::NumAvailPage(){
    //get number of avaible free page reading the bitmap
    int nbFreePage = physicalPageTable_map->NumClear();

    DEBUG('x', "PageProvider number of available pages = %d\n", nbFreePage);
    return nbFreePage;
}

#endif //CHANGED