#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

//Semaphore for threads
static Semaphore *thread_charSemaphore;
static Semaphore *thread_stringSemaphore;


static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
    readAvail = new Semaphore("read await", 0);
    writeDone = new Semaphore("write done", 0);

    thread_charSemaphore = new Semaphore("thread char method mutex", 1);
    thread_stringSemaphore = new Semaphore("thread string method mutex", 1);

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
    thread_charSemaphore->P(); //mutex to protect from multiple thread request

    console->TX(ch);
	writeDone->P ();	// wait for write to finish

    thread_charSemaphore->V();
  
}

int ConsoleDriver::GetChar()
{
    thread_charSemaphore->P(); //mutex to protect from multiple thread request
  
    readAvail->P ();	// wait for character to arrive
    thread_charSemaphore->V();

	return console->RX ();
}

void ConsoleDriver::PutString(const char s[])
{
    thread_stringSemaphore->P(); //mutex to protect string print to print all string before an other thread

    int i;
    for(i = 0; s[i] != '\0'; i++){ //to "read" all the char tab
        //put the current char
        PutChar(s[i]);
    }

    thread_stringSemaphore->V();
}

void ConsoleDriver::GetString(char *s, int n)
{
    thread_stringSemaphore->P(); 

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

    thread_stringSemaphore->V();
}


#endif //CHANGED