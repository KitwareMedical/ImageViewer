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


#include <set>



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

  // Structures to hold pointers to objects that should be 
  // deleted at destruction time of the application.
  // This is needed because the FEMMesh is holding arrays of 
  // pointers to all the objects it manages: Points, Nodes,
  // Cells, Elements.
  typedef  std::set< BasePointType   * >    PointsToBeDeletedContainer;
  typedef  std::set< BaseNodeType    * >    NodesToBeDeletedContainer;
  typedef  std::set< BaseCellType    * >    CellsToBeDeletedContainer;
  typedef  std::set< BaseElementType * >    ElementsToBeDeletedContainer;

public:

  FEMMeshApplicationBase();
  ~FEMMeshApplicationBase();
  
  virtual void CreateSphere(void);
  virtual void CreateTriangle(void);
  virtual void CreateFEMMesh(void);
  virtual void DisplayFEMMesh(void);
  virtual void DisplayAxes(void);

protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;


  FEMMeshType::Pointer  m_FEMMesh;

private:

  // Internal data used to feed the MESH
  // This tables of pointers are keept here
  // in order to facilitate memory release
  // when the program is quitting.
  // The FEMMesh itself will not release 
  // any of the pointer that are paased to
  // it as Point *, Node *, Cell *, Element *.
  //
  // std::set is used to make sure that no
  // pointer is deleted twice.
  //
  // NOTE that ONLY those Points, Nodes, 
  // Cells or Elements that are dynamically
  // allocated should be included in these 
  // sets.
  //
  PointsToBeDeletedContainer      m_PointsToBeDeleted;
  NodesToBeDeletedContainer       m_NodesToBeDeleted;
  CellsToBeDeletedContainer       m_CellsToBeDeleted;
  ElementsToBeDeletedContainer    m_ElementsToBeDeleted;


};




#endif
