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
  m_ImageMapper           = MapperType::New();

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

  m_ImageMapper->SetDomain( m_TargetImage );


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
      value = m_ImageMapper->Evaluate( point );
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
  this->ShowStatus("Image Transformation done");

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
  
  switch( m_SelectedMethod )
  {
  case mutualInformation:
    m_MutualInformationMethod->SetReference( m_ReferenceImage );
    m_MutualInformationMethod->SetTarget(    m_TargetImage    );
    m_MutualInformationMethod->StartRegistration();
    break;
  case normalizedCorrelation:
    m_NormalizedCorrelationMethod->SetReference( m_ReferenceImage );
    m_NormalizedCorrelationMethod->SetTarget(    m_TargetImage    );
    m_NormalizedCorrelationMethod->StartRegistration();
    break;
  case patternIntensity:
    m_PatternIntensityMethod->SetReference( m_ReferenceImage );
    m_PatternIntensityMethod->SetTarget(    m_TargetImage    );
    m_PatternIntensityMethod->StartRegistration();
    break;
  case meanSquares:
    m_MeansSquaresMethod->SetReference( m_ReferenceImage );
    m_MeansSquaresMethod->SetTarget(    m_TargetImage    );
    m_MeansSquaresMethod->StartRegistration();
    break;
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



