/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBoundingBox.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// .Name vtkBoundingBox
// .Section Description
#ifndef __vtkBoundingBox_
#define __vtkBoundingBox_

#include "vtkObject.h"

class VTK_COMMON_EXPORT vtkBoundingBox: public vtkObject
{
public:
  static vtkBoundingBox *New();

  vtkTypeMacro(vtkBoundingBox,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) {}

  const int *GetExtent() const
    {
      return Extent;
    }
  void SetExtent(int x0, int x1, int y0, int y1, int z0, int z1);
  void Merge(const vtkBoundingBox *);
  
protected:
  vtkBoundingBox();
  ~vtkBoundingBox() {}
  
private:
  int Extent[6];
    
};

#endif
