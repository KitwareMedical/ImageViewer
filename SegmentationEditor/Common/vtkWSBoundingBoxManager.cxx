/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkWSBoundingBoxManager.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "vtkWSBoundingBoxManager.h"
#include "vtkObjectFactory.h"

vtkWSBoundingBoxManager* vtkWSBoundingBoxManager::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkWSBoundingBoxManager");
  if(ret)
    {
    return (vtkWSBoundingBoxManager*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkWSBoundingBoxManager;
}

vtkWSBoundingBoxManager::vtkWSBoundingBoxManager()
{
  this->LabeledImage = 0;
}

bounding_box_t vtkWSBoundingBoxManager::GetBoundingBox(unsigned long n)
{
  vtkBoundingBoxHash::iterator boxPtr = BoundingBoxTable.find(n);
  if (boxPtr == BoundingBoxTable.end())
    {
      vtkWarningMacro(<< "No box with label " << n << " can be found.");
      bounding_box_t null_box;
      null_box.x0 = null_box.x1 = null_box.y0 = null_box.y1 = null_box.z0 =
        null_box.z1 = 0;
      return null_box;
    }
  
  return ( (*boxPtr).second );
}

void vtkWSBoundingBoxManager
::GetBoundingBox(vtkBoundingBox *box, unsigned long   n)
{
  bounding_box_t res = this->GetBoundingBox(n);
  box->SetExtent(res.x0, res.x1, res.y0, res.y1, res.z0, res.z1);
}

void vtkWSBoundingBoxManager::GenerateBoundingBoxes()
{
  // Check the labeled image
  if (this->LabeledImage == 0)
    {
      vtkErrorMacro(<< "No LabeledImage has been specified");
      exit(-1);
    }
  
  // Check scalar type of labeled.
  if ( this->LabeledImage->GetScalarType() != VTK_UNSIGNED_LONG )
    {
      vtkErrorMacro(<< "LabeledImage must be unsigned long data type.");
      exit(-1);
    }

  BoundingBoxTable.clear(); // empty the current table
  
  // Loop through the extent of the image.  At each pixel do the
  // following:  Create or look up that pixel value's (label) entry in the
  // table.  Reset the bounds accordingly.
  int x, y, z, sx0, sx1, sy0, sy1, sz0, sz1;
  this->LabeledImage->GetExtent(sx0, sx1, sy0, sy1, sz0, sz1);

  unsigned long *dataPtr
    = (unsigned long *)(this->LabeledImage->GetScalarPointer(sx0, sy0, sz0));

  vtkBoundingBoxHash::iterator boxPtr;
  bounding_box_t tempBox;
  
  for (z = sz0; z <= sz1; z++)
    {
      for (y = sy0; y <= sy1; y++)
        {
          for (x = sx0; x <= sx1; x++)
            {
              boxPtr = BoundingBoxTable.find(*dataPtr);
              if (boxPtr == BoundingBoxTable.end())
                {
                  // Add a new entry
                  tempBox.x0 = tempBox.x1 = x;
                  tempBox.y0 = tempBox.y1 = y;
                  tempBox.z0 = tempBox.z1 = z;
                  
                  BoundingBoxTable.insert(
                         vtkBoundingBoxHash::value_type(*dataPtr, tempBox) );

                }
              else
                {
                  if ( x < (*boxPtr).second.x0 ) (*boxPtr).second.x0 = x;
                  if ( y < (*boxPtr).second.y0 ) (*boxPtr).second.y0 = y;
                  if ( z < (*boxPtr).second.z0 ) (*boxPtr).second.z0 = z;
                  
                  if ( x > (*boxPtr).second.x1 ) (*boxPtr).second.x1 = x;
                  if ( y > (*boxPtr).second.y1 ) (*boxPtr).second.y1 = y;
                  if ( z > (*boxPtr).second.z1 ) (*boxPtr).second.z1 = z;
                }
              dataPtr++;
            }
        }
    }
  
}
