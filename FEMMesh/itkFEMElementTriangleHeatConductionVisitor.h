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
    const PointIdentifier    & id0 = *pts++;
    const PointIdentifier    & id1 = *pts++;
    const PointIdentifier    & id2 = *pts++;
    const PointType          & p0  = points.ElementAt( id0 );
    const PointType          & p1  = points.ElementAt( id1 );
    const PointType          & p2  = points.ElementAt( id2 );

    StiffnessMatrixType & stiffnessMatrix = *(m_StiffnessMatrix);
    
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
    const RealType sm00 = ( b0*b0 + c0*c0 ) / areaTwice;
    const RealType sm01 = ( b0*b1 + c0*c1 ) / areaTwice;
    const RealType sm02 = ( b0*b2 + c0*c2 ) / areaTwice;
    const RealType sm10 = sm01;
    const RealType sm11 = ( b1*b1 + c1*c1 ) / areaTwice;
    const RealType sm12 = ( b1*b2 + c1*c2 ) / areaTwice;
    const RealType sm20 = sm02;
    const RealType sm21 = sm12;
    const RealType sm22 = ( b2*b2 + c2*c2 ) / areaTwice;

    // add the contributions to the stiffness matrix
    stiffnessMatrix[ id0 ][ id0 ] += sm00;
    stiffnessMatrix[ id0 ][ id1 ] += sm01;
    stiffnessMatrix[ id0 ][ id2 ] += sm02;
    stiffnessMatrix[ id1 ][ id0 ] += sm10;
    stiffnessMatrix[ id1 ][ id1 ] += sm11;
    stiffnessMatrix[ id1 ][ id2 ] += sm12;
    stiffnessMatrix[ id2 ][ id0 ] += sm20;
    stiffnessMatrix[ id2 ][ id1 ] += sm21;
    stiffnessMatrix[ id2 ][ id2 ] += sm22;
    }



};
  

} // end namespace fem

} // end namespace itk


#endif


