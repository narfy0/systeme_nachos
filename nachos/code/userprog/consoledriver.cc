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
    for(i = 0; s[i] != '\0'; i++){ //to "read" all the char tab
        //put the current char
        PutChar(s[i]);
    }
    #endif // CHANGED
}

void ConsoleDriver::GetString(char *s, int n)
{
    #ifdef CHANGED
    int i;
    char c;
    for(i = 0; i < n; i++){ //to "read" all the char tab
        //put the current char
        c = GetChar();
        *s = c;
        s++;
        
        if(c == '\n' || c == EOF){
            break;
        }
        
    }
    *s = '\0';
    #endif // CHANGED
}


#endif //CHANGED