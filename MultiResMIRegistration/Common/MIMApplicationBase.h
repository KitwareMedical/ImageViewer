/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIMApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MIMApplicationBase_h
#define _MIMApplicationBase_h

#include "itkObjectFactory.h"
#include "itkObject.h"

namespace itk
{

/** \class MIMApplicationBase
 * 
 * This class ties together an input parser, a preprocessor,
 * a registrator and an output generator component to form a MIM registration
 * application.
 *
 */
template < typename TInputImage,
           typename TImage,
           typename TParser,
           typename TPreprocessor,
           typename TRegistrator,
           typename TGenerator >
class MIMApplicationBase : public Object
{
public:

  /** Standard class typedefs. */
  typedef MIMApplicationBase Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MIMApplication, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input image type. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::PixelType InputPixelType;
  enum { ImageDimension = TInputImage::ImageDimension };

  /** Internal image type. */
  typedef TImage ImageType;
  typedef typename ImageType::PixelType PixelType;

  /** Input parser type. */
  typedef TParser ParserType;
  typedef typename ParserType::Pointer ParserPointer;

  /** Preprocessor type. */
  typedef TPreprocessor PreprocessorType;
  typedef typename PreprocessorType::Pointer PreprocessorPointer;

  /** Registrator type. */
  typedef TRegistrator RegistratorType;
  typedef typename RegistratorType::Pointer RegistratorPointer;

  /** Output generator type. */
  typedef TGenerator GeneratorType;
  typedef typename GeneratorType::Pointer GeneratorPointer;

  /** Affine transform type */
  typedef typename PreprocessorType::AffineTransformType AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  /** Execute the application. */
  virtual void Execute();

protected:

  MIMApplicationBase();
  virtual ~MIMApplicationBase(){};

  /** Initialize the input parser. */
  virtual void InitializeParser(){};

  /*** Initialize the preprocessor */
  virtual void InitializePreprocessor();

  /*** Initialize the registrator  */
  virtual void InitializeRegistrator();

  /*** Initialize the output generator. */
  virtual void InitializeGenerator(){};

  ParserPointer           m_Parser;
  PreprocessorPointer     m_Preprocessor;
  RegistratorPointer      m_Registrator;
  GeneratorPointer        m_Generator;

  AffineTransformPointer  m_Transform;
 

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "MIMApplicationBase.txx"
#endif

#endif
