/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SimpleAppInputParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SimpleAppInputParser_h
#define _SimpleAppInputParser_h

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include "itkArray.h"

namespace itk
{

/** \class SimpleAppInputParser
 *
 * This component parse an input parameter file for a simple MI registration
 * application.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - name of parameter file
 *
 * Outputs:
 *  - pointer to the fixed image
 *  - pointer to the moving image
 *  - moving image permutation order
 *  - boolean array indicating which axes to flip after permutation
 *  
 *  - the number of levels
 *  - the fixed image starting shrink factors
 *  - the moving image starting shrink factors
 *  - the number of iterations at each level
 *  - the learning rate at each level
 *  - scaling factor applied to translation parameters during optimization
 *
 *  - directory where to write the PGM files
 */
template <typename TImage>
class ITK_EXPORT SimpleAppInputParser : public Object
{
public:

  /** Standard class typedefs. */
  typedef SimpleAppInputParser Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SimpleAppInputParser, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;

  /** Image dimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);
 
  /** ShrinkFactors type. */
  typedef FixedArray<unsigned int, itkGetStaticConstMacro(ImageDimension)> ShrinkFactorsType;

  /** Spacing type. */
  typedef FixedArray<double, itkGetStaticConstMacro(ImageDimension)> SpacingType;

  /** IterationArray type. */
  typedef Array<unsigned int> IterationsArrayType;

  /** RateArray type. */
  typedef Array<double> RatesArrayType;

  /** PermuteOrder type. */
  typedef FixedArray<unsigned int, itkGetStaticConstMacro(ImageDimension)> PermuteOrderType;

  /** FlipAxes type. */
  typedef FixedArray<bool, itkGetStaticConstMacro(ImageDimension)> FlipAxesType;

  /** Set input parameter file name. */
  itkSetStringMacro( ParameterFileName );

  /** Parse the input file. */
  void Execute();

  /** Get pointer to the fixed image. */
  itkGetObjectMacro( FixedImage, ImageType );

  /** Get pointer to the moving image. */
  itkGetObjectMacro( MovingImage, ImageType );
 
  /** Get axes permute order. */
  itkGetConstReferenceMacro( PermuteOrder, PermuteOrderType );

  /** Get flip axes. */
  itkGetConstReferenceMacro( FlipAxes, FlipAxesType );

  /** Get the number of levels. */
  itkGetMacro( NumberOfLevels, unsigned short );

  /** Get the fixed image starting shrink factors. */
  itkGetConstReferenceMacro( FixedImageShrinkFactors, ShrinkFactorsType );

  /** Get the moving image starting shrink factors. */
  itkGetConstReferenceMacro( MovingImageShrinkFactors, ShrinkFactorsType );

  /** Get the number of iterations at each level. */
  itkGetConstReferenceMacro( NumberOfIterations, IterationsArrayType );

  /** Get the learning rate at each level. */
  itkGetConstReferenceMacro( LearningRates, RatesArrayType );

  /** Get the translation parameters scale. */
  itkGetConstMacro( TranslationScale, double );
 
  /** Get the output PGM directory. */
  itkGetStringMacro( PGMDirectoryName );

protected:
  SimpleAppInputParser();
  ~SimpleAppInputParser(){};

private:
  SimpleAppInputParser( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string                   m_ParameterFileName;
  typename ImageType::Pointer   m_FixedImage;
  typename ImageType::Pointer   m_MovingImage;
  PermuteOrderType              m_PermuteOrder;
  FlipAxesType                  m_FlipAxes;
  unsigned short                m_NumberOfLevels;
  ShrinkFactorsType             m_FixedImageShrinkFactors;
  ShrinkFactorsType             m_MovingImageShrinkFactors;
  IterationsArrayType           m_NumberOfIterations;
  RatesArrayType                m_LearningRates;
  double                        m_TranslationScale;
  std::string                   m_PGMDirectoryName;

};


} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "SimpleAppInputParser.txx"
#endif

#endif
