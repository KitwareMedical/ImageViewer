/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMMesh.txx
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

#ifndef _itkFEMMesh_txx
#define _itkFEMMesh_txx

#include "itkFEMMesh.h"


namespace itk {
namespace fem {


template< unsigned int NPointDimension >
FEMMesh<NPointDimension>
::FEMMesh()
{

            
}


template< unsigned int NPointDimension >
FEMMesh<NPointDimension>
::~FEMMesh()
{

            
}



 
template < unsigned int NPointDimension >
void
FEMMesh<NPointDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


 
template < unsigned int NPointDimension >
unsigned long
FEMMesh<NPointDimension>
::GetNumberOfPoints(void) const
{
  return m_Points.size();
}



 
template < unsigned int NPointDimension >
unsigned long
FEMMesh<NPointDimension>
::GetNumberOfCells(void) const
{
  return m_Cells.size();
}




 
template < unsigned int NPointDimension >
unsigned long
FEMMesh<NPointDimension>
::GetNumberOfNodes(void) const
{
  return m_Nodes.size();
}




 
template < unsigned int NPointDimension >
unsigned long
FEMMesh<NPointDimension>
::GetNumberOfElements(void) const
{
  return m_Elements.size();
}



 
template < unsigned int NPointDimension >
void
FEMMesh<NPointDimension>
::AddPoint( PointType * p ) 
{
  m_Points.push_back( p );
}



 
template < unsigned int NPointDimension >
void
FEMMesh<NPointDimension>
::AddCell( CellType * p ) 
{
  m_Cells.push_back( p );
}



 
template < unsigned int NPointDimension >
void
FEMMesh<NPointDimension>
::AddElement( ElementType * p ) 
{
  m_Elements.push_back( p );
}



 
template < unsigned int NPointDimension >
void
FEMMesh<NPointDimension>
::AddNode( NodeType * p ) 
{
  m_Nodes.push_back( p );
}








}} // end namespace itk::fem


#endif /* #ifndef _itkFEMMesh_txx */



