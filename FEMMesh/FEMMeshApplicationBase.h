/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMeshApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef FEMMeshApplicationBase_H
#define FEMMeshApplicationBase_H


#include "itkMacro.h"

#include "vtkRenderWindow.h"
#include "itkFEMMesh.h"
#include "itkDefaultDynamicMeshTraits.h"

// This is the base classe for the Application

class FEMMeshApplicationBase 
{

public:

  enum { PointsDimension              = 3 };
  enum { MaximumTopologicalDimension  = 3 };

  typedef float     CoordinateRepresentationType;

  typedef double    InterpolationWeightsType;

  // These two types are provided here but 
  // not used because the additional data is
  // added through derivation of the itk::CellInterface
  // class and polymorphism.
  typedef char      PointDataType;
  typedef char      CellDataType;

  typedef itk::DefaultDynamicMeshTraits<
                                PointDataType,
                                PointsDimension,
                                MaximumTopologicalDimension,
                                CoordinateRepresentationType,
                                InterpolationWeightsType,
                                CellDataType
                                                  >  MeshTraits;

  typedef itk::Mesh< MeshTraits > MeshType;

  typedef itk::fem::FEMMesh< MeshType >   FEMMeshType;


public:

  FEMMeshApplicationBase();
  ~FEMMeshApplicationBase();
  
  virtual void CreateSphere(void);
  virtual void CreateTriangle(void);

protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;

  FEMMeshType::Pointer  * m_FEMMesh;

};


#endif
