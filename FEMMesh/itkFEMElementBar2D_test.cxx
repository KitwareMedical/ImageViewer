/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementBar2D_test.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkFEMElementBar2D_test.h"


namespace itk {
namespace fem {

/* Define mapping between points and local DOF */
const unsigned int Bar2D::pt2dof[2][2] = 
  { {0,1},
    {2,3} };

Bar2D::Bar2D()
{
  /* Initialize the dofs array */
  for(int i=0;i<NDOF;i++)
  {
    dofs[i]=-1;
  }

}


}} // end namespace itk::fem
