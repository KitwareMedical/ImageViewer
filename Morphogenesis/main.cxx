#include "TissueViewer.h"


int main()
{

  TissueViewer * viewer = new TissueViewer();

  
  viewer->Show();
  
  Fl::run();

  delete viewer;

  return 0;
}
