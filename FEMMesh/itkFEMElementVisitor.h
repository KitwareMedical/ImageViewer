/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementVisitor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMElementVisitor_h
#define __itkFEMElementVisitor_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

namespace itk
{

namespace fem
{

// forward reference FEMElementBase
template < typename TFEMMesh > class FEMElementBase;
  

/** \class FEMElementVisitor
 * Define the abstract interface for a visitor class that can visit the
 * elements in a FEMMesh.  This follows the Visitor Design Pattern.   
 * To make this class easier to use, the FEMElementVisitorImplementation  
 * is provided as a templated class to implement the pure virtual functions
 * of FEMElementVisitor.
 *
 * \ingroup MeshAccess 
 */
template <
  typename TFEMMesh
>  
class FEMElementVisitor : public LightObject
{
public: 
  /** Standard class typedefs. */
  typedef FEMElementVisitor       Self;
  typedef LightObject  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
    
  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMElementVisitor,LightObject);

  typedef FEMElementBase< TFEMMesh >  ElementBaseType;

  /** This method is called by each Element as it visits this visitor. */
  virtual void VisitFromElement(unsigned long elementId, ElementBaseType * ) = 0;

  /**  Return the index of the ElementTopology. */
  virtual int GetElementTopologyId() = 0;
  
protected:
  FEMElementVisitor() {};
  ~FEMElementVisitor() {};

private:
  FEMElementVisitor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

  
/** \class FEMElementVisitorImplementation
 * A template class used to implement a visitor object.
 *
 * The Visitor implementation does the down cast to 
 * the specific element type that is being visited.  After the
 * cast, a member of the UserVisitor type called Visit is
 * passed the exact element type being visited.  To use this
 * class, write a class that implements a function 
 * Visit(int id, ElementTopology*). Then, use that as the UserVisitor
 * template parameter.
 *
 * Template parameters for FEMElementVisitorImplementation:
 * TPixelType = see FEMElement
 *
 * TElementTraits = see FEMElement
 *
 * ElementTopology = The specific type of element that needs to be visited.
 *
 * UserVisitor = A user supplied class that implements the function
 *               Visit(int id, ElementTopology*)
 *
 * \ingroup MeshAccess 
 */
template<
  typename TFEMMesh,
  class ElementTopology,
  class UserVisitor 
>
class FEMElementVisitorImplementation : 
    public FEMElementVisitor< TFEMMesh >, public UserVisitor
{
public:
  /** Standard class typedefs. */
  typedef FEMElementVisitorImplementation       Self;
  typedef SmartPointer<Self>  Pointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMElementVisitorImplementation,LightObject);
  
  /** Call the static method GetTopologyId for the ElementTopology type that
   * we are templated over. */
  virtual int GetElementTopologyId() { return ElementTopology::GetTopologyId();}

  typedef FEMElementBase< TFEMMesh >  ElementBaseType;

  /** Call the method Visit from the UserVisitor template parameter that
   * this class inherits from.  I am my own gradpa... */
  void VisitFromElement(unsigned long elementId, ElementBaseType * c)
    {
    this->UserVisitor::Visit(elementId, static_cast< ElementTopology * >( c ) );
    }

protected:
  FEMElementVisitorImplementation() {};
  ~FEMElementVisitorImplementation() {};

private:
  FEMElementVisitorImplementation(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};

} // end namespace fem
} // end namespace itk

#endif
