/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackImageContainer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVispackImageContainer_h
#define __itkVispackImageContainer_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkSmartPointer.h"

#include <utility>
#include "image/image.h"

namespace itk
{

/** \class VispackImageContainer
 * Defines a front-end to the vispack VISImRep object, which serves as the
 * reference counted data container for both VISImage and VISVolume.
 *  This is a full-fleged Object, so there is modification time, debug, and
 * reference count information. 
 *
 * Template parameters for VispackImageContainer:
 *
 * TElement =
 *    The element type stored in the container.
 */
  
template < typename TElement >
class VispackImageContainer:   public Object,  private VISImRep<TElement>
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef VispackImageContainer     Self;
  
  /**
   * Standard "Superclass" typedef.
   */
  typedef Object  Superclass;

  /**
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /**
   * Save the template parameters.
   */
  typedef unsigned int        ElementIdentifier;
  typedef TElement            Element;
  
private:
  /**
   * Quick access to the vispack data type that was inherited.
   */
  typedef VISImRep<Element>  VispackType;
  
protected:
  /**
   * Provide pass-through constructors corresponding to all the VISImRep
   * constructors.  These are for internal use only since
   * this is also an Object which must be constructed through the
   * "New()" routine.
   */
  VispackImageContainer():  VispackType() {}
  
  VispackImageContainer(unsigned int size):  VispackType(size) {}
  
  VispackImageContainer(VispackType *r):  VispackType(r) {}
  
  VispackImageContainer(unsigned int size, Element *buffer):
    VispackType(size, buffer) {}
  
public:
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Index operator. This version can be an lvalue.
   */
  TElement & operator[](const ElementIdentifier id)
    { return this->VispackType::at(id); };

  /**
   * Index operator. This version can only be an rvalue
   */
  const TElement & operator[](const ElementIdentifier id) const
    { return this->VispackType::operator[](id); };
    
  /**
   * Return a pointer to the beginning of the buffer.  This is used by
   * the image iterator class.
   */
  TElement *GetBufferPointer()
    { return &(this->VispackType::buffer()); };
  
  /**
   * Get the number of elements currently stored in the container.
   */
  unsigned long Size(void) const
    { return this->VispackType::size(); };

  /**
   * Tell the container to allocate enough memory to allow at least
   * as many elements as the size given to be stored.  This is NOT
   * guaranteed to actually allocate any memory, but is useful if the
   * implementation of the container allocates contiguous storage.
   */
  void Reserve(ElementIdentifier num)
    { this->VispackType::initialize(num, 0); };
  
  /**
   * Tell the container to try to minimize its memory usage for storage of
   * the current number of elements.  This is NOT guaranteed to decrease
   * memory usage.
   */
  void Squeeze(void)  { /* not supported by vispack */ };
  
  /**
   * Standard part of every itk Object.
   */
  itkTypeMacro(VispackImageContainer, Object);

  /**
   * Set this object's vispack component equal to a vispack container.
   * NOTE: This could probably be modified to preserve reference counting
   * in the original vispack image
   */
  const Self &operator= (const VispackType& o )
  {
    this->VispackType::initialize(o.size(), 0);
    this->VispackType::copy(&o);
    return *this;
  }
  
};

} // end namespace itk
  
#endif
