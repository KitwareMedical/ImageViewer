/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ValidationOutput.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ValidationOutput_txx
#define _ValidationOutput_txx

#include "ValidationOutput.h"

#include "itkExceptionObject.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSimilarityIndexImageFilter.h"
#include "itkHausdorffDistanceImageFilter.h"
#include "itkStatisticsImageFilter.h"

namespace itk
{

template <typename TImage>
ValidationOutput<TImage>
::ValidationOutput()
{
  m_LabelImage = NULL;
  m_GroundTruthImage = NULL;

  m_LabelLowerThreshold = 1;
  m_LabelUpperThreshold = NumericTraits<PixelType>::max();

  m_GroundTruthLowerThreshold = 1;
  m_GroundTruthUpperThreshold = NumericTraits<PixelType>::max();

  m_OutputFileName = "";
  m_AppendOutputFile = false;

  m_PatientID = "";

}


template <typename TImage>
void
ValidationOutput<TImage>
::Execute()
{

  if ( !m_LabelImage || !m_GroundTruthImage || 
       m_OutputFileName == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }


  // Compute the overlap
  typedef itk::BinaryThresholdImageFilter<ImageType,ImageType> ThresholdType;
  typename ThresholdType::Pointer threshold1 = ThresholdType::New();

  threshold1->SetInput( m_LabelImage );
  threshold1->SetLowerThreshold( m_LabelLowerThreshold );
  threshold1->SetUpperThreshold( m_LabelUpperThreshold );
  threshold1->SetInsideValue( 1 );
  threshold1->SetOutsideValue( 0 );

  typename ThresholdType::Pointer threshold2 = ThresholdType::New();

  threshold2->SetInput( m_GroundTruthImage );
  threshold2->SetLowerThreshold( m_GroundTruthLowerThreshold );
  threshold2->SetUpperThreshold( m_GroundTruthUpperThreshold );
  threshold2->SetInsideValue( 1 );
  threshold2->SetOutsideValue( 0 );


  typedef itk::SimilarityIndexImageFilter<ImageType,ImageType> OverlapType;
  typename OverlapType::Pointer overlap = OverlapType::New();

  overlap->SetInput1( threshold1->GetOutput() );
  overlap->SetInput2( threshold2->GetOutput() );

  overlap->Update();


  typedef itk::HausdorffDistanceImageFilter<ImageType,ImageType> DistanceType;
  typename DistanceType::Pointer distance = DistanceType::New();

  distance->SetInput1( threshold1->GetOutput() );
  distance->SetInput2( threshold2->GetOutput() );

  distance->Update();

  typedef itk::StatisticsImageFilter<ImageType> SumType;
  typename SumType::Pointer summer = SumType::New();
  unsigned long volume1, volume2;

  summer->SetInput( threshold1->GetOutput() );
  summer->Update();
  volume1 = static_cast<unsigned long>( summer->GetSum() );

  summer->SetInput( threshold2->GetOutput() );
  summer->Update();
  volume2 = static_cast<unsigned long>( summer->GetSum() );

  summer->SetInput( threshold1->GetOutput() );

  std::cout << m_PatientID.c_str() 
            << " Overlap: " << overlap->GetSimilarityIndex() 
            << " Distance: " << distance->GetHausdorffDistance()
            << " Est. volume: " << volume1
            << " True volume: " << volume2
            << std::endl;

  // Write out the overlap
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

  fprintf( file, "%s\t%.5f\t%3.3f\t%ld\t%ld\n", m_PatientID.c_str(), overlap->GetSimilarityIndex(),
    distance->GetHausdorffDistance(), volume1, volume2 );

  fclose( file );

}


} // namespace itk

#endif
