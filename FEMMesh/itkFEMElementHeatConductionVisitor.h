/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementHeatConductionVisitor.h
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

#ifndef __FEMElementHeatConductionVisitor_h
#define __FEMElementHeatConductionVisitor_h


#include "itkFEMElement.h"

namespace itk
{
namespace fem
{


// This macro is intended to be used by the derived classes.
// it facilitates to declare all the types required there 
#define FEMElementHeatConductionVisitorTypesMacro( superclassType, elementType )   \
  typedef superclassType                                    Superclass; \
  typedef typename Superclass::RealType                     RealType; \
  typedef typename Superclass::MeshType                     MeshType; \
  typedef typename Superclass::MeshConstPointer             MeshConstPointer; \
  typedef typename Superclass::PointsContainer              PointsContainer;      \
  typedef typename Superclass::PointsContainerConstPointer  PointsContainerConstPointer; \
  typedef typename Superclass::PointDataContainer           PointDataContainer; \
  typedef typename Superclass::PointDataContainerConstPointer  PointDataContainerConstPointer; \
  typedef elementType                                       ElementType;  \
  typedef typename elementType::BaseCellType                CellType;  \
  typedef typename CellType::PointType                      PointType;  \
  typedef typename CellType::PointIdentifier                PointIdentifier;  \
  typedef typename CellType::PointIdConstIterator           PointIdConstIterator; \
  typedef typename MeshType::PixelType                      FieldVariableType;




/**

  \class FEMElementHeatConductionVisitor
  \brief This class implements a Visitor Pattern that computes
  the area of elements.

   */
template < typename TMesh >
class FEMElementHeatConductionVisitor
{

public:

  typedef double                              RealType;

  typedef TMesh                               MeshType;

  typedef typename 
  MeshType::ConstPointer                      MeshConstPointer;

  typedef typename 
  MeshType::PointsContainer                   PointsContainer;

  typedef typename 
  MeshType::PointsContainerConstPointer       PointsContainerConstPointer;

  typedef typename 
  MeshType::PointDataContainer                PointDataContainer;

  typedef typename 
  MeshType::PointDataContainerConstPointer    PointDataContainerConstPointer;



  // Set the mesh and cache some pointer for improving 
  // performance by taking this out from the Visit() method.
  void SetMesh( const MeshType * mesh )
     { 
     m_Mesh           =   mesh;
     if( !m_Mesh )
       {
       itkGenericExceptionMacro(<<"SetMesh() called with a null pointer");
       }
     m_Points         =   mesh->GetPoints();
     if( !m_Points )
       {
       itkGenericExceptionMacro(<<"SetMesh() called with a Mesh without Points");
       }
     m_PointData      =   mesh->GetPointData();
     if( !m_PointData )
       {
       itkGenericExceptionMacro(<<"SetMesh() called with a Mesh without PointData");
       }
    
     }


protected:
  
  // Pointer to the FEMMesh
  MeshConstPointer                  m_Mesh;

  // This is a ConstSmartPointer to the PointsContainer
  // caching this prevents from asking to the mesh
  // at each element visit.
  PointsContainerConstPointer       m_Points;

  // This is a ConstSmartPointer to the PointsContainer
  // caching this prevents from asking to the mesh
  // at each element visit.
  PointDataContainerConstPointer    m_PointData;


};
  

} // end namespace fem

} // end namespace itk


#endif


