/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MultiResMIRegistration.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>

#include "SimpleApp.h"
#include "itkExceptionObject.h"


int main(int argc, char *argv[])
{
  if ( argc < 2 )
    {
    std::cout << "Parameter file name missing" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: MultiResMIRegistration param.file" << std::endl;
    return 1;
    }

  // run the registration
  try
    {
    typedef itk::SimpleApp<signed short> AppType;
    AppType::Pointer theApp = AppType::New();
    theApp->SetParameterFileName( argv[1] );
    theApp->Execute();

    }
  catch( itk::ExceptionObject& err)
    {
    std::cout << "Caught an ITK exception: " << std::endl;
    std::cout << err << std::endl;
    }
  catch(...)
    {
    std::cout << "Caught an non-ITK exception " << std::endl;
    }

  return 0;

}

