/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ApplicationBase_h
#define _ApplicationBase_h

#include "itkObjectFactory.h"
#include "itkObject.h"

namespace itk
{

/** \class ApplicationBase
 * 
 * This class ties together an input parser, a preprocessor,
 * a registrator, a labeler and an output generator component to 
 * form a deformable registration/atlas segmentation application.
 *
 */
template < typename TParser,
           typename TPreprocessor,
           typename TRegistrator,
           typename TLabeler,
           typename TGenerator >
class ApplicationBase : public Object
{
public:

  /** Standard class typedefs. */
  typedef ApplicationBase Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MIMApplication, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input parser type. */
  typedef TParser ParserType;
  typedef typename ParserType::Pointer ParserPointer;

  /** Preprocessor type. */
  typedef TPreprocessor PreprocessorType;
  typedef typename PreprocessorType::Pointer PreprocessorPointer;

  /** Registrator type. */
  typedef TRegistrator RegistratorType;
  typedef typename RegistratorType::Pointer RegistratorPointer;

  /** Labeler type. */
  typedef TLabeler LabelerType;
  typedef typename LabelerType::Pointer LabelerPointer;

  /** Output generator type. */
  typedef TGenerator GeneratorType;
  typedef typename GeneratorType::Pointer GeneratorPointer;

  /** Execute the application. */
  virtual void Execute();

protected:

  ApplicationBase();
  virtual ~ApplicationBase(){};

  /** Initialize the input parser. */
  virtual void InitializeParser(){};

  /*** Initialize the preprocessor */
  virtual void InitializePreprocessor(){};

  /*** Initialize the registrator  */
  virtual void InitializeRegistrator(){};

  /*** Initialize the labeler */
  virtual void InitializeLabeler(){};

  /*** Initialize the output generator. */
  virtual void InitializeGenerator(){};

  ParserPointer           m_Parser;
  PreprocessorPointer     m_Preprocessor;
  RegistratorPointer      m_Registrator;
  LabelerPointer          m_Labeler;
  GeneratorPointer        m_Generator;
 

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "ApplicationBase.txx"
#endif

#endif
