/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ClassifierValidationOutput.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ClassifierValidationOutput_h
#define _ClassifierValidationOutput_h

#include "itkObject.h"
#include <string>
#include "RawVolumeWriter.h"
#include "itkArray.h"


namespace itk
{

/** \class ClassificationValidationOutput
 *
 * This component takes as input a label image,
 * and label ground truth image. It computes the overlap between the
 * warped atlas and the ground truth and writes the overlap
 * value to file.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - pointer to the classified image
 *  - number of class labels in the classified image
 *  - pointer to the classified image ground truth
 *  - name of the output file
 *  - flag to indicate whether or not to append to output file
 *  - patient id number
 *
 * A threshold filter is used to extract each class label
 * in the multi-label classified and the truth image. This 
 * thresholding is 
 * The lower and upper thresholds are used to extract one
 * label out of a  image. For example, if required
 * segmentation is represented by pixels labeled 5 then
 * one should set both the lower and upper threshold to 5.
 * If required segmentation is represented by all non-zero
 * pixels the lower threshold should be set to 1 and the
 * the upper to NumericTraits<PixelType>::max().
 *
 */
template <typename TImage>
class ITK_EXPORT ClassifierValidationOutput : public Object
{
public:

  /** Standard class typedefs. */
  typedef ClassifierValidationOutput Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClassifierValidationOutput, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  typedef typename ImageType::PixelType PixelType;
  typedef typename ImageType::SizeType SizeType;
  typedef typename ImageType::IndexType IndexType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::Pointer  ImagePointer;
  typedef Array<unsigned int>  IntegerArrayType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TImage::ImageDimension };

  /** Set the label image. */
  itkSetObjectMacro( ClassifiedImage, ImageType );

  /** Set the starting slice number of the segmentation file. */
  itkSetMacro( StartSegSliceNumber, signed long );

  /** Set the number of slices in the segmentation image. */
  itkSetMacro( NumberOfSegSlices, unsigned long );

  /** Set the lower threshold for the label image. */
  itkSetMacro( ClassifiedImageLowerThreshold, PixelType );

  /** Set the upper threshold for the label image. */
  itkSetMacro( ClassifiedImageUpperThreshold, PixelType );

  /** Set the label image ground truth. */
  itkSetObjectMacro( TruthImage, ImageType );

  /** Set the lower threshold for the ground truth. */
  itkSetMacro( TruthImageLowerThreshold, PixelType );

  /** Set the upper threshold for the ground truth. */
  itkSetMacro( TruthImageUpperThreshold, PixelType );

  /** Set the tissue labels in the truth data set */
  itkSetMacro( TruthLabels, IntegerArrayType );

  /** Set output file name. */
  itkSetStringMacro( OutputFileName );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

  /** Set patient number. */
  itkSetStringMacro( PatientID );

  /** Activiate the process. */
  void Execute();
  

protected:
  ClassifierValidationOutput();
  ~ClassifierValidationOutput(){};


private:
  ClassifierValidationOutput( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  ImagePointer     m_ClassifiedImage;
  ImagePointer     m_TruthImage;

  signed long      m_StartSegSliceNumber;
  unsigned long    m_NumberOfSegSlices;

  PixelType        m_ClassifiedImageLowerThreshold;
  PixelType        m_ClassifiedImageUpperThreshold;

  PixelType        m_TruthImageLowerThreshold;
  PixelType        m_TruthImageUpperThreshold;

  SizeType         m_LowerBoundaryCropSize;
  SizeType         m_UpperBoundaryCropSize;

  IntegerArrayType m_TruthLabels;

  unsigned long    m_Volume1;
  unsigned long    m_Volume2;
  double           m_SimilarityIndex;

  std::string      m_OutputFileName;
  bool             m_AppendOutputFile;

  std::string      m_PatientID;

  /** Run the crop image to match up the classified and the truth image */
  void ExtractSubImage( ImagePointer & image );

  /** Threshold the images and get the statistics measured */
  void GetMeasuredStatistics(unsigned int);


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "ClassifierValidationOutput.txx"
#endif

#endif
