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

  this->m_ImageLoaded           = false;
  this->m_Reader                = ImageReaderType::New();
  
  // The Target is indeed an adaptor 
  // from InputPixelType to PixelType
  this->m_TargetImage           = TargetType::New();
  this->m_TargetImage->SetImage( m_Reader->GetOutput() );
  
  this->m_ReferenceImage        = ReferenceType::New();
  this->m_MappedReferenceImage  = MappedReferenceType::New();
  this->m_ImageMapper           = MapperType::New();
  this->m_RegistrationMethod    = RegistrationMethodType::New();

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

  this->m_Reader->SetFileToLoad( filename );
  this->m_Reader->Update();

  this->m_ImageLoaded = true;

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

  this->m_ReferenceImage = ReferenceType::New();

  this->m_ReferenceImage->SetLargestPossibleRegion(
      this->m_TargetImage->GetLargestPossibleRegion() );

  this->m_ReferenceImage->SetBufferedRegion(
      this->m_TargetImage->GetBufferedRegion() );

  this->m_ReferenceImage->SetRequestedRegion(
      this->m_TargetImage->GetRequestedRegion() );


  this->m_ImageMapper->SetDomain( this->m_TargetImage );


  typedef ReferenceType::IndexType  IndexType;

  ReferenceIteratorType it( m_ReferenceImage,
                   m_ReferenceImage->GetRequestedRegion() );

  it.Begin();

  while( ! it.IsAtEnd() )
  {
    IndexType index = it.GetIndex();
    PointType point;
    for(unsigned int i=0; i<ImageDimension; i++)
    {
      point[i] = index[i];
    }
    it.Set( this->m_ImageMapper->Evaluate( point ) );
    ++it;
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

  if( ! (this->m_ImageLoaded) ) 
  {
    ShowStatus("Please load an image first");
    return;
  }
  
  this->m_RegistrationMethod->SetReference( m_ReferenceImage );
  this->m_RegistrationMethod->SetTarget(    m_TargetImage    );

  this->m_RegistrationMethod->StartRegistration();

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



