/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GaussianFilter2D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <GaussianFilter2D.h>

int main()
{

  
  liFilterConsole2D * console = new liFilterConsole2D();

  console->Show();

  Fl::run();

  delete console;
  
  return 0;
}



