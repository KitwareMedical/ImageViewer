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





FEMHeatConduction2D::DisplacementType
FEMHeatConduction2D
::CellEquation(unsigned int i, unsigned int j) const
{
//  the integrand here is :
//   kx dNi/dx dNj/dx + Ky dNi/dy dNj/dy
  DisplacementType sum = NumericTraits< DisplacementType >::Zero;
  return sum;
}




void
FEMHeatConduction2D
::AssembleMasterEquation(void)
{

  typedef MeshType::CellsContainer   CellsContainer;

  // This has to be replaced by a Visitor when
  // the mesh is composed of different cell types
  // e.g. triangles and quads.

  CellsContainer::Pointer   cells = m_Mesh->GetCells();
  if( !cells ) 
    {
    itkGenericExceptionMacro(<<"The Mesh is empty of Cells");
    }

  CellsContainer::Iterator  cell = cells->Begin();
  CellsContainer::Iterator  end  = cells->End();
  while( cell != end )
    {
    cell++;
    }

}





}} // end namespace itk::fem





