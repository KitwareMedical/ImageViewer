#include "MatroxViewer.h"
#include "itkExceptionObject.h"


int main()
{
  ISIS::MatroxViewer application;

  application.Show();

  try
    {
    Fl::run();
    }
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << "ITK Exception thrown !" << std::endl;
    std::cerr << exp << std::endl;
    }
  catch( std::exception & exp )
    {
    std::cerr << "STL Exception thrown !" << std::endl;
    std::cerr << exp.what() << std::endl;
    }
  catch( ... )
    {
    std::cerr << "Unknown Exception thrown !" << std::endl;
    }

return 0;
}


