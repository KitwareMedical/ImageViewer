/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    main.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// Disable warning for long symbol names in this file only
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "ceExtractorConsole.h"


/**
 *
 *   Main program 
 *
 */
int main()
{

  ceExtractorConsole * console = new ceExtractorConsole();

  console->Show();
  console->SetSigma( 4.0 );
  console->ShowInput();

  Fl::run();

  delete console;
  
  return 0;

}


















