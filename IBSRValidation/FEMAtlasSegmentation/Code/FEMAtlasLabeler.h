/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMAtlasLabeler.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMAtlasLabeler_h
#define _FEMAtlasLabeler_h

#include "itkObject.h"
#include "itkVector.h"
#include "itkImage.h"
#include "itkFEM.h"
#include "itkFEMRegistrationFilter.h"

#include "itkArray.h"

namespace itk
{

/** \class FEMAtlasLabeler
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
 *   - pointer to the deforma
 *   - the output label value
 *
 * Outputs:
 *   - output labeled image
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
class FEMAtlasLabeler : public Object
{
public:

  /** Standard class typedefs. */
  typedef FEMAtlasLabeler Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMAtlasLabeler, Object);

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
  /** Set the ouput label image. */
  itkSetObjectMacro( OutputLabelImage, LabelImageType );


protected:
  FEMAtlasLabeler();
  ~FEMAtlasLabeler();

private:
  FEMAtlasLabeler( const Self& ); //purposely not implemented
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
#include "FEMAtlasLabeler.txx"
#endif

#endif
