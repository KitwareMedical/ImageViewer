/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ClassifierValidationOutput.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ClassifierValidationOutput_txx
#define _ClassifierValidationOutput_txx

#include "ClassifierValidationOutput.h"

#include "itkExceptionObject.h"
#include "itkCropImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSimilarityIndexImageFilter.h"
#include "itkHausdorffDistanceImageFilter.h"
#include "itkStatisticsImageFilter.h"

namespace itk
{

template <typename TImage>
ClassifierValidationOutput<TImage>
::ClassifierValidationOutput()
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
ClassifierValidationOutput<TImage>
::Execute()
{

  /*** Write out the classified volume ****/

  typedef itk::RawVolumeWriter<TImage> ByteWriterType;
  ByteWriterType::Pointer bytewriter = ByteWriterType::New();

  bytewriter->SetInputImage( m_ClassifiedImage );
  bytewriter->SetFileName( "classified.raw" );
  bytewriter->Execute();


  if ( !m_ClassifiedImage || !m_TruthImage || m_OutputFileName == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  IndexType classifiedRegionIndex = 
    (m_ClassifiedImage)->GetBufferedRegion().GetIndex();

  //-------------------------------------------------------------------------
  //Extract the sub image from classified image to align first slice with the
  //truth image.
  //-------------------------------------------------------------------------
  m_UpperBoundaryCropSize.Fill(0);
  m_LowerBoundaryCropSize.Fill(0);

  m_LowerBoundaryCropSize[2] = m_StartSegSliceNumber;  

  this->ExtractSubImage( m_ClassifiedImage );  

  //-------------------------------------------------------------------------
  // Identify the size by comparing with the truth and the classified images
  //-------------------------------------------------------------------------

  SizeType   truthRegionSize, classifiedRegionSize;

  bool truthImageSizeChange      = false;
  bool classifiedImageSizeChange = false;

  truthRegionSize          = (m_TruthImage)->GetBufferedRegion().GetSize();
  classifiedRegionSize     = (m_ClassifiedImage)->GetBufferedRegion().GetSize();

  //Find the new size
  if( truthRegionSize[2] != classifiedRegionSize[2])
    {
    if( truthRegionSize[2] >  classifiedRegionSize[2])
      {
      m_UpperBoundaryCropSize[2] = truthRegionSize[2] - classifiedRegionSize[2];
      m_LowerBoundaryCropSize[2] = 0;
      truthImageSizeChange = true;  
      }
    else
      {
      m_UpperBoundaryCropSize[2] = classifiedRegionSize[2] - truthRegionSize[2];
      m_LowerBoundaryCropSize[2] = 0;
      classifiedImageSizeChange = true;
      }
    }

  //Set the extract image filter

  if( truthImageSizeChange ) 
    {
    this->ExtractSubImage( m_TruthImage );
    }
  if( classifiedImageSizeChange ) 
    {
    this->ExtractSubImage( m_ClassifiedImage );  
    }




  ByteWriterType::Pointer bytewriter1 = ByteWriterType::New();

  bytewriter1->SetInputImage( m_TruthImage );
  bytewriter1->SetFileName( "truthImage.raw" );
  bytewriter1->Execute();

  ByteWriterType::Pointer bytewriter2 = ByteWriterType::New();

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

  for(unsigned int i=1; i<m_TruthLabels.Size(); ++i)
    {

    this->GetMeasuredStatistics(i);

    std::cout << m_PatientID.c_str() 
              << "Tissue Label: " << i
              << " Overlap: " <<  m_SimilarityIndex
              << " Est. volume: " << m_Volume1
              << " True volume: " << m_Volume2
              << std::endl; 
   
    // Write out the overlap
    fprintf( file, "%s\t%d\t%.5f\t%ld\t%ld\n", m_PatientID.c_str(), i, 
      m_SimilarityIndex, m_Volume1, m_Volume2 );

    }

  fclose( file );

}


template <typename TImage>
void
ClassifierValidationOutput<TImage>
::ExtractSubImage( ImagePointer &image )
{

  RegionType region = (image)->GetBufferedRegion();
  ImageRegionIterator<ImageType> iterator(image, region);

  // Create a filter
  CropImageFilter< ImageType, ImageType >::Pointer extract;
  extract = CropImageFilter< ImageType, ImageType >::New();
  extract->SetInput( image );
  
  extract->SetUpperBoundaryCropSize(m_UpperBoundaryCropSize);
  extract->SetLowerBoundaryCropSize(m_LowerBoundaryCropSize);
  extract->UpdateLargestPossibleRegion();
  image = extract->GetOutput();
}

template <typename TImage>
void ClassifierValidationOutput<TImage>
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

  m_TruthImageLowerThreshold  = m_TruthLabels[ labelIndex ];
  m_TruthImageUpperThreshold  = m_TruthLabels[ labelIndex ];

  threshold2->SetInput( m_TruthImage );
  threshold2->SetLowerThreshold( m_TruthImageLowerThreshold );
  threshold2->SetUpperThreshold( m_TruthImageUpperThreshold );
  threshold2->SetInsideValue( 1 );
  threshold2->SetOutsideValue( 0 );

  typedef itk::SimilarityIndexImageFilter<ImageType,ImageType> OverlapType;
  typename OverlapType::Pointer overlap = OverlapType::New();

  overlap->SetInput1( threshold1->GetOutput() );
  overlap->SetInput2( threshold2->GetOutput() );

  overlap->Update();

  m_SimilarityIndex = overlap->GetSimilarityIndex();

  typedef itk::StatisticsImageFilter<ImageType> SumType;
  typename SumType::Pointer summer = SumType::New();


  summer->SetInput( threshold1->GetOutput() );
  summer->Update();
  m_Volume1 = static_cast<unsigned long>( summer->GetSum() );

  summer->SetInput( threshold2->GetOutput() );
  summer->Update();
  m_Volume2 = static_cast<unsigned long>( summer->GetSum() );

}

} // namespace itk

#endif
