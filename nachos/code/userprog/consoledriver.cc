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
    int isEndOfString = 0;
    for(i = 0; isEndOfString; i++){ //to "read" all the char tab
        char currentChar = s[i];

        //put the current char
        PutChar(currentChar);

        //if it's the end of the string
        if(currentChar == '\0'){
            isEndOfString = 1; //switch this value to stop the for loop
        }
    }
    #endif // CHANGED
}

void ConsoleDriver::GetString(char *s, int n)
{
    // ...
}

#endif //CHANGED