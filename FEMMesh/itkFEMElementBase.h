/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMElementBase_h
#define __itkFEMElementBase_h


#include "vnl/vnl_matrix.h"


namespace itk {
namespace fem {


/** 
 * \brief Base class for Elements in a FEM system.
 * 
 */
template < typename TFEMMesh >
class FEMElementBase 
{

public:

  /** Standard class typedefs. */
  typedef FEMElementBase            Self;
  typedef LightObject               Superclass;
//  typedef SmartPointer<Self>        Pointer;
//  typedef SmartPointer<const Self>  ConstPointer;
  typedef        Self  *   Pointer;
  typedef  const Self  *   ConstPointer;


  /** \warning This is an Abstract class it is not expected to
      be instantiated. It should not have a New() macro  */


  /** Run-time type information (and related methods). */
 itkTypeMacro(Image, ImageBase);
 
  // this type is used to represent components of the load
  // and the Stiffness matrix
  typedef float       RealType;

  /** Type for representing the Stiffnes and Mass matrices  */
  typedef  vnl_matrix< RealType >       MatrixType;

  /** Type for representing sets of external loads */
  typedef  vnl_vector< RealType >       LoadsVectorType;

  /** Type of the displacement field at each point */
  typedef TFEMMesh                                  FEMMeshType;
  typedef typename FEMMeshType::PixelType           DisplacementType;

  /**
   * Return the number of components of the Displacement field
   * also known as degrees of freedom (DOF) for a derived element class
   */
  virtual unsigned int GetNumberOfDisplacementComponents( void ) const = 0;


  /**
   *  The Cell return the displacement associated with the ith point
   *  of its list of points
   */
  virtual const DisplacementType & 
     GetDisplacement(unsigned int i, FEMMeshType *mesh ) const = 0;

  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual MatrixType GetStiffnessMatrix( const FEMMeshType * ) const = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  virtual MatrixType GetMassMatrix( void ) const 
    { return MatrixType(); }


  /** This must be implemented by all sub-classes of CellInterface */
  typedef typename FEMMeshType::CellMultiVisitorType    CellMultiVisitorType;
  virtual void Accept(unsigned long cellId, CellMultiVisitorType * )= 0; 
  

protected:

  /** Constructor */
  FEMElementBase() {}

  /** Virtual Destructor */
  virtual ~FEMElementBase() {};



};




}} // end namespace itk::fem

#endif 



