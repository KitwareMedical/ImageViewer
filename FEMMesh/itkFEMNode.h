/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMNode.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMNode_h
#define __itkFEMNode_h

#include "itkVertexCell.h"

namespace itk {
namespace fem {



/**
 * \class FEMNode
 * \brief Base class for Nodes in a mesh which is used for finite element modeling.
 *
 * Every point in finite element mesh should also define the degrees of freedom
 * that exist at that point. A point with the degrees of freedom defined will
 * be refered to as a FEM Node.
 * Derived class should define a function that creates and initializes a
 * proper FEM Node object. 
 * 
 * The Node derives from the itk::CellVertex which is naturally associated to
 * a Point in the PointSet level of the Mesh.  The Id used for the node when
 * added to the FEMMesh provides a natural association with the Point having 
 * the same Id at the PointSet level.
 *
 * By using different derived classes in a mesh as a Node types (zero dimensional cells)
 * it is possible to create different FEM Nodes in a system.
 *
 */
template< typename TCellTraits >
class FEMNode : public VertexCell<TCellTraits>
{
public:


protected:
  FEMNode()  {}            /** default constructor */
  virtual ~FEMNode() {}    /** virtual destructor */

};





}} // end namespace itk::fem

#endif 



