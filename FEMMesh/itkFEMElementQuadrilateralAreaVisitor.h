/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementQuadrilateralAreaVisitor.h
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

#ifndef __FEMElementQuadrilateralAreaVisitor_h
#define __FEMElementQuadrilateralAreaVisitor_h


#include "itkFEMElementQuadrilateral.h"
#include "itkFEMElementAreaVisitor.h"

namespace itk
{

namespace fem
{

/**

  \class FEMElementQuadrilateralAreaVisitor
  \brief This class implements a Visitor Pattern that computes
  the area of Triangular elements.

   */
template < typename TMesh >
class FEMElementQuadrilateralAreaVisitor : public FEMElementAreaVisitor< TMesh >
{

public:



  // This macro takes care of inheriting all the typedef from the Superclass
  FEMElementAreaVisitorTypesMacro(  FEMElementAreaVisitor< TMesh >,
                                    FEMElementQuadrilateral< TMesh >  );
 

  // Visit the Element and compute the Area
  void Visit( unsigned long cellId, ElementType * t )
    {
    const CellType & cell = t->GetCell();
    PointIdConstIterator pts = cell.PointIdsBegin();
    const PointType & p0 = m_Points->ElementAt( *pts++ );
    const PointType & p1 = m_Points->ElementAt( *pts++ );
    const PointType & p2 = m_Points->ElementAt( *pts++ );
    const PointType & p3 = m_Points->ElementAt( *pts++ );
    std::cout << std::endl;
    std::cout << "p0 = " << p0 << std::endl;
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p3 = " << p2 << std::endl;
    const double area = ( p1[0] * p2[1] - p1[1] * p2[0] + 
                          p2[0] * p0[1] - p2[1] * p0[0] +
                          p0[0] * p1[1] - p0[1] * p1[0] +

                          p2[0] * p3[1] - p2[1] * p3[0] + 
                          p3[0] * p0[1] - p3[1] * p0[0] +
                          p0[0] * p2[1] - p0[1] * p2[0] 
                                                           ) / 2.0f;
                          
    std::cout << "Area = " << area << std::endl;
    m_CumulatedArea += area;
    }

};
  

} // end namespace fem

} // end namespace itk


#endif


