/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCellInterface.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkCellInterface_txx
#define _itkCellInterface_txx
#include "itkCellInterface.h"

namespace itk
{
namespace fem
{
/**
 * Get the interpolation order of the cell.  Usually linear.
 */
template <typename TCellTraits>
int
CellInterface< TCellTraits >
::GetInterpolationOrder(void) const
{
  return 1;
}
  

/**
 * Get the point id list used by the cell in a form suitable to pass to
 * SetPointIds(first) on another cell.  This is equivalent to
 * PointIdsBegin() const.
 */
template <typename TCellTraits>
CellInterface< TCellTraits >::PointIdConstIterator
CellInterface< TCellTraits >
::GetPointIds(void) const
{
  return this->PointIdsBegin();
}
 
  
/**
 * By default, a cell is not a boundary.
 */
template <typename TCellTraits>
bool
CellInterface< TCellTraits >
::IsBoundary(void)
{
  return false;
}


/**
 * This is only part of the boundary interface.  The call is ignored.
 */
template <typename TCellTraits>
void
CellInterface< TCellTraits >
::AddUsingCell(CellIdentifier)
{
}


/**
 * This is only part of the boundary interface.  The call is ignored.
 */
template <typename TCellTraits>
void
CellInterface< TCellTraits >
::RemoveUsingCell(CellIdentifier)
{
}


/**
 * By default, the cell is not a boundary, so it has no using cells.
 * This will always return false for a cell.
 */
template < typename TCellTraits>
bool
CellInterface< TCellTraits >
::IsUsingCell(CellIdentifier)
{
  return false;
}


/**
 * This is only part of the boundary interface.  The call is ignored.
 */
template < typename TCellTraits>
int
CellInterface< TCellTraits >
::GetNumberOfUsingCells(void)
{
  return 0;
}


/**
 * This is only part of the boundary interface.  The call is ignored.
 */
template <typename TCellTraits>
CellInterface< TCellTraits >::UsingCellsContainerIterator
CellInterface< TCellTraits >
::UsingCellsBegin(void)
{
  return UsingCellsContainerIterator();
}


/**
 * This is only part of the boundary interface.  The call is ignored.
 */
template <typename TCellTraits>
CellInterface< TCellTraits >::UsingCellsContainerIterator
CellInterface< TCellTraits >
::UsingCellsEnd(void)
{
  return UsingCellsContainerIterator();
}

} // end namespace fem
} // end namespace itk

#endif
