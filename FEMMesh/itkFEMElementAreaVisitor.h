/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementAreaVisitor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __FEMElementAreaVisitor_h
#define __FEMElementAreaVisitor_h


#include "itkFEMElement.h"

namespace itk
{
namespace fem
{


// This macro is intended to be used by the derived classes.
// it facilitates to declare all the types required there 
#define FEMElementAreaVisitorTypesMacro( superclassType, elementType )   \
  typedef superclassType                                    Superclass; \
  typedef typename Superclass::MeshType                     MeshType; \
  typedef typename Superclass::MeshConstPointer             MeshConstPointer; \
  typedef typename Superclass::PointsContainerConstPointer  PointsContainerConstPointer; \
  typedef elementType                                       ElementType;  \
  typedef typename elementType::BaseCellType                CellType;  \
  typedef typename CellType::PointType                      PointType;  \
  typedef typename CellType::PointIdConstIterator           PointIdConstIterator; \




/**

  \class FEMElementAreaVisitor
  \brief This class implements a Visitor Pattern that computes
  the area of elements.

   */
template < typename TMesh >
class FEMElementAreaVisitor
{

public:

  typedef TMesh                                           MeshType;
  typedef typename MeshType::ConstPointer                 MeshConstPointer;
  typedef typename MeshType::PointsContainerConstPointer  PointsContainerConstPointer;



  // Set the mesh and cache some pointer for improving 
  // performance by taking this out from the Visit() method.
  void SetMesh( const MeshType * mesh )
     { 
     m_Mesh           =   mesh;
     m_Points         =   mesh->GetPoints();
     m_CumulatedArea  =   0.0f;
     }

  // Return the sum of the areas of all the visited elements
  double GetArea(void) const
    { return m_CumulatedArea; }


protected:
  
  MeshConstPointer                  m_Mesh;

  // This is a SmartPointer to the PointsContainer
  // caching this prevents from asking to the mesh
  // at each element visit.
  PointsContainerConstPointer       m_Points;


  // Variable to cumulate the area of all visited elements
  double                            m_CumulatedArea;

};
  

} // end namespace fem

} // end namespace itk


#endif


