/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ValidationOutput.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ValidationOutput_h
#define _ValidationOutput_h

#include "itkObject.h"
#include <string>

namespace itk
{

/** \class ValidationOutput
 *
 * This component takes as input a label image,
 * and label ground truth image. It computes the overlap between the
 * warped atlas and the ground truth and writes the overlap
 * value to file.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - pointer to the label image
 *  - lower threshold for label image
 *  - upper threshold for label image
 *  - pointer to the label image ground truth
 *  - lower threshold for label ground truth
 *  - upper threshold for label ground truth
 *  - name of the output file
 *  - flag to indicate whether or not to append to output file
 *  - patient id number
 *
 * The lower and upper thresholds are used to extract one
 * label out of a multi-label image. For example, if required
 * segmentation is represented by pixels labeled 5 then
 * one should set both the lower and upper threshold to 5.
 * If required segmentation is represented by all non-zero
 * pixels the lower threshold should be set to 1 and the
 * the upper to NumericTraits<PixelType>::max().
 *
 */
template <typename TImage>
class ITK_EXPORT ValidationOutput : public Object
{
public:

  /** Standard class typedefs. */
  typedef ValidationOutput Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ValidationOutput, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  typedef typename ImageType::PixelType PixelType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TImage::ImageDimension };

  /** Set the label image. */
  itkSetObjectMacro( LabelImage, ImageType );

  /** Set the lower threshold for the label image. */
  itkSetMacro( LabelLowerThreshold, PixelType );

  /** Set the upper threshold for the label image. */
  itkSetMacro( LabelUpperThreshold, PixelType );

  /** Set the label image ground truth. */
  itkSetObjectMacro( GroundTruthImage, ImageType );

  /** Set the lower threshold for the ground truth. */
  itkSetMacro( GroundTruthLowerThreshold, PixelType );

  /** Set the upper threshold for the ground truth. */
  itkSetMacro( GroundTruthUpperThreshold, PixelType );

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
  ValidationOutput();
  ~ValidationOutput(){};

private:
  ValidationOutput( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename ImageType::Pointer   m_LabelImage;
  typename ImageType::Pointer   m_GroundTruthImage;

  PixelType                     m_LabelLowerThreshold;
  PixelType                     m_LabelUpperThreshold;

  PixelType                     m_GroundTruthLowerThreshold;
  PixelType                     m_GroundTruthUpperThreshold;

  std::string                   m_OutputFileName;
  bool                          m_AppendOutputFile;

  std::string                   m_PatientID;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "ValidationOutput.txx"
#endif

#endif
