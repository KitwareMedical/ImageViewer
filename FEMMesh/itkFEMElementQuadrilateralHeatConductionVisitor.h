/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementQuadrilateralHeatConductionVisitor.h
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

#ifndef __FEMElementQuadrilateralHeatConductionVisitor_h
#define __FEMElementQuadrilateralHeatConductionVisitor_h


#include "itkFEMElementQuadrilateral.h"
#include "itkFEMElementHeatConductionVisitor.h"

namespace itk
{

namespace fem
{

/**

  \class FEMElementQuadrilateralHeatConductionVisitor
  \brief This class implements a Visitor Pattern that computes
  the area of Quadrilateral elements.

   */
template < typename TMesh >
class FEMElementQuadrilateralHeatConductionVisitor : 
          public FEMElementHeatConductionVisitor< TMesh >
{

public:


  // This macro takes care of inheriting all the typedef from the Superclass
  FEMElementHeatConductionVisitorTypesMacro( 
                      FEMElementHeatConductionVisitor< TMesh >,
                      FEMElementQuadrilateral< TMesh >        
                                                                  );
 

  // Visit the Element and compute the HeatConduction
  void Visit( unsigned long cellId, ElementType * t )
    {
    ElementType & element = *t;
    ElementType::ShapeFunctionsDerivativesType shapeFunctionsDerivatives;
    ElementType::ParametricPointType gaussPoint;
    element.ComputeShapeFunctionDerivativesAt( gaussPoint, shapeFunctionsDerivatives );
    }
};
  

} // end namespace fem

} // end namespace itk


#endif


