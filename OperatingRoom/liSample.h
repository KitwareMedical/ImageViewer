/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSample.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liSAMPLE
#define liSAMPLE

#include "itkPoint.h"

class ITK_EXPORT liSample {
public:
  typedef     double                     CoordType;
  typedef     itk::Point<CoordType,3>    PointType;
  typedef     itk::Vector<CoordType,3>   VectorType;
  PointType   point;
  float       value; 
};

#endif
