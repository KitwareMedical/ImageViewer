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


template<class TMesh>
FEMMesh<TMesh>
::FEMMesh()
{
  m_ElementsContainer = ElementsContainer::New();
}


template<class TMesh>
FEMMesh<TMesh>
::~FEMMesh()
{

            
}




/**
 * Dynamically build the links from points back to their using cells.  This
 * information is stored in the cell links container, not in the points.
 */
template <typename TMesh>
void
FEMMesh<TMesh>
::Accept( ElementMultiVisitorType* mv)
{
  if(!m_ElementsContainer)
    {
    return;
    }
  for(ElementsContainerIterator i = m_ElementsContainer->Begin();
      i != m_ElementsContainer->End(); ++i)
    {
    if( i->Value() )
      {
      i->Value()->Accept(i->Index(), mv);
      }
    else
      {
      itkDebugMacro("Null cell at " << i->Index());
      }
    }
}





}} // end namespace itk::fem


#endif /* #ifndef _itkFEMMesh_txx */



