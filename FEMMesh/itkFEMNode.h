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

#include "itkArray.h"


namespace itk {
namespace fem {



/**
 * \class FEMNode
 * \brief Base class for Nodes in a mesh which is used for finite element modeling.
 *
 * Every point in finite element mesh should also define the degrees of freedom
 * that exist at that point. A point with the degrees of freedom defined will
 * be refered to as a FEM Node.
 *
 * Derived class should define a function that creates and initializes a
 * proper FEM Node object. 
 * 
 * By using different derived classes in a mesh as a Node types 
 * it is possible to create different FEM Nodes in a system.
 *
 * Note that Nodes are not necessarily associated one-to-one with the Points
 * at the geometrical level. This is due to the fact that the Cells representing
 * the geometry of the element can use a different order than the one used for
 * interpolating the degrees of freedom.
 *
 */
class FEMNode 
{

public:


  // Type used for representing the deegrees of freedom.
  // This is defined in order to provide a common API for the 
  // FEM solver which will only receive arrays of this type 
  typedef  float  DisplacementType;
  typedef  float  FloatType;

  typedef Array< DisplacementType >   DisplacementContainerType;

  virtual unsigned int GetNumberOfDegreesOfFreedom(void) const = 0;

  const DisplacementContainerType & GetDisplacements( void ) const
    { return m_Displacements; }

  FEMNode()  {}            /** default constructor */
  virtual ~FEMNode() {}    /** virtual destructor */

  
protected:

  DisplacementContainerType m_Displacements; 

};





}} // end namespace itk::fem

#endif 



