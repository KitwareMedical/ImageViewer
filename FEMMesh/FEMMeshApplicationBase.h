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

#include "itkFEM2DDeformationNode.h"
#include "itkFEMLineCell.h"

// This is the base classe for the Application

class FEMMeshApplicationBase 
{

public:

  enum { PointsDimension              = 3 };

  // Type for the FEM Mesh containing both the geometrical
  // and physical aspects of the problem.
  typedef itk::fem::FEMMesh< PointsDimension > FEMMeshType;


  // Types related with Geometry. The points represent space
  // positions and the cells define the topology of the mesh.
  // Cells are also responsible for providing methods for 
  // mapping from geometrical space to parametric coordinates.
  typedef  FEMMeshType::PointType             BasePointType;
  typedef  FEMMeshType::CellType              BaseCellType;

  // Node type containing the Degrees of Freedom 
  // of the physical problem. 
  // Note that Nodes are not necessarily associated with 
  // points because the geometry can be represented by 
  // interpolating with a certain order while the physical
  // layer can use a different order.
  typedef  FEMMeshType::NodeType                   BaseNodeType;
  typedef  FEMMeshType::ElementType                BaseElementType;


  // Actual implementation types for Nodes, Cells and Elements
  // In this simple case we are creating Nodes of the same type,
  // Cells of the same type, Points of the same type and Elements
  // of the same type. However the use of polymorphism allows to
  // set any type of Node, Point, Cell or Element as long as it
  // derives from the corresponding Node, Point, Cell or Element
  // base class.
  typedef itk::fem::FEM2DDeformationNode           ActualNodeType;
  typedef itk::fem::FEMLineCell<PointsDimension>   ActualCellType;
  typedef BasePointType                            ActualPointType;
  typedef BaseElementType                          ActualElementType;

public:

  FEMMeshApplicationBase();
  ~FEMMeshApplicationBase();
  
  virtual void CreateSphere(void);
  virtual void CreateTriangle(void);
  virtual void CreateFEMMesh(void);
  virtual void DisplayFEMMesh(void);

protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;


  FEMMeshType::Pointer  m_FEMMesh;

private:

  // Internal data used to feed the MESH
  ActualPointType       *  m_Points;
  ActualNodeType        *  m_Nodes;
  ActualCellType        *  m_Cells;
  ActualElementType     *  m_Elements;

};


#endif
