#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
    readAvail = new Semaphore("read await", 0);
    writeDone = new Semaphore("write done", 0);
    // console = ... 
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}

ConsoleDriver::~ConsoleDriver()
{
    delete console;
    delete writeDone;
    delete readAvail;
}

void ConsoleDriver::PutChar(int ch)
{ 
    console->TX(ch);
	writeDone->P ();	// wait for write to finish
}

int ConsoleDriver::GetChar()
{
    readAvail->P ();	// wait for character to arrive
	return console->RX ();
}

void ConsoleDriver::PutString(const char s[])
{
    #ifdef CHANGED
    int i;
    for(i = 0; s[i] == '\0'; i++){ //to "read" all the char tab
        //put the current char
        PutChar(s[i]);
    }
    #endif // CHANGED
}

void ConsoleDriver::GetString(char *s, int n)
{
    // ...
}

/**
 * Copy a string, char by char, from MIPS world (using "from" address in args)
 * to the kernel world (using "to" address in args). Use ReadMem() method.
 * Stop when copy '\0' which have to be here in order to ensure system's security
 * 
 * Returns number of copied characters
 **/
int copyStringFromMachine(int from, char *to, unsigned size)
{
    //...
}

#endif //CHANGED