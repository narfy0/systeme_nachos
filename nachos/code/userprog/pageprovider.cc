#ifdef CHANGED

#include "pageprovider.h"
#include "bitmap.h"

//Constructor
PageProvider::PageProvider (unsigned int numPages)
{
    //the bit map which manage physical pages
    physicalPageTable_map = new BitMap(numPages);
}

//Destructor
PageProvider::~PageProvider ()
{
}

void GetEmptyPage(){

}

void ReleasePage(){

}

void NumAvailPage(){

}

#endif //CHANGED