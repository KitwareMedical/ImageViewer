/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    AtlasLabeler.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _AtlasLabeler_txx
#define _AtlasLabeler_txx

#include "AtlasLabeler.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkReinitializeLevelSetImageFilter.h"
#include "itkWarpImageFilter.h"

namespace itk
{


template <typename TLabelImage, typename TDeformationField>
AtlasLabeler<TLabelImage, TDeformationField>
::AtlasLabeler()
{
  // Images need to be set from the outside
  m_AtlasLabelImage  = NULL;
  m_DeformationField = NULL;
  m_OutputLabelImage = NULL;

  // Default parameters
  m_LowerThreshold = 1;
  m_UpperThreshold = NumericTraits<LabelPixelType>::max();
  m_OutputValue = 1;
    
}


template <typename TLabelImage, typename TDeformationField>
AtlasLabeler<TLabelImage, TDeformationField>
::~AtlasLabeler()
{

}


template <typename TLabelImage, typename TDeformationField>
void
AtlasLabeler<TLabelImage, TDeformationField>
::Execute()
{

  if ( !m_AtlasLabelImage || !m_DeformationField )
    {
    itkExceptionMacro( << "Either altas label or deformation field not set" );
    return;
    }

  typedef float RealType;
  typedef Image<RealType, ImageDimension> RealImageType;

  typedef BinaryThresholdImageFilter<LabelImageType,RealImageType> InputThresholderType;
  InputThresholderType::Pointer inputThresholder = InputThresholderType::New();

  inputThresholder->SetInput( m_AtlasLabelImage );
  inputThresholder->SetInsideValue( -0.5 );
  inputThresholder->SetOutsideValue( 0.5 );
  inputThresholder->SetLowerThreshold( m_LowerThreshold );
  inputThresholder->SetUpperThreshold( m_UpperThreshold );

  typedef ReinitializeLevelSetImageFilter<RealImageType> DistanceType;
  DistanceType::Pointer distance = DistanceType::New();

  distance->SetInput( inputThresholder->GetOutput() );
  distance->NarrowBandingOn();

  typedef WarpImageFilter<RealImageType,RealImageType,DeformationFieldType> WarperType;
  WarperType::Pointer warper = WarperType::New();

  warper->SetInput( distance->GetOutput() );
  warper->SetDeformationField( m_DeformationField );
  warper->SetOutputOrigin( m_DeformationField->GetOrigin() );
  warper->SetOutputSpacing( m_DeformationField->GetSpacing() );
  warper->SetEdgePaddingValue( NumericTraits<RealType>::max() );

  typedef BinaryThresholdImageFilter<RealImageType,LabelImageType> OutputThresholderType;
  OutputThresholderType::Pointer outputThresholder = OutputThresholderType::New();

  outputThresholder->SetInput( warper->GetOutput() );
  outputThresholder->SetUpperThreshold( NumericTraits<RealType>::Zero );
  outputThresholder->SetInsideValue ( m_OutputValue );
  outputThresholder->SetOutsideValue ( NumericTraits<LabelPixelType>::Zero );
  
  outputThresholder->Update();

  m_OutputLabelImage = outputThresholder->GetOutput();

}


} // namespace itk


#endif
