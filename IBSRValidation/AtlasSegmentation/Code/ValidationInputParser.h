/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ValidationInputParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ValidationInputParser_h
#define _ValidationInputParser_h

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include "itkArray.h"

namespace itk
{

/** \class ValidationInputParser
 *
 * This component parse an input parameter file for a simple 
 * atlas based segmentation application using the IBSR 
 * dataset.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - path to the IBSR "20Normals_T1" directory
 *  - path to the IBSR "20Normals_T1_brain" directory
 *  - altas patient ID
 *  - atlas starting slice number
 *  - number of atlas patient slices
 *  - subject patient ID 
 *  - subject starting slice number
 *  - number of subject patient slices
 *  - the algorithm parameter filename
 *
 * 
 * Outputs:
 *  - pointer to the subject (fixed) image
 *  - pointer to the atlas (moving) image
 *  - pointer to the altas label image
 *  - pointer to the subject label image
 *
 *  - the number of histogram levels to use
 *  - the number of histogram match points to use
 *  
 *  - the number of levels
 *  - the number of iterations at each level
 *
 *  - the fixed image starting shrink factors
 *  - the moving image starting shrink factors
 *
 */
template <typename TImage, typename TLabelImage>
class ITK_EXPORT ValidationInputParser : public Object
{
public:

  /** Standard class typedefs. */
  typedef ValidationInputParser Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ValidationInputParser, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer ImagePointer;
  typedef TLabelImage LabelImageType;
  typedef typename LabelImageType::Pointer LabelImagePointer;

  /** Image dimension enumeration. */
  enum { ImageDimension = TImage::ImageDimension };
 
  /** ShrinkFactors type. */
  typedef FixedArray<unsigned int, ImageDimension> ShrinkFactorsType;

  /** IterationArray type. */
  typedef Array<unsigned int> IterationsArrayType;

  /** Set IBSR images directory. */
  itkSetStringMacro( ImageDirectoryName );

  /** Set IBSR brain ground truth directory. */
  itkSetStringMacro( BrainSegmentationDirectoryName );

  /** Set the atlas patient. */
  itkSetStringMacro( AtlasPatientID );

  /** Set the atlas patient starting slice number. */
  itkSetMacro( AtlasStartSliceNumber, signed long );

  /** Set the number of atlas patient slices. */
  itkSetMacro( AtlasNumberOfSlices, unsigned long );

  /** Set the subject patient. */
  itkSetStringMacro( SubjectPatientID );

  /** Set the subject patient starting slice number. */
  itkSetMacro( SubjectStartSliceNumber, signed long );

  /** Set the number of subject patient slices. */
  itkSetMacro( SubjectNumberOfSlices, unsigned long );

  /** Set input parameter file name. */
  itkSetStringMacro( ParameterFileName );

  /** Parse the input file. */
  void Execute();

  /** Get pointer to the atlas image. */
  itkGetObjectMacro( AtlasImage, ImageType );

  /** Get pointer to the subject image. */
  itkGetObjectMacro( SubjectImage, ImageType );

  /** Get pointer to the atlas label image. */
  itkGetObjectMacro( AtlasLabelImage, LabelImageType );

  /** Get pointer to the subject label image. */
  itkGetObjectMacro( SubjectLabelImage, LabelImageType );

  /** Get the number of histogram bins. */
  itkGetConstMacro( NumberOfHistogramLevels, unsigned long );

  /** Get the number of match points. */
  itkGetConstMacro( NumberOfMatchPoints, unsigned long );
 
  /** Get the number of levels. */
  itkGetMacro( NumberOfLevels, unsigned short );

  /** Get the atlas image starting shrink factors. */
  itkGetConstReferenceMacro( AtlasImageShrinkFactors, ShrinkFactorsType );

  /** Get the subject image starting shrink factors. */
  itkGetConstReferenceMacro( SubjectImageShrinkFactors, ShrinkFactorsType );

  /** Get the number of iterations at each level. */
  itkGetConstReferenceMacro( NumberOfIterations, IterationsArrayType );

protected:
  ValidationInputParser();
  ~ValidationInputParser(){};

  void ReadImage( const char *, signed long, unsigned long, ImagePointer& );
  void ReadLabelImage( const char *, unsigned long, LabelImagePointer& );

private:
  ValidationInputParser( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string                   m_ImageDirectoryName;
  std::string                   m_BrainSegmentationDirectoryName;
  std::string                   m_AtlasPatientID;
  std::string                   m_SubjectPatientID;
  signed long                   m_AtlasStartSliceNumber;
  signed long                   m_SubjectStartSliceNumber;
  unsigned long                 m_AtlasNumberOfSlices;
  unsigned long                 m_SubjectNumberOfSlices;

  std::string                   m_ParameterFileName;

  typename ImageType::Pointer   m_AtlasImage;
  typename ImageType::Pointer   m_SubjectImage;

  typename LabelImageType::Pointer   m_AtlasLabelImage;
  typename LabelImageType::Pointer   m_SubjectLabelImage;

  unsigned long                 m_NumberOfHistogramLevels;
  unsigned long                 m_NumberOfMatchPoints;
  unsigned short                m_NumberOfLevels;
  ShrinkFactorsType             m_AtlasImageShrinkFactors;
  ShrinkFactorsType             m_SubjectImageShrinkFactors;
  IterationsArrayType           m_NumberOfIterations;

};


} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "ValidationInputParser.txx"
#endif

#endif
