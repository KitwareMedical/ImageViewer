/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBinaryVolumeLogic.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// .Name vtkBinaryVolumeLogic
// .Section Description
// 
#ifndef __vtkBinaryVolumeLogic_
#define __vtkBinaryVolumeLogic_
#include "vtkImageData.h"
#include "vtkBinaryVolume.h"
#include "vtkWSLookupTableManager.h"
#include "vtkWSBoundingBoxManager.h"

class VTK_EXPORT vtkBinaryVolumeLogic : public vtkObject
{
public:
  static vtkBinaryVolumeLogic *New();

  vtkTypeMacro(vtkBinaryVolumeLogic,vtkImageData);
  void PrintSelf(ostream& os, vtkIndent indent) {}

  // Searches a bounding box in the SourceVolume for indicies whose pixels
  // match the value parameter.  Turns ON corresponding indicies in the
  // BinaryVolume.
  void Add(int, int, int, int, int, int, unsigned long);
  void AddEquivalencies(vtkWSLookupTableManager *, vtkWSBoundingBoxManager *);

  // Searches a bounding box in the SourceVolume for indicies whose pixels
  // match the value parameter.  Turns FF corresponding indicies in the
  // BinaryVolume.
  void Subtract(int, int, int, int, int, int, unsigned long);
  void SubtractEquivalencies(vtkWSLookupTableManager *, vtkWSBoundingBoxManager *);
  
  vtkSetObjectMacro(SourceVolume, vtkImageData);
  vtkGetObjectMacro(SourceVolume, vtkImageData);

  vtkGetObjectMacro(BinaryVolume, vtkBinaryVolume);
  vtkSetObjectMacro(BinaryVolume, vtkBinaryVolume);
  
protected:
  vtkBinaryVolumeLogic()
    { SourceVolume = 0;   BinaryVolume = 0;    }
  ~vtkBinaryVolumeLogic() {}

  void CheckExtent(int, int, int, int, int, int);
  
private:
  vtkImageData  *SourceVolume;
  vtkBinaryVolume *BinaryVolume;
 
};

#endif
