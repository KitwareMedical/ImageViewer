
/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMBrainStripValidationApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMBrainStripValidationApp_txx
#define _FEMBrainStripValidationApp_txx

#include "FEMBrainStripValidationApp.h"

namespace itk
{

template <typename TImage, typename TLabelImage, typename TRealImage>
FEMBrainStripValidationApp<TImage,TLabelImage,TRealImage>
::FEMBrainStripValidationApp()
{ 
  m_ImageDirectoryName = "";
  m_BrainSegmentationDirectoryName = "";
  m_AtlasPatientID = "";
  m_SubjectPatientID = "";
  m_AtlasStartSliceNumber = 1;
  m_SubjectStartSliceNumber = 1;
  m_AtlasNumberOfSlices = 1;
  m_SubjectNumberOfSlices = 1;

  m_ParameterFileName = "";
  m_OutputFileName = "";
  m_AppendOutputFile = true;
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
FEMBrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeParser()
{
  m_Parser->SetImageDirectoryName( m_ImageDirectoryName.c_str() );
  m_Parser->SetBrainSegmentationDirectoryName( m_BrainSegmentationDirectoryName.c_str() );

  m_Parser->SetAtlasPatientID( m_AtlasPatientID.c_str() );
  m_Parser->SetAtlasStartSliceNumber( m_AtlasStartSliceNumber );
  m_Parser->SetAtlasNumberOfSlices( m_AtlasNumberOfSlices );

  m_Parser->SetSubjectPatientID( m_SubjectPatientID.c_str() );
  m_Parser->SetSubjectStartSliceNumber( m_SubjectStartSliceNumber );
  m_Parser->SetSubjectNumberOfSlices( m_SubjectNumberOfSlices );
  m_Parser->SetImageXSize(m_ImageXSize);
  m_Parser->SetImageYSize(m_ImageYSize);

  m_Parser->SetParameterFileName( m_ParameterFileName.c_str() );
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
FEMBrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializePreprocessor()
{
  bool Resample=true;

  if ( Resample){  
  
  typedef typename ImageType::SizeType                 ImageSizeType;
  typedef itk::AffineTransform<double,ImageDimension>   AffineTransformType;

  // Create an affine transformation
  itk::ResampleImageFilter< ImageType, ImageType >::Pointer resample;
  AffineTransformType::Pointer aff = AffineTransformType::New();
  aff->Scale(0.90);

  // Create a linear interpolation image function
  typedef itk::LinearInterpolateImageFunction<ImageType,double>  InterpolatorType;
  InterpolatorType::Pointer interp;
  
  
  interp = InterpolatorType::New();
  resample = itk::ResampleImageFilter< ImageType, ImageType >::New();
  ImageSizeType size={{128,128,91}};
  resample->SetSize(size);
  resample->SetTransform(aff.GetPointer());
  resample->SetInterpolator(interp.GetPointer());

  // Run the resampling filter
  
  interp->SetInputImage(m_Parser->GetSubjectImage());
  resample->SetInput(m_Parser->GetSubjectImage());
  resample->Update();
  m_Parser->SetSubjectImage(resample->GetOutput());
  
  interp = InterpolatorType::New();
  resample = itk::ResampleImageFilter< ImageType, ImageType >::New();
  resample->SetSize(size);
  resample->SetTransform(aff.GetPointer());
  resample->SetInterpolator(interp.GetPointer());
  

  interp->SetInputImage(m_Parser->GetAtlasImage());
  resample->SetInput(m_Parser->GetAtlasImage());
  resample->Update();
  m_Parser->SetAtlasImage(resample->GetOutput());



  // Create a linear interpolation image function
  typedef itk::LinearInterpolateImageFunction<LabelImageType,double>  InterpolatorType2;
  InterpolatorType2::Pointer interp2;
  // Create and configure a resampling filter
  itk::ResampleImageFilter<  LabelImageType, LabelImageType >::Pointer resample2;
  
  
  interp2 = InterpolatorType2::New();
  resample2 = itk::ResampleImageFilter<  LabelImageType,  LabelImageType >::New();
  resample2->SetSize(size);
  resample2->SetTransform(aff.GetPointer());
  resample2->SetInterpolator(interp2.GetPointer());

  // Run the resampling filter
  
  interp2->SetInputImage(m_Parser->GetSubjectLabelImage());
  resample2->SetInput(m_Parser->GetSubjectLabelImage());
  resample2->Update();
  m_Parser->SetSubjectLabelImage(resample2->GetOutput());
  
  
  interp2 = InterpolatorType::New();
  resample2 = itk::ResampleImageFilter<  LabelImageType,  LabelImageType >::New();
  resample2->SetSize(size);
  resample2->SetTransform(aff.GetPointer());
  resample2->SetInterpolator(interp2.GetPointer());

  interp2->SetInputImage(m_Parser->GetAtlasLabelImage());
  resample2->SetInput(m_Parser->GetAtlasLabelImage());
  resample2->Update();
  m_Parser->SetAtlasLabelImage(resample2->GetOutput());
  
  /*
  typedef typename LabelImageType::PixelType PType;
  itk::RawImageIO<PType,ImageDimension>::Pointer io;
  itk::ImageFileWriter<LabelImageType>::Pointer writer;
  io = itk::RawImageIO<PType,ImageDimension>::New();
  writer = itk::ImageFileWriter<LabelImageType>::New();
  writer->SetImageIO(io);
  std::string fn1=m_ImageDirectoryName+"/SmallAIR/whead"+m_SubjectPatientID+".img";
  writer->SetFileName(fn1.c_str());
  writer->SetInput(resample->GetOutput() ); 
  writer->Write();


  std::string fn2=m_ImageDirectoryName+"/SmallAIR/brain"+m_SubjectPatientID+".img";
  writer->SetFileName(fn2.c_str());
  writer->SetInput(resample->GetOutput() ); 
  writer->Write();

  return;
  */
  }

  m_Preprocessor->SetInputFixedImage( m_Parser->GetSubjectImage() );
  m_Preprocessor->SetInputMovingImage( m_Parser->GetAtlasImage() );

  m_Preprocessor->SetNumberOfHistogramLevels( this->GetNumberOfHistogramLevels() );
  m_Preprocessor->SetNumberOfMatchPoints( this->GetNumberOfMatchPoints() );

  m_Preprocessor->SetEdgeFilter( this->GetEdgeFilter() );
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
FEMBrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeRegistrator()
{
  m_Registrator->SetFixedImage( m_Preprocessor->GetOutputFixedImage() );
  m_Registrator->SetMovingImage( m_Preprocessor->GetOutputMovingImage() );
  m_Registrator->SetFEMConfigurationFileName(this->m_FEMConfigurationFileName.c_str());
  m_Registrator->SetAtlasLabelImage( m_Parser->GetAtlasLabelImage() );
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
FEMBrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeLabeler()
{

  m_Labeler->SetAtlasLabelImage( m_Registrator->GetWarpedAtlasLabelImage() );

  typedef typename TLabelImage::PixelType LabelPixelType;
  m_Labeler->SetLowerThreshold( 1 );
  m_Labeler->SetUpperThreshold( NumericTraits<LabelPixelType>::max() );

  m_Labeler->SetOutputValue( 1 );

}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
FEMBrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeGenerator()
{

  m_Generator->SetPatientID( m_SubjectPatientID.c_str() );
  m_Generator->SetGroundTruthImage( m_Parser->GetSubjectLabelImage() );

  m_Generator->SetLabelImage( m_Labeler->GetOutputLabelImage() );

  m_Generator->SetOutputFileName( m_OutputFileName.c_str() );
  m_Generator->SetAppendOutputFile( m_AppendOutputFile );


  typedef typename LabelImageType::PixelType PType;
  itk::RawImageIO<PType,ImageDimension>::Pointer io;
  itk::ImageFileWriter<LabelImageType>::Pointer writer;
  io = itk::RawImageIO<PType,ImageDimension>::New();
  writer = itk::ImageFileWriter<LabelImageType>::New();
  writer->SetImageIO(io);
  writer->SetFileName("subjectlabel.raw");
  writer->SetInput(m_Parser->GetSubjectLabelImage() ); 
  writer->Write();
 
  itk::RawImageIO<PType,ImageDimension>::Pointer io2;
  itk::ImageFileWriter<LabelImageType>::Pointer writer2;
  io2 = itk::RawImageIO<PType,ImageDimension>::New();
  writer2 = itk::ImageFileWriter<LabelImageType>::New();
  writer2->SetImageIO(io2);
  writer2->SetFileName("outputlabel.raw");
  writer2->SetInput(m_Labeler->GetOutputLabelImage()); 
  writer2->Write();

 

}


} // namespace itk

#endif
