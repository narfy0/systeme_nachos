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
    // ...
}

void ConsoleDriver::GetString(char *s, int n)
{
    // ...
}

#endif //CHANGED