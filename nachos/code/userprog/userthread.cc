#ifdef CHANGED

#include "userthread.h"

typedef struct {
    int f;
    int arg;
} Schmurtz;

static void StartUserThread(void *schmurtz){

}

int do_ThreadCreate(int f, int arg){
    Schmurtz *schmurtz;
    schmurtz->f = f;
    schmurtz->arg = arg;

    //...

    StartUserThread(schmurtz); 
}

void do_ThreadExit(){
    
}



#endif // CHANGED