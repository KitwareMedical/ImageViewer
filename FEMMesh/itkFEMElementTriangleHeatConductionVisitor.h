/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementTriangleHeatConductionVisitor.h
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

#ifndef __FEMElementTriangleHeatConductionVisitor_h
#define __FEMElementTriangleHeatConductionVisitor_h


#include "itkFEMElementTriangle.h"
#include "itkFEMElementHeatConductionVisitor.h"
#include "itkMatrix.h"


namespace itk
{

namespace fem
{

/**

  \class FEMElementTriangleHeatConductionVisitor
  \brief This class implements a Visitor Pattern that computes
  the area of Triangular elements.

   */
template < typename TMesh >
class FEMElementTriangleHeatConductionVisitor : 
            public FEMElementHeatConductionVisitor< TMesh >
{

public:


  // This macro takes care of inheriting all the typedef from the Superclass
  FEMElementHeatConductionVisitorTypesMacro( 
                  FEMElementHeatConductionVisitor< TMesh >,
                  FEMElementTriangle< TMesh >        
                                                               );
 

  // Visit the Element and compute the HeatConduction
  void Visit( unsigned long cellId, ElementType * t )
    {
    const CellType           & cell   = t->GetCell();
    PointIdConstIterator       pts    = cell.PointIdsBegin();
    const PointsContainer    & points = *(m_Points.GetPointer());
    const PointDataContainer & values = *(m_PointData.GetPointer());
    const PointIdentifier    & id0 = *pts++;
    const PointIdentifier    & id1 = *pts++;
    const PointIdentifier    & id2 = *pts++;
    const FieldVariableType  & v0  = values.ElementAt( id0 );
    const FieldVariableType  & v1  = values.ElementAt( id1 );
    const FieldVariableType  & v2  = values.ElementAt( id2 );
    const PointType          & p0  = points.ElementAt( id0 );
    const PointType          & p1  = points.ElementAt( id1 );
    const PointType          & p2  = points.ElementAt( id2 );
    
    const double areaTwice = ( p1[0] * p2[1] - p1[1] * p2[0] + 
                               p2[0] * p0[1] - p2[1] * p0[0] +
                               p0[0] * p1[1] - p0[1] * p1[0]    );

    // Compute coefficients of the linear equations for
    // the Shape functions
    const RealType c0 = p2[0] - p1[0];
    const RealType c1 = p0[0] - p2[0];
    const RealType c2 = p1[0] - p0[0];

    const RealType b0 = p1[1] - p2[1];
    const RealType b1 = p2[1] - p0[1];
    const RealType b2 = p0[1] - p1[1];

    // Compute the contributions to the StiffnessMatrix 
    // in the master equation.
    m_StiffnessMatrix[ 0 ][ 0 ] = ( b0*b0 + c0*c0 ) / areaTwice;
    m_StiffnessMatrix[ 0 ][ 1 ] = ( b0*b1 + c0*c1 ) / areaTwice;
    m_StiffnessMatrix[ 0 ][ 2 ] = ( b0*b2 + c0*c2 ) / areaTwice;
    m_StiffnessMatrix[ 1 ][ 0 ] = ( b1*b0 + c1*c0 ) / areaTwice;
    m_StiffnessMatrix[ 1 ][ 1 ] = ( b1*b1 + c1*c1 ) / areaTwice;
    m_StiffnessMatrix[ 1 ][ 2 ] = ( b1*b2 + c1*c2 ) / areaTwice;
    m_StiffnessMatrix[ 2 ][ 0 ] = ( b2*b0 + c2*c0 ) / areaTwice;
    m_StiffnessMatrix[ 2 ][ 1 ] = ( b2*b1 + c2*c1 ) / areaTwice;
    m_StiffnessMatrix[ 2 ][ 2 ] = ( b2*b2 + c2*c2 ) / areaTwice;

    std::cout << "StiffnessMatrix = " << m_StiffnessMatrix << std::endl;
    }

private:

  Matrix< RealType, 3, 3 >      m_StiffnessMatrix;


};
  

} // end namespace fem

} // end namespace itk


#endif


