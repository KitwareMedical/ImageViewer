/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FuzzyConnectedness.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "FuzzyConnectApp.h"
#include "itkExceptionObject.h"


int main(int argc, char *argv[])
{

  // run the application
  try
    {
    FuzzyConnectApp theApp;
    theApp.Execute();
    }
  catch( itk::ExceptionObject& err)
    {
    std::cout << "Caught an exception: " << std::endl;
    std::cout << err.GetLocation() << std::endl;
    std::cout << err.GetDescription() << std::endl;
    }

  return 0;

}

