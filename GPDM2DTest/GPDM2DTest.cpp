/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GPDM2DTest.cpp
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "MyTestAppUI.h"

int main()
{

  
  MyTestAppInterface * mywin = new MyTestAppInterface;

  mywin->Show();

  Fl::run();

//  delete console;

  return 0;
}


