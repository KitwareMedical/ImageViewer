/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MSClassifierValidationOutput.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MSClassifierValidationOutput_txx
#define _MSClassifierValidationOutput_txx

#include "MSClassifierValidationOutput.h"

#include "itkExceptionObject.h"
#include "itkCropImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSimilarityIndexImageFilter.h"
#include "itkHausdorffDistanceImageFilter.h"
#include "itkStatisticsImageFilter.h"

namespace itk
{

template <typename TImage>
MSClassifierValidationOutput<TImage>
::MSClassifierValidationOutput()
{
  m_ClassifiedImage = NULL;
  m_TruthImage = NULL;

  for( unsigned int i=0; i< ImageDimension; i++ )
    {
    m_LowerBoundaryCropSize[i] = 0;
    m_UpperBoundaryCropSize[i] = 0;
    }

  m_ClassifiedImageLowerThreshold = 1;
  m_ClassifiedImageUpperThreshold = 1;

  m_TruthImageLowerThreshold = 1;
  m_TruthImageUpperThreshold = 1;

  m_OutputFileName = "";
  m_AppendOutputFile = false;

  m_PatientID = "";

}


template <typename TImage>
void
MSClassifierValidationOutput<TImage>
::Execute()
{

  /*** Write out the classified volume ****/

  typedef itk::RawVolumeWriter<TImage> ByteWriterType;
  typename ByteWriterType::Pointer bytewriter = ByteWriterType::New();

  bytewriter->SetInputImage( m_ClassifiedImage );
  bytewriter->SetFileName( "classified.raw" );
  bytewriter->Execute();


  if ( !m_ClassifiedImage || m_OutputFileName == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  IndexType classifiedRegionIndex = 
    (m_ClassifiedImage)->GetBufferedRegion().GetIndex();

  typename ByteWriterType::Pointer bytewriter2 = ByteWriterType::New();

  bytewriter2->SetInputImage( m_ClassifiedImage );
  bytewriter2->SetFileName( "classifiedImage.raw" );
  bytewriter2->Execute();

  //Set the new index
  RegionType tempRegion = (m_ClassifiedImage)->GetBufferedRegion();
  tempRegion.SetIndex( classifiedRegionIndex );
  m_ClassifiedImage->SetBufferedRegion( tempRegion );
  tempRegion = (m_ClassifiedImage)->GetLargestPossibleRegion();
  tempRegion.SetIndex( classifiedRegionIndex );
  m_ClassifiedImage->SetLargestPossibleRegion( tempRegion );  

  FILE * file;
  if ( m_AppendOutputFile )
    {
    file = fopen( m_OutputFileName.c_str(), "a" );
    }
  else
    {
    file = fopen( m_OutputFileName.c_str(), "w" );
    }

  if ( !file )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Can't open output file." );
    throw err;
    }

  unsigned int labelOfInterest = m_TruthLabels.Size() - 1;
  labelOfInterest = 4;
  this->GetMeasuredStatistics(labelOfInterest);

  std::cout << m_PatientID.c_str() 
            << "Tissue Label: " << labelOfInterest
            << " Est. volume: " << m_Volume1 * (0.3 * 0.078 * 0.078)
            << " True volume: " << 29.5
            << std::endl; 
 
  // Write out the overlap
  fprintf( file, "%s\t%d\t%5.2f\t\n", m_PatientID.c_str(), labelOfInterest, 
    (m_Volume1 * (0.3 * 0.078 * 0.078)) );

  fclose( file );

}

template <typename TImage>
void MSClassifierValidationOutput<TImage>
::GetMeasuredStatistics(unsigned int labelIndex)
{
  // Compute the overlap
  typedef itk::BinaryThresholdImageFilter<ImageType,ImageType> ThresholdType;
  typename ThresholdType::Pointer threshold1 = ThresholdType::New();

  m_ClassifiedImageLowerThreshold = labelIndex;
  m_ClassifiedImageUpperThreshold = labelIndex;

  threshold1->SetInput( m_ClassifiedImage );
  threshold1->SetLowerThreshold( m_ClassifiedImageLowerThreshold );
  threshold1->SetUpperThreshold( m_ClassifiedImageUpperThreshold );
  threshold1->SetInsideValue( 1 );
  threshold1->SetOutsideValue( 0 );

  typename ThresholdType::Pointer threshold2 = ThresholdType::New();

  typedef itk::StatisticsImageFilter<ImageType> SumType;
  typename SumType::Pointer summer = SumType::New();


  summer->SetInput( threshold1->GetOutput() );
  summer->Update();
  m_Volume1 = static_cast<unsigned long>( summer->GetSum() );

}

} // namespace itk

#endif
