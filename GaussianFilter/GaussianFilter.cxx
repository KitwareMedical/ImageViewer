/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GaussianFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#include <GaussianFilter.h>


int main()
{

  liFilterConsole * console = new liFilterConsole();

  console->Show();
  console->ShowAbout();

  Fl::run();

  delete console;
  
  return 0;
}



