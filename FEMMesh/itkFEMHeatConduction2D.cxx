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
#include "itkFEMElementBase.h"


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

   // Here instantiate all the visitors and
   // send them to walk through the mesh

}





}} // end namespace itk::fem





