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


  // Traits for all the elements of the Mesh
  typedef itk::fem::DefaultDynamicMeshTraits<
                                PointDataType,
                                PointsDimension,
                                MaximumTopologicalDimension,
                                CoordinateRepresentationType,
                                InterpolationWeightsType,
                                CellDataType
                                                  >  MeshTraits;


  // Type for the Geometrical level
  typedef itk::fem::Mesh< MeshTraits >        MeshType;

  // Type for the Physical level
  typedef itk::fem::FEMMesh< MeshType >       FEMMeshType;

  // Node type. It is derived from a Zero dimension Cell
  typedef  FEMMeshType::NodeType              NodeType;
  typedef  NodeType::Pointer                  NodePointer;
  typedef  FEMMeshType::CellIdentifier        CellIdentifierType;


  // Types related with Points
  typedef  FEMMeshType::PointType             PointType;
  typedef  FEMMeshType::PointIdentifier       PointIdentifierType;

public:

  FEMMeshApplicationBase();
  ~FEMMeshApplicationBase();
  
  virtual void CreateSphere(void);
  virtual void CreateTriangle(void);
  virtual void CreateFEMMesh(void);


protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;


  FEMMeshType::Pointer  m_FEMMesh;

};


#endif
