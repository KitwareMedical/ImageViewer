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

  liImageRegistrationConsole * console = new liImageRegistrationConsole();

  console->Show();
  console->ShowAbout();

  Fl::run();

  delete console;
  
  return 0;
}



