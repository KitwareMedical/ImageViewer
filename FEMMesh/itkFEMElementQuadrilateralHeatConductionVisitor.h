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
    ElementType::ParametricPointsArrayType        gaussPoints;
    ElementType::IntegrationWeightsArrayType      gaussWeights;
    ElementType::ShapeFunctionsDerivativesType    shapeFunctionsDerivatives;
    ElementType::JacobianMatrixType               jacobian;

    const CellType           & cell   = t->GetCell();
    PointIdConstIterator       pts    = cell.PointIdsBegin();
    const PointIdentifier    & id0 = *pts++;
    const PointIdentifier    & id1 = *pts++;
    const PointIdentifier    & id2 = *pts++;
    const PointIdentifier    & id3 = *pts++;

    const unsigned int numberOfDisplacementComponents = 
                          ElementType::NumberOfDisplacementComponents;

    StiffnessMatrixType   localStiffnessMatrix(
                                  numberOfDisplacementComponents, 
                                  numberOfDisplacementComponents    );

    element.GetGaussIntegrationPoints( gaussPoints );
    element.GetGaussIntegrationWeights( gaussWeights );

    const ElementType::PointsContainer & meshPoints = *(m_Points.GetPointer());
    const unsigned int numberOfIntegrationPoints = gaussPoints.Size();

    for(unsigned int p=0; p<numberOfIntegrationPoints; p++)
      {
      element.ComputeShapeFunctionDerivativesAt( 
                       gaussPoints[p], shapeFunctionsDerivatives );

      element.ComputeJacobianMatrixAt( gaussPoints[p], meshPoints, jacobian );

      for(unsigned int i=0; i<numberOfDisplacementComponents; i++)
        {
        for(unsigned int j=0; j<numberOfDisplacementComponents; j++)
          {
          localStiffnessMatrix[i][j] += gaussWeights[p] *
            ( shapeFunctionsDerivatives[i][0] * shapeFunctionsDerivatives[j][0] +
              shapeFunctionsDerivatives[i][1] * shapeFunctionsDerivatives[j][1] );
          }
        }
      }

    // add the contributions to the stiffness matrix
    StiffnessMatrixType & globalStiffnessMatrix = *(m_StiffnessMatrix);
    globalStiffnessMatrix[ id0 ][ id0 ] += localStiffnessMatrix[0][0];
    globalStiffnessMatrix[ id0 ][ id1 ] += localStiffnessMatrix[0][1];
    globalStiffnessMatrix[ id0 ][ id2 ] += localStiffnessMatrix[0][2];
    globalStiffnessMatrix[ id0 ][ id3 ] += localStiffnessMatrix[0][3];
    globalStiffnessMatrix[ id1 ][ id0 ] += localStiffnessMatrix[1][0];
    globalStiffnessMatrix[ id1 ][ id1 ] += localStiffnessMatrix[1][1];
    globalStiffnessMatrix[ id1 ][ id2 ] += localStiffnessMatrix[1][2];
    globalStiffnessMatrix[ id1 ][ id3 ] += localStiffnessMatrix[1][3];
    globalStiffnessMatrix[ id2 ][ id0 ] += localStiffnessMatrix[2][0];
    globalStiffnessMatrix[ id2 ][ id1 ] += localStiffnessMatrix[2][1];
    globalStiffnessMatrix[ id2 ][ id2 ] += localStiffnessMatrix[2][2];
    globalStiffnessMatrix[ id2 ][ id3 ] += localStiffnessMatrix[2][3];
    globalStiffnessMatrix[ id3 ][ id0 ] += localStiffnessMatrix[3][0];
    globalStiffnessMatrix[ id3 ][ id1 ] += localStiffnessMatrix[3][1];
    globalStiffnessMatrix[ id3 ][ id2 ] += localStiffnessMatrix[3][2];
    globalStiffnessMatrix[ id3 ][ id3 ] += localStiffnessMatrix[3][3];
    }
};
  

} // end namespace fem

} // end namespace itk


#endif


