/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsoleBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#include <liImageRegistrationConsoleBase.h>


/************************************
 *
 *  Constructor
 *
 ***********************************/
liImageRegistrationConsoleBase 
::liImageRegistrationConsoleBase()
{

  m_ImageLoaded           = false;
  m_Reader                = ImageReaderType::New();
  
  // The Target is indeed an adaptor 
  // from InputPixelType to PixelType
  m_TargetImage           = TargetType::New();
  m_TargetImage->SetImage( m_Reader->GetOutput() );
  
  m_ReferenceImage        = ReferenceType::New();
  m_MappedReferenceImage  = MappedReferenceType::New();
  m_TargetMapper          = TargetMapperType::New();
  m_ReferenceMapper       = ReferenceMapperType::New();

  m_MutualInformationMethod = 
                        MutualInformationRegistrationMethodType::New();
  
  m_MeansSquaresMethod      = 
                        MeanSquaresRegistrationMethodType::New();
  
  m_PatternIntensityMethod  = 
                        PatternIntensityRegistrationMethodType::New();
  
  m_NormalizedCorrelationMethod =
                        NormalizedCorrelationRegistrationMethodType::New();

  m_SelectedMethod = meanSquares;

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liImageRegistrationConsoleBase 
::~liImageRegistrationConsoleBase()
{

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::Load( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_Reader->SetFileToLoad( filename );
  m_Reader->Update();

  m_ImageLoaded = true;

}


 
/************************************
 *
 *  Show Progress
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::ShowProgress( float )
{

}


 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::ShowStatus( const char * )
{

}


/************************************
 *
 *  Generate reference image
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::GenerateReference( void )
{

  this->ShowStatus("Transforming the original image...");

  // Select to Process the whole image 
  m_TargetImage->SetRequestedRegion(
      m_TargetImage->GetBufferedRegion() );
  

  //  Allocate the reference accordingly
  m_ReferenceImage = ReferenceType::New();

  m_ReferenceImage->SetLargestPossibleRegion(
      m_TargetImage->GetLargestPossibleRegion() );

  m_ReferenceImage->SetBufferedRegion(
      m_TargetImage->GetBufferedRegion() );

  m_ReferenceImage->SetRequestedRegion(
      m_TargetImage->GetRequestedRegion() );

  m_ReferenceImage->Allocate();

  m_TargetMapper->SetDomain( m_TargetImage );


  this->UpdateTransformationParameters();

  typedef ReferenceType::IndexType  IndexType;

  ReferenceIteratorType it( m_ReferenceImage,
                   m_ReferenceImage->GetRequestedRegion() );
  
  float percent = 0.0;

  const unsigned long totalPixels =
        m_ReferenceImage->GetOffsetTable()[ImageDimension];

  const unsigned long hundreth = totalPixels / 100;
  unsigned long counter = 0;

  it.Begin();

  while( ! it.IsAtEnd() )
  {

    if( counter > hundreth ) 
    {
      counter = 0;
      percent += 0.01;
      this->ShowProgress( percent );
    }
    
    IndexType index = it.GetIndex();
    PointType point;
    for(unsigned int i=0; i<ImageDimension; i++)
    {
      point[i] = index[i];
    }
    PixelType value;
    try 
    {
      value = m_TargetMapper->Evaluate( point );
    }
    catch( itk::MapperException )
    {      
      value = 0.0;
    }
    it.Set( value );
    ++it;
    ++counter;
  }

  this->ShowProgress( 1.0 );
  this->ShowStatus("Target Image Transformation done");

}


 


/************************************
 *
 *  Stop Registration
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::Stop( void )
{

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }
  
  switch( m_SelectedMethod )
  {
  case mutualInformation:
    m_MutualInformationMethod->GetOptimizer()->StopOptimization();
    break;
  case normalizedCorrelation:
    m_NormalizedCorrelationMethod->GetOptimizer()->StopOptimization();
    break;
  case patternIntensity:
    m_PatternIntensityMethod->GetOptimizer()->StopOptimization();
    break;
  case meanSquares:
    m_MeansSquaresMethod->GetOptimizer()->StopOptimization();
    break;
  }



}




/************************************
 *
 *  Execute
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::Execute( void )
{

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }
  
  const double translationScale = 1e4;

  switch( m_SelectedMethod )
  {
  case mutualInformation:
    {
    m_MutualInformationMethod->SetReference( m_ReferenceImage );
    m_MutualInformationMethod->SetTarget(    m_TargetImage    );

    TargetType::SizeType size;
    size = m_TargetImage->GetRequestedRegion().GetSize();

    // set the transformation centers
    MutualInformationRegistrationMethodType::PointType transCenter;
    for( unsigned int j = 0; j < ImageDimension; j++ )
      {
      transCenter[j] = double(size[j]) / 2;
      }

    m_MutualInformationMethod->SetTargetTransformationCenter( transCenter );
    m_MutualInformationMethod->SetReferenceTransformationCenter( transCenter );

    // set optimization related parameters
    m_MutualInformationMethod->SetNumberOfIterations( 1000 );
    m_MutualInformationMethod->SetLearningRate( 0.2 );

    // set metric related parameters
    m_MutualInformationMethod->GetMetric()->SetTargetStandardDeviation( 20.0 );
    m_MutualInformationMethod->GetMetric()->SetReferenceStandardDeviation( 20.0 );
    m_MutualInformationMethod->GetMetric()->SetNumberOfSpatialSamples( 50 );

    m_MutualInformationMethod->StartRegistration();

    m_ReferenceMapper->GetTransformation()->SetParameters(
        m_MutualInformationMethod->GetOptimizer()->GetCurrentPosition() );

    break;
    }

  case normalizedCorrelation:
    {
    m_NormalizedCorrelationMethod->SetReference( m_ReferenceImage );
    m_NormalizedCorrelationMethod->SetTarget(    m_TargetImage    );
    m_NormalizedCorrelationMethod->SetTranslationScale( translationScale );
    m_NormalizedCorrelationMethod->StartRegistration();

    m_ReferenceMapper->GetTransformation()->SetParameters(
        m_NormalizedCorrelationMethod->GetOptimizer()->GetCurrentPosition() );

    break;
    }

  case patternIntensity:
    {
    m_PatternIntensityMethod->SetReference( m_ReferenceImage );
    m_PatternIntensityMethod->SetTarget(    m_TargetImage    );
    m_PatternIntensityMethod->SetTranslationScale( translationScale );
    m_PatternIntensityMethod->StartRegistration();

    m_ReferenceMapper->GetTransformation()->SetParameters(
        m_PatternIntensityMethod->GetOptimizer()->GetCurrentPosition() );

    break;
    }

  case meanSquares:
    {
    m_MeansSquaresMethod->SetReference( m_ReferenceImage );
    m_MeansSquaresMethod->SetTarget(    m_TargetImage    );
    m_MeansSquaresMethod->SetTranslationScale( translationScale );
    m_MeansSquaresMethod->GetOptimizer()->SetMaximumStepLength( 1.0  );
    m_MeansSquaresMethod->GetOptimizer()->SetMinimumStepLength( 1e-3 );
    m_MeansSquaresMethod->GetOptimizer()->SetGradientMagnitudeTolerance( 1e-8 );
    m_MeansSquaresMethod->GetOptimizer()->SetMaximumNumberOfIterations( 200 );
    m_MeansSquaresMethod->StartRegistration();

    m_ReferenceMapper->GetTransformation()->SetParameters(
        m_MeansSquaresMethod->GetOptimizer()->GetCurrentPosition() );

    break;
    }

  }



}




 
/************************************
 *
 *  Generate Mapped Reference image
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::GenerateMappedReference( void )
{

  this->ShowStatus("Transforming the reference image...");


  //  Allocate the reference accordingly
  m_MappedReferenceImage = MappedReferenceType::New();

  m_MappedReferenceImage->SetLargestPossibleRegion(
      m_TargetImage->GetLargestPossibleRegion() );

  m_MappedReferenceImage->SetBufferedRegion(
      m_TargetImage->GetBufferedRegion() );

  // Process all the buffered data
  m_MappedReferenceImage->SetRequestedRegion(
      m_TargetImage->GetBufferedRegion() ); 

  m_MappedReferenceImage->Allocate();

  m_ReferenceMapper->SetDomain( m_ReferenceImage );


  this->UpdateTransformationParameters();

  typedef ReferenceType::IndexType  IndexType;

  ReferenceIteratorType it( 
              m_MappedReferenceImage,
              m_MappedReferenceImage->GetRequestedRegion() );
  
  float percent = 0.0;

  const unsigned long totalPixels =
        m_MappedReferenceImage->GetOffsetTable()[ImageDimension];

  const unsigned long hundreth = totalPixels / 100;
  unsigned long counter = 0;

  it.Begin();

  while( ! it.IsAtEnd() )
  {

    if( counter > hundreth ) 
    {
      counter = 0;
      percent += 0.01;
      this->ShowProgress( percent );
    }
    
    IndexType index = it.GetIndex();
    PointType point;
    for(unsigned int i=0; i<ImageDimension; i++)
    {
      point[i] = index[i];
    }
    PixelType value;
    try 
    {
      value = m_ReferenceMapper->Evaluate( point );
    }
    catch( itk::MapperException )
    {      
      value = 0.0;
    }
    it.Set( value );
    ++it;
    ++counter;
  }

  this->ShowProgress( 1.0 );
  this->ShowStatus("Reference Image Transformation done");

}


 
/************************************
 *
 *  Update the parameters of the 
 *  Transformation
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::UpdateTransformationParameters( void )
{

}


 
/************************************
 *
 *  Select the registration method
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::SelectRegistrationMethod( RegistrationMethodType method )
{
  m_SelectedMethod = method;
}



