/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMValidationInputParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMValidationInputParser_h
#define _FEMValidationInputParser_h

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include "itkArray.h"

namespace itk
{

/** \class FEMValidationInputParser
 *
 * This component parse an input parameter file for a simple fem
 * atlas based segmentation application using the IBSR 
 * dataset.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - path to the IBSR "20Normals_T1" directory
 *  - path to the IBSR "20Normals_T1_brain" directory
 *  - atlas patient ID
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
 *  - pointer to the atlas label image
 *  - pointer to the subject label image
 *
 *  - the number of histogram levels to use
 *  - the number of histogram match points to use
 *
 */
template <typename TImage, typename TLabelImage>
class ITK_EXPORT FEMValidationInputParser : public Object
{
public:

  /** Standard class typedefs. */
  typedef FEMValidationInputParser Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMValidationInputParser, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer ImagePointer;
  typedef TLabelImage LabelImageType;
  typedef typename LabelImageType::Pointer LabelImagePointer;

  /** Image dimension enumeration. */
  enum { ImageDimension = TImage::ImageDimension };
 
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


  /** Set the size of the x-dimension of the image */
  itkSetMacro( ImageXSize, unsigned int );
  itkGetMacro( ImageXSize, unsigned int );

  /** Set the size of the y-dimension of the image */
  itkSetMacro( ImageYSize, unsigned int );
  itkGetMacro( ImageYSize, unsigned int );

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
 
protected:
  FEMValidationInputParser();
  ~FEMValidationInputParser(){};

  void ReadImage( const char *, signed long, unsigned long, ImagePointer& );
  void ReadLabelImage( const char *, unsigned long, LabelImagePointer& );

private:
  FEMValidationInputParser( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string                   m_ImageDirectoryName;
  std::string                   m_BrainSegmentationDirectoryName;
  std::string                   m_AtlasPatientID;
  std::string                   m_SubjectPatientID;
  signed long                   m_AtlasStartSliceNumber;
  signed long                   m_SubjectStartSliceNumber;
  unsigned long                 m_AtlasNumberOfSlices;
  unsigned long                 m_SubjectNumberOfSlices;
  unsigned int                  m_ImageXSize;
  unsigned int                  m_ImageYSize;

  std::string                   m_ParameterFileName;

  typename ImageType::Pointer   m_AtlasImage;
  typename ImageType::Pointer   m_SubjectImage;

  typename LabelImageType::Pointer   m_AtlasLabelImage;
  typename LabelImageType::Pointer   m_SubjectLabelImage;

  unsigned long                 m_NumberOfHistogramLevels;
  unsigned long                 m_NumberOfMatchPoints;

};


} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "FEMValidationInputParser.txx"
#endif

#endif
