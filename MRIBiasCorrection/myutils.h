/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    myutils.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MYUTILS_H
#define __MYUTILS_H

#include <vnl/vnl_vector.h>
#include <iostream>

void print_line(std::string msg)
{
  std::cout << msg << std::endl ;
}

void dump(vnl_vector<double> vect)
{
  std::cout << "vector dump" << std::endl ;
  for (unsigned int i = 0 ; i < vect.size() ; i++)
    {
    std::cout << i << ": " << vect[i] << std::endl ;
    }
}
#endif
