/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ClassifierApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ClassifierApplicationBase_h
#define _ClassifierApplicationBase_h

#include "itkObjectFactory.h"
#include "itkObject.h"

namespace itk
{

/** \class ClassifierApplicationBase
 * 
 * This class ties together an input parser, a Classifier,
 * a registrator, a labeler and an output generator component to 
 * form a deformable registration/atlas segmentation application.
 *
 */
/*
template < typename TParser,
           typename TClassifier,
           typename TRegistrator,
           typename TLabeler,
           typename TGenerator >
class ClassifierApplicationBase : public Object
*/
template < typename TParser,
           typename TClassifier,
           typename TOutputGenerator >
class ClassifierApplicationBase : public Object
{
public:

  /** Standard class typedefs. */
  typedef ClassifierApplicationBase Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClassifierApplicationBase, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input parser type. */
  typedef TParser ParserType;
  typedef typename ParserType::Pointer ParserPointer;

  /** Classifier type. */
  typedef TClassifier ClassifierType;
  typedef typename ClassifierType::Pointer ClassifierPointer;

  /** Classifier type. */
  typedef TOutputGenerator OutputGeneratorType;
  typedef typename OutputGeneratorType::Pointer OutputGeneratorPointer;

  /** Registrator type. */
//  typedef TRegistrator RegistratorType;
//  typedef typename RegistratorType::Pointer RegistratorPointer;

  /** Labeler type. */
//  typedef TLabeler LabelerType;
//  typedef typename LabelerType::Pointer LabelerPointer;

  /** Execute the application. */
  virtual void Execute();

protected:

  ClassifierApplicationBase();
  virtual ~ClassifierApplicationBase(){};

  /** Initialize the input parser. */
  virtual void InitializeParser(){};

  /*** Initialize the preprocessor */
  virtual void InitializeClassifier(){};

  /*** Initialize the output generator. */
  virtual void InitializeOutputGenerator(){};  

/*** Initialize the registrator  */
//  virtual void InitializeRegistrator(){};

  /*** Initialize the labeler */
//  virtual void InitializeLabeler(){};



  ParserPointer           m_Parser;
  ClassifierPointer       m_Classifier;
  OutputGeneratorPointer  m_OutputGenerator;
//  RegistratorPointer      m_Registrator;
//  LabelerPointer          m_Labeler;
//  GeneratorPointer        m_Generator;
 

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "ClassifierApplicationBase.txx"
#endif

#endif
