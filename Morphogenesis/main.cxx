#include "TissueViewer.h"


int main()
{

  bio::TissueViewer * viewer = new bio::TissueViewer();

  
  viewer->Show();
  
  Fl::run();

  delete viewer;

  return 0;
}
