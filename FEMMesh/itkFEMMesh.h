/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMMesh.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMMesh_h
#define __itkFEMMesh_h


#include "itkFEMNode.h"
#include "itkFEMElement.h"
#include "itkFEMCell.h"
#include "itkPoint.h"
#include "itkObject.h"
#include "itkObjectFactory.h"


namespace itk {
namespace fem {




/** 
 * \class FEMMesh
 *
 * FEM Solver Class 
 */
template< unsigned int NPointDimension >
class FEMMesh : public Object

{

public:
   /** Standard typedefs. */
  typedef FEMMesh                   Self;
  typedef Object                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Standard part of every itk Object.
    \todo probably a fully resolved type should be used here... */
  itkTypeMacro(Mesh, Object); 

  /** type used for representing coordinates in space 
      it is used as template argument for the Points and Vectors */ 
  typedef float   CoordinateRepresentationType;

  /** Dimension of the geometrical space */
  enum { PointDimension = NPointDimension };

  /** Type used to represent positions in space  */
  typedef Point< CoordinateRepresentationType, PointDimension > PointType;  

  /** Type for the Cells */
  typedef FEMCell< PointDimension >          CellType;

  /** Type for the Nodes */
  typedef FEMNode                            NodeType;

  /** Type for the Element */
  typedef FEMElement                         ElementType;

  /** Type for storing the Points  */
  typedef std::vector< PointType * >         PointsContainerType;

  /** Type for storing the Cells */
  typedef std::vector< CellType * >          CellsContainerType;

  /** Type for storing the Nodes  */
  typedef std::vector< NodeType * >          NodesContainerType;

  /** Type for storing the Elements */
  typedef std::vector< ElementType * >       ElementsContainerType;

  /** Return the number of points currently stored */
  unsigned long GetNumberOfPoints(void) const;

  /** Return the number of cells currently stored */
  unsigned long GetNumberOfCells(void) const;

  /** Return the number of elements currently stored */
  unsigned long GetNumberOfElements(void) const;

  /** Return the number of nodes currently stored */
  unsigned long GetNumberOfNodes(void) const;

  /** Add Point */
  void AddPoint( PointType * );

  /** Add Node */
  void AddNode( NodeType * );

  /** Add Cell */
  void AddCell( CellType * );

  /** Add Element */
  void AddElement( ElementType * );

protected:

  /** Constructor for use by New() method. */
  FEMMesh();
  virtual ~FEMMesh();

     
private:
  FEMMesh(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
          
  // Dimension of the Space in which the geometry of the Mesh is represented
  unsigned int      m_PointsDimension;
  
  // Container of points defining the geometry of the mesh in space
  PointsContainerType   m_Points;

  // Container of Nodes defining the degrees of freedom
  NodesContainerType   m_Nodes;

  // Container of cells defining the geometry of the mesh in space
  CellsContainerType    m_Cells;

  // Container of Elements defining the physics of the problem
  ElementsContainerType   m_Elements;



protected:

  void PrintSelf(std::ostream& os, Indent indent) const;

};



}} // end namespace itk::fem

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMMesh.txx"
#endif

#endif /* #ifndef __itkFEMMesh_h */



