/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMDisplacementNode.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMDisplacementNode_h
#define __itkFEMDisplacementNode_h

#include "itkFEMNode.h"

namespace itk {
namespace fem {



/**
 * \class FEMDisplacementNode
 * \brief FEMNode with a Displacement Field of N-D components.
 *
 *
 */
template < unsigned int NNumberOfDisplacementComponents >
class FEMDisplacementNode : public FEMNode
{

public:

  
  typedef FEMNode     Superclass;

  /** Type used for representing the Displacement Field Variables
      This is defined in order to provide a common API for the 
      FEM solver which will only receive arrays of this type   */
  typedef  Superclass::DisplacementType   DisplacementType;


  /** Number of Components comforming the Displacement Field
      (also known as the number of Degrees of Freedom)    */
  enum { NumberOfDisplacementComponents = NNumberOfDisplacementComponents };

  /** Container type used for storing the values of the Displacement
      Field Variables. A FixedArray is used here because is has zero
      memory overhead  */
  typedef FixedArray< DisplacementType >  DisplacementContainerType;

  /** Return the number of components of the Displacement Field 
      (also known as the number of Degrees of Freedom */
  virtual unsigned int GetNumberOfDisplacementComponents(void) const 
    { return NumberOfDisplacementComponents; }

  /** Return one of the components of the Displacement field.
   The component is identified by the integer "Id".
   \warning No bound checking is performed in order to improve performance */
  const DisplacementType & GetDisplacement( unsigned int Id ) const 
    { return m_Displacement[ Id ]; }



  FEMDisplacementNode()  {}  /** default constructor */
  virtual ~FEMDisplacementNode() {}    /** virtual destructor */


private:

  DisplacementContainerType     m_Displacement;

};





}} // end namespace itk::fem

#endif 



