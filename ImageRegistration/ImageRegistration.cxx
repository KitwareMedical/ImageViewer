/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistration.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#include <ImageRegistration.h>


int main()
{

  liImageRegistrationConsole * console = new liImageRegistrationConsole();

  console->Show();

  Fl::run();

  delete console;
  
  return 0;
}



