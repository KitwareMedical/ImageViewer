/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    AtlasLabeler.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _AtlasLabeler_h
#define _AtlasLabeler_h

#include "itkObject.h"
#include "itkVector.h"
#include "itkImage.h"
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"

#include "itkArray.h"

namespace itk
{

/** \class AtlasLabeler
 *
 * This component performs atlas-based labeling using the user
 * specified altas label image and defomation field.
 *
 * The registration process is activated by method Execute().
 *
 * Inputs:
 *   - pointer to atlas label image
 *   - the lower threshold for the atlas label image
 *   - the upper threshold for the atlas label image
 *   - pointer to the deformation field
 *   - the output label value
 *
 * Outputs:
 *   - pointer to compute deformation field
 *
 * The lower and upper thresholds are used to extract one
 * label out of a multi-label image. For example, if required
 * segmentation is represented by pixels labeled 5 then
 * one should set both the lower and upper threshold to 5.
 * If required segmentation is represented by all non-zero
 * pixels the lower threshold should be set to 1 and the
 * the upper to NumericTraits<LabelPixelType>::max().
 *
 */ 
template <
typename TLabelImage, 
typename TDeformationField>
class AtlasLabeler : public Object
{
public:

  /** Standard class typedefs. */
  typedef AtlasLabeler Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(AtlasLabeler, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Label Image Type. */
  typedef TLabelImage LabelImageType;
  typedef typename LabelImageType::PixelType LabelPixelType;

  /** Deformation field Type. */
  typedef TDeformationField DeformationFieldType;

  /** Image dimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int, TLabelImage::ImageDimension);

  /** Set the atlas label image. */
  itkSetObjectMacro( AtlasLabelImage, LabelImageType );
  
  /** Set the atlas label image lower threshold. */
  itkSetMacro( LowerThreshold, LabelPixelType );
  
  /** Set the altas label image upper threshold. */
  itkSetMacro( UpperThreshold, LabelPixelType );

  /** Set the deformation field. */
  itkSetObjectMacro( DeformationField, DeformationFieldType );

  /** Set the output label value. */
  itkSetMacro( OutputValue, LabelPixelType );

  /** Method to execute the registration. */
  virtual void Execute();

  /** Get the ouput label image. */
  itkGetObjectMacro( OutputLabelImage, LabelImageType );


protected:
  AtlasLabeler();
  ~AtlasLabeler();

private:
  AtlasLabeler( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename LabelImageType::Pointer            m_AtlasLabelImage;
  typename LabelImageType::Pointer            m_OutputLabelImage;
  typename DeformationFieldType::Pointer      m_DeformationField;

  LabelPixelType                              m_LowerThreshold;
  LabelPixelType                              m_UpperThreshold;
  LabelPixelType                              m_OutputValue;


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "AtlasLabeler.txx"
#endif

#endif
