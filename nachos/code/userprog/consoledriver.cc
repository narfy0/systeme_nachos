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
{ //ne doit pas ecrire \0 (cf le man de putchar)
        // line under belongs to getchar and not putchar ?
	  //readAvail->P ();	// wait for character to arrive
	  // ch = console->RX (); // > get from COnsoleTest example

      // if(ch != '\n' && ch != EOF){ //TODO check if this condition is necessary
        //not necesarry to add '<' and '>' around each char ? like in action II.
          console->TX(ch);
	      writeDone->P ();	// wait for write to finish
      //}

    /** CHECK IF ITS NECESSARY
	  if (ch == 'q') {
	      //printf ("Nothing more, bye!\n");
          printf("Au revoir\n");
	      break;		// if q, quit
	  }

      // EOF 
      if (ch == EOF){
        printf("Au revoir ! (EOF)\n");
        break;//if end of file, quit
      }
      **/

}

int ConsoleDriver::GetChar()
{   //is it all ?
    readAvail->P ();	// wait for character to arrive
	return console->RX (); //TODO check it
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