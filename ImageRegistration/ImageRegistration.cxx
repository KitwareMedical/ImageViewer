/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistration.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <ImageRegistration.h>


int main()
{

  try 
    {
    liImageRegistrationConsole * console = new liImageRegistrationConsole();
    console->Show();
    Fl::run();
    delete console;
    }
  catch(...)
    {
    std::cerr << "Exception caught in main" << std::endl;
    }

  return 0;

}



