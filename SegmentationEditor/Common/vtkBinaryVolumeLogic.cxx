/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBinaryVolumeLogic.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkBinaryVolumeLogic.h"
#include "vtkObjectFactory.h"

vtkBinaryVolumeLogic* vtkBinaryVolumeLogic::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkBinaryVolumeLogic");
  if(ret)
    {
    return (vtkBinaryVolumeLogic*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkBinaryVolumeLogic;
}


void vtkBinaryVolumeLogic::AddEquivalencies(vtkWSLookupTableManager *manager,
                                            vtkWSBoundingBoxManager *bboxes)
{
  // Get the equivalency list and add each one.
  bounding_box_t box;
  
  const unsigned long *eqList = manager->GetEquivalencies();
  if (eqList == 0) return;
  unsigned long sz = eqList[0];
  for (unsigned i = 1; i < sz+1; ++i)
    {
      box = bboxes->GetBoundingBox(eqList[i]);

      //          std::cout << "Adding label: " << eqList[i] << " with extent " <<
      //              box.x0 << " " <<  box.x1 << " " <<  box.y0 << " " <<  box.y1 << " " <<
      //              box.z0 << " " <<  box.z1 << std::endl;
      
      this->Add(box.x0, box.x1, box.y0, box.y1, box.z0, box.z1, eqList[i]);
    }
  
}

void vtkBinaryVolumeLogic::Add(int x0, int x1, int y0, int y1, int z0, int z1,
                               unsigned long value)
{
  // Error checking.  Also verifies that the volumes have been set and are of
  // the correct data type.
  this->CheckExtent(x0, x1, y0, y1, z0, z1);

  // Calculate wrap offsets for iterating through the source volume.  These are 
  // the values used to increment pointers at the edges of the bounding box
  // (extent).
  int sx0, sx1, sy0, sy1, sz0, sz1, xWrapOffset, yWrapOffset;
  this->SourceVolume->GetExtent(sx0, sx1, sy0, sy1, sz0, sz1);

  //      std::cout << "Source extent is " << sx0 << " " << sx1 << " " << sy0 << " " << 
  //        sy1 << " " << sz0 << " " << sz1 << std::endl;
  //      std::cout << "Bounding box is " <<  x0 << " " << x1 << " " << y0 << " " << 
  //        y1 << " " << z0 << " " << z1 << std::endl;
    
  xWrapOffset = (sx1 - x1) + (x0 - sx0);
  yWrapOffset = ( (sy1 - y1) + (y0 - sy0) ) * ((sx1 - sx0) + 1);

  //     std::cout << "xWrapOffset = " << xWrapOffset << std::endl;
  //      std::cout << "yWrapOffset = " << yWrapOffset << std::endl;
    
  // Loop over the extent.  Bounds checking has already been done on the source 
  // and binary volumes.
  int x, y, z;
  
  unsigned long *dataPtr
    = (unsigned long *)(this->SourceVolume->GetScalarPointer(x0, y0, z0));

  for (z = z0; z <= z1; z++)
    {
      for (y = y0; y <= y1; y++)
        {
          for (x = x0; x <= x1; x++)
            {
              if (*dataPtr == value)
                {  this->BinaryVolume->Set(x, y, z); }
              dataPtr++;
            }
          dataPtr += xWrapOffset;
        }
      dataPtr += yWrapOffset;
    }
  this->BinaryVolume->Modified();
}

void vtkBinaryVolumeLogic::SubtractEquivalencies(vtkWSLookupTableManager *manager,
                                            vtkWSBoundingBoxManager *bboxes)
{
  // Get the equivalency list and add each one.
  bounding_box_t box;
  
  const unsigned long *eqList = manager->GetEquivalencies();
  if (eqList == 0) return;
  unsigned long sz = eqList[0];
  for (unsigned i = 1; i < sz+1; ++i)
    {
      box = bboxes->GetBoundingBox(eqList[i]);

      //      std::cout << "Adding label: " << eqList[i] << " with extent " <<
      //        box.x0 << " " <<  box.x1 << " " <<  box.y0 << " " <<  box.y1 << " " <<
      //        box.z0 << " " <<  box.z1 << std::endl;
      
      this->Subtract(box.x0, box.x1, box.y0, box.y1, box.z0, box.z1, eqList[i]);
    }
  
}

void vtkBinaryVolumeLogic::Subtract(int x0, int x1, int y0, int y1, int z0, int z1,
                               unsigned long value)
{
  // Error checking.  Also verifies that the volumes have been set and are of
  // the correct data type.
  this->CheckExtent(x0, x1, y0, y1, z0, z1);

  // Calculate wrap offsets for iterating through the source volume.  These are 
  // the values used to increment pointers at the edges of the bounding box
  // (extent).
  int sx0, sx1, sy0, sy1, sz0, sz1, xWrapOffset, yWrapOffset;
  this->SourceVolume->GetExtent(sx0, sx1, sy0, sy1, sz0, sz1);

  //  std::cout << "Source extent is " << sx0 << " " << sx1 << " " << sy0 << " " << 
  //    sy1 << " " << sz0 << " " << sz1 << std::endl;
  //  std::cout << "Bounding box is " <<  x0 << " " << x1 << " " << y0 << " " << 
  //    y1 << " " << z0 << " " << z1 << std::endl;
    
  xWrapOffset = (sx1 - x1) + (x0 - sx0);
  yWrapOffset = ( (sy1 - y1) + (y0 - sy0) ) * ((sx1 - sx0) + 1);
  //  std::cout << "xWrapOffset = " << xWrapOffset << std::endl;
  //  std::cout << "yWrapOffset = " << yWrapOffset << std::endl;
    
  // Loop over the extent.  Bounds checking has already been done on the source 
  // and binary volumes.
  int x, y, z;
  
  unsigned long *dataPtr
    = (unsigned long *)(this->SourceVolume->GetScalarPointer(x0, y0, z0));

  for (z = z0; z <= z1; z++)
    {
      for (y = y0; y <= y1; y++)
        {
          for (x = x0; x <= x1; x++)
            {
              if (*dataPtr == value)
                {  this->BinaryVolume->Unset(x, y, z); }
              dataPtr++;
            }
          dataPtr += xWrapOffset;
        }
      dataPtr += yWrapOffset;
    }
  this->BinaryVolume->Modified();
}

void vtkBinaryVolumeLogic::CheckExtent(int x0, int x1, int y0, int y1, int z0,
                                       int z1)

{
  int sx0, sx1, sy0, sy1, sz0, sz1;

  // Check to see that we have set the volumes
  if (this->BinaryVolume == 0)
    {
      vtkErrorMacro(<< "No Binary Volume Specified.");
      exit(-1);
    }
  if (this->SourceVolume == 0)
    {
      vtkErrorMacro(<< "No Source Volume Specified.");
      exit(-1);
    }
  
  // Check scalar type of source volume.
  if ( this->SourceVolume->GetScalarType() != VTK_UNSIGNED_LONG )
    {
      vtkErrorMacro(<< "SourceVolume must be unsigned long data type.");
      exit(-1);
    }

  
  // Check the boundaries of the volumes.
  this->SourceVolume->GetExtent(sx0, sx1, sy0, sy1, sz0, sz1);
  if (   x0 < sx0 || x0 > sx1 || x1 > sx1 || x1 < sx0
      || y0 < sy0 || y0 > sy1 || y1 > sy1 || y1 < sy0
      || z0 < sz0 || z0 > sz1 || z1 > sz1 || z1 < sz0 )
    {
      vtkErrorMacro(<< "Extent is not contained within the extent of the SourceImage");
      exit(-1);
    }

  this->BinaryVolume->GetExtent(sx0, sx1, sy0, sy1, sz0, sz1);
  if (   x0 < sx0 || x0 > sx1 || x1 > sx1 || x1 < sx0
         || y0 < sy0 || y0 > sy1 || y1 > sy1 || y1 < sy0
         || z0 < sz0 || z0 > sz1 || z1 > sz1 || z1 < sz0 )
    {
      vtkErrorMacro(<< "Extent is not contained within the extent of the BinaryImage");
      exit(-1);
    }  
}
