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


/** 

  This program illustrate how to make use of the FEM classes
  

  */

//#include "itkFEMElementBar2D_test.h"

#include "FEMMeshApplication.h"



int main(int, char**)
{
//  itk::fem::Bar2D a;

  FEMMeshApplication  application;

  application.Show();

  Fl::run();

  return 0;


}
