/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThinPlateSplinesApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ThinPlateSplinesApplicationBase_H
#define ThinPlateSplinesApplicationBase_H


#include "itkMacro.h"

#include "vtkRenderWindow.h"

#include "itkPointSet.h"
#include "itkDefaultStaticMeshTraits.h"

#include <set>



// This is the base classe for the Application

class ThinPlateSplinesApplicationBase 
{

public:

  enum { PointsDimension              = 2 };

  typedef char              PointDataType;
  typedef float             CoordinateRepresentationType;
  typedef float             InterpolationWeightType;
  typedef char              CellDataType;

  enum  { MaxTopologicalDimension = 3 };
 
  typedef itk::DefaultStaticMeshTraits<
                                  PointDataType,
                                  PointsDimension,
                                  MaxTopologicalDimension,
                                  CoordinateRepresentationType,
                                  InterpolationWeightType,
                                  CellDataType 
                                          >       MeshTraits;
      
  // Set of points to convert
  typedef itk::PointSet< MeshTraits > PointSetType;


public:

  ThinPlateSplinesApplicationBase();
  ~ThinPlateSplinesApplicationBase();
  
  virtual void DisplayAxes(void);
  virtual void CreateSpline(void);

protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;


  PointSetType::Pointer  m_PointSet;

private:


};




#endif
