/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBinaryVolume.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// .Name vtkBinaryVolume
// .Section Description
// 
#ifndef __vtkBinaryVolume_
#define __vtkBinaryVolume_

#include "vtkImageData.h"

class VTK_EXPORT vtkBinaryVolume : public vtkImageData
{
public:
  static vtkBinaryVolume *New();

  vtkTypeMacro(vtkBinaryVolume,vtkImageData);
  void PrintSelf(ostream& os, vtkIndent indent) {}

  void Clear();

  void SetWithRadius(int x, int y, int z);
  void UnsetWithRadius(int x, int y, int z);
  
  void  Set(int x, int y, int z)
    { *( (unsigned char *)( GetScalarPointer(x, y, z) )) = 1;}
  void Unset(int x, int y, int z)
    { *( (unsigned char *)( GetScalarPointer(x, y, z) )) = 0;}
  bool   Get(int x, int y, int z)
    { return (bool) (*( (unsigned char *)( GetScalarPointer(x, y, z) ))); }

  int WriteToDisk(const char *fn);
  int ReadFromDisk(const char *fn);
  
  // For interfacing with Tcl.
  int   GetAsInt(int x, int y, int z)
    { return (int) (*( (unsigned char *)( GetScalarPointer(x, y, z) ))); }
  float GetAsFloat(int x, int y, int z)
    { return (float) (*( (unsigned char *)( GetScalarPointer(x, y, z) ))); }

  void SetPaintRadius(int r)
    { paint_radius = r; }

  int GetPaintRadius()
    { return paint_radius;}
  
protected:
  int paint_radius;
  
  vtkBinaryVolume();
  ~vtkBinaryVolume() {}
  
private:

  
};

#endif
