/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMHeatConduction2D.cxx
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

#include "itkFEMHeatConduction2D.h"
#include "itkFEMElementTriangleHeatConductionVisitor.h"
#include "itkFEMElementQuadrilateralHeatConductionVisitor.h"
#include "vnl/algo/vnl_lsqr.h"


namespace itk {
namespace fem {


FEMHeatConduction2D
::FEMHeatConduction2D()
{
   m_Mesh = FEMMeshType::New();
}


FEMHeatConduction2D
::~FEMHeatConduction2D()
{

            
}

 
void
FEMHeatConduction2D
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}





void
FEMHeatConduction2D
::AssembleMasterEquation(void)
{

  // Allocate memory and initialize the global stiffness matrix
  this->InitializeStiffnessMatrix();


  // Instantiate Visitors

  // Types for Triangles
  typedef itk::fem::FEMElementTriangle< 
                            FEMMeshType >  TriangleElementType;
 
  typedef itk::fem::FEMElementTriangleHeatConductionVisitor< 
                            FEMMeshType >  TriangleHeatConductionVisitorType;

  
  typedef itk::fem::FEMElementVisitorImplementation<
                                        FEMMeshType,
                                        TriangleElementType,
                                        TriangleHeatConductionVisitorType  
                                            >  TriangleHeatConductionVisitorImplementationType;


  TriangleHeatConductionVisitorImplementationType::Pointer triangleVisitor = 
                                  TriangleHeatConductionVisitorImplementationType::New();



  // Types for Quadrilaterals
  typedef itk::fem::FEMElementQuadrilateral< 
                            FEMMeshType >  QuadrilateralElementType;
 
  typedef itk::fem::FEMElementQuadrilateralHeatConductionVisitor< 
                            FEMMeshType >  QuadrilateralHeatConductionVisitorType;

  
  typedef itk::fem::FEMElementVisitorImplementation<
                                        FEMMeshType,
                                        QuadrilateralElementType,
                                        QuadrilateralHeatConductionVisitorType  
                                            >  QuadrilateralHeatConductionVisitorImplementationType;


  QuadrilateralHeatConductionVisitorImplementationType::Pointer quadrilateralVisitor = 
                                  QuadrilateralHeatConductionVisitorImplementationType::New();



  triangleVisitor->SetMesh( m_Mesh );
  quadrilateralVisitor->SetMesh( m_Mesh );


  triangleVisitor->SetStiffnessMatrix( m_StiffnessMatrix );
  quadrilateralVisitor->SetStiffnessMatrix( m_StiffnessMatrix );



  // Create a MultiVisitor
  typedef FEMMeshType::ElementMultiVisitorType       MultiVisitorType;

  MultiVisitorType::Pointer multiVisitor    = MultiVisitorType::New();


  multiVisitor->AddVisitor( triangleVisitor );
  multiVisitor->AddVisitor( quadrilateralVisitor );



  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  m_Mesh->Accept( multiVisitor );
  
  std::cout << "Heat Conduction Visitors Done ! " << std::endl;

  std::cout << "Stiffness Matrix = " << std::endl;
  std::cout << m_StiffnessMatrix     << std::endl;

  std::cout << "Number of Columns = " << m_StiffnessMatrix.cols() << std::endl;
  std::cout << "Number of Rows    = " << m_StiffnessMatrix.rows() << std::endl;


}



void
FEMHeatConduction2D
::InitializeStiffnessMatrix(void)
{
  const unsigned int numberOfPoints = m_Mesh->GetNumberOfPoints();
  m_StiffnessMatrix = StiffnessMatrixType( numberOfPoints, numberOfPoints ); 
  m_StiffnessMatrix.fill( NumericTraits< RealType >::Zero );
}




}} // end namespace itk::fem





