/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBoundingBoxHash.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __vtkBoundingBoxHash_h_
#define __vtkBoundingBoxHash_h_

#include <hash_map>

struct bounding_box_t
{
  int x0;
  int x1;
  int y0;
  int y1;
  int z0;
  int z1;
};

typedef hash_map<unsigned long, bounding_box_t, hash<unsigned long> >
  vtkBoundingBoxHash;


#endif
