/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMElement_h
#define __itkFEMElement_h

#include "itkFEMElementBase.h"
#include "vnl/vnl_matrix.h"


namespace itk {
namespace fem {


/** 
 * \brief Base class for Elements in a FEM system.
 * 
 */
template <    typename TCell, typename TFEMMesh > 
class FEMElement : public FEMElementBase< TFEMMesh >
{

public:

  typedef FEMElementBase<TFEMMesh>                  ElementBaseType;
  typedef TFEMMesh                                  FEMMeshType;
  typedef typename FEMMeshType::CellTraits          CellTraits;

  
  // note that we cannot use "CellType" here because there 
  // is already an enum called "CellType" defined in CellInterface<>
  typedef TCell                                     BaseCellType;
  typedef FEMElementBase< TFEMMesh >                Superclass;

  typedef typename FEMMeshType::PointsContainer     PointsContainer;
  typedef typename FEMMeshType::PointDataContainer  PointDataContainer;
  typedef typename FEMMeshType::CellsContainer      CellsContainer;
  typedef typename FEMMeshType::PointIdentifier     PointIdentifier;
  

  /** Return a reference to the internal cell of the Element 
      Note that the Element *has* a Cell as an ivar 
      \sa GetCellPointer   */
  BaseCellType & GetCell(void) 
      { return m_Cell; }

  const BaseCellType & GetCell(void) const
      { return m_Cell; }

  /** Return a pointer to the internal cell of the Element 
      Note that the Element *has* a Cell as an ivar not just a pointer    
      \sa GetCell                                  */
  const BaseCellType * GetCellPointer(void) const
      { return & m_Cell; }

protected:

  BaseCellType        m_Cell;


};




}} // end namespace itk::fem

#endif 



