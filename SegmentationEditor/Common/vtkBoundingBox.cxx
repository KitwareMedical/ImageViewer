/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBoundingBox.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkObjectFactory.h"
#include "vtkBoundingBox.h"

vtkBoundingBox* vtkBoundingBox::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkBoundingBox");
  if(ret)
    {
    return (vtkBoundingBox*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkBoundingBox;
}

vtkBoundingBox::vtkBoundingBox()
{
  Extent[0] = 0; Extent[1] = 0;
  Extent[2] = 0; Extent[3] = 0;
  Extent[4] = 0; Extent[5] = 0;
}

void vtkBoundingBox::SetExtent(int x0, int x1, int y0, int y1, int z0, int z1)
{
  Extent[0] = x0; Extent[1] = x1;
  Extent[2] = y0; Extent[3] = y1;
  Extent[4] = z0; Extent[5] = z1;
}

void vtkBoundingBox::Merge(const vtkBoundingBox *b)
{
  if ( b->Extent[0] < Extent[0] ) Extent[0] = b->Extent[0];
  if ( b->Extent[2] < Extent[2] ) Extent[2] = b->Extent[2];
  if ( b->Extent[4] < Extent[4] ) Extent[4] = b->Extent[4];
  
  if ( b->Extent[1] > Extent[1] ) Extent[1] = b->Extent[1];
  if ( b->Extent[3] > Extent[3] ) Extent[3] = b->Extent[3];
  if ( b->Extent[5] > Extent[5] ) Extent[5] = b->Extent[5];
}
