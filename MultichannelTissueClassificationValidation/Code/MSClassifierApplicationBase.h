/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MSClassifierApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MSClassifierApplicationBase_h
#define _MSClassifierApplicationBase_h

#include "itkObjectFactory.h"
#include "itkObject.h"

namespace itk
{

/** \class MSClassifierApplicationBase
 * 
 * This class ties together an input parser, a MSClassifier, and an
 * output generator component to form a deformable registration/atlas
 * segmentation application.
 *
 */

template < typename TParser,
           typename TMSClassifier,
           typename TOutputGenerator >
class MSClassifierApplicationBase : public Object
{
public:

  /** Standard class typedefs. */
  typedef MSClassifierApplicationBase Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MSClassifierApplicationBase, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input parser type. */
  typedef TParser ParserType;
  typedef typename ParserType::Pointer ParserPointer;

  /** MSClassifier type. */
  typedef TMSClassifier MSClassifierType;
  typedef typename MSClassifierType::Pointer MSClassifierPointer;

  /** MSClassifier type. */
  typedef TOutputGenerator OutputGeneratorType;
  typedef typename OutputGeneratorType::Pointer OutputGeneratorPointer;

  /** Execute the application. */
  virtual void Execute();

protected:

  MSClassifierApplicationBase();
  virtual ~MSClassifierApplicationBase(){};

  /** Initialize the input parser. */
  virtual void InitializeParser(){};

  /*** Initialize the preprocessor */
  virtual void InitializeMSClassifier(){};

  /*** Initialize the output generator. */
  virtual void InitializeOutputGenerator(){};  

  ParserPointer           m_Parser;
  MSClassifierPointer     m_MSClassifier;
  OutputGeneratorPointer  m_OutputGenerator;

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "MSClassifierApplicationBase.txx"
#endif

#endif
