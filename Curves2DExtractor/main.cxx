#include "ceExtractorConsole.h"


/**
 *
 *   Main program 
 *
 */
int main()
{

  ceExtractorConsole * console = new ceExtractorConsole();

  console->Show();
  console->SetSigma( 4.0 );
  console->ShowInput();

  Fl::run();

  delete console;
  
  return 0;

}


















