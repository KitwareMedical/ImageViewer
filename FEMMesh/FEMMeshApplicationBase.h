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
#include "itkLineCell.h"

#include "itkFEMHeatConduction2D.h"

// This is the base classe for the Application

class FEMMeshApplicationBase 
{

public:

  enum { PointsDimension              = 3 };
  enum { MaximumTopologicalDimension  = 3 };
  enum { DisplacementDimension        = 3 };

  // Type used for representing the coordinates of points
  // in geometric space
  typedef float     CoordinateRepresentationType;

  // Type used by the Cells for the weights required for
  // interpolation of points and values.
  typedef double    InterpolationWeightsType;

  // PointDataType is the type that will be used for the
  // Degrees of Freedom (or DisplacementField) of the 
  // FEM problem.
  typedef float           DisplacementRepresentationType;
  typedef itk::Vector< DisplacementRepresentationType,
                       DisplacementDimension >  DisplacementType;

  typedef DisplacementType               PointDataType;

  // For FEM it is expected that eventual values stored
  // at the Cells will be of the same type that those
  // values stored at the Points.
  typedef PointDataType   CellDataType;


  // Traits for all the elements of the Mesh
  typedef itk::DefaultDynamicMeshTraits<
                                PointDataType,
                                PointsDimension,
                                MaximumTopologicalDimension,
                                CoordinateRepresentationType,
                                InterpolationWeightsType,
                                CellDataType
                                                  >  MeshTraits;

  // Type for the Cell Traits
  typedef MeshTraits::CellTraits                CellTraits;

  // Type for the Geometrical level
  typedef itk::Mesh<  PointDataType, 
                      PointsDimension, 
                      MeshTraits >              MeshType;

  // Type for the Physical level
  typedef itk::fem::FEMMesh< MeshType >         FEMMeshType;

  // Type for giving an unique identifier to every cell
  typedef  FEMMeshType::CellIdentifier          CellIdentifierType;

  // Types related with Points
  typedef  FEMMeshType::PointType               PointType;
  typedef  FEMMeshType::PointIdentifier         PointIdentifierType;

  // Type for the Multivisitor that will iterate over the Cells
  typedef  FEMMeshType::CellMultiVisitorType    CellMultiVisitorType;

  // Type for One dimensional Cells
  typedef  itk::LineCell< PointDataType, CellTraits >   LineCellType;


  typedef itk::fem::FEMHeatConduction2D        HeatSolverType;
  typedef HeatSolverType::FEMMeshType          HeatMeshType;

public:

  FEMMeshApplicationBase();

  virtual ~FEMMeshApplicationBase();
  
  virtual void CreateSphere(void);
  virtual void CreateTriangle(void);
  virtual void CreateFEMMesh(void);
  virtual void CreateFEMMesh2(void);
  virtual void DisplayFEMMesh(void);
  virtual void DisplayFEMMesh2(void);
  virtual void DisplayAxes(void);

protected:
 
  vtkRenderWindow     * m_RenderWindow;
  vtkRenderer         * m_Renderer;


  FEMMeshType::Pointer    m_FEMMesh;

  HeatSolverType::Pointer m_HeatSolver;

};


#endif
