
#include "itkExceptionObject.h"
#include "SurfaceGenerator.h"


int main()
{

  SurfaceGenerator::Pointer generator = SurfaceGenerator::New();

  generator->Show();
  generator->ShowDisplay();
  
  try 
    {
    Fl::run();
    }
  catch( itk::ExceptionObject & exception )
    {
    std::cout << "Exception caught !" << std::endl;
    std::cout << "Description : " << exception.GetDescription() << std::endl;
    std::cout << "Location    : " << exception.GetLocation()    << std::endl;
    }

  return 0;

}



