/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRawImageReaderViewer.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkRawImageReaderViewer.h"
#include "FL/fl_ask.H"




RawImageReaderViewer
::RawImageReaderViewer()
{
  m_RawImageSource =  FileSourceType::New();
  m_RawImageSink   =  FileSinkType::New();  
                      
  m_RawReader      =  RawReaderType::New(); 
  m_RawWriter      =  RawReaderType::New(); 

  m_RawImageSource->SetImageIO( m_RawReader );
  m_RawImageSink->SetImageIO(   m_RawWriter );

  // size of BrainWeb images in the ftp repository
  m_NumberOfPixelsInX = 181;
  m_NumberOfPixelsInY = 217;
  m_NumberOfPixelsInZ = 181;

}



RawImageReaderViewer
::~RawImageReaderViewer()
{
}



void
RawImageReaderViewer
::SetDimensionX( unsigned int numberOfPixels )
{
  m_NumberOfPixelsInX = numberOfPixels;
}


void
RawImageReaderViewer
::SetDimensionY( unsigned int numberOfPixels )
{
  m_NumberOfPixelsInY = numberOfPixels;
}


void
RawImageReaderViewer
::SetDimensionZ( unsigned int numberOfPixels )
{
  m_NumberOfPixelsInZ = numberOfPixels;
}



void
RawImageReaderViewer
::Quit(void)
{
  this->HideImage();
}


void
RawImageReaderViewer
::HideImage(void)
{
  m_Viewer.Hide();
}


void
RawImageReaderViewer
::ShowImage(void)
{
  m_Viewer.SetImage( m_RawImageSource->GetOutput() );
  m_Viewer.Show();
}




void
RawImageReaderViewer
::Load( void )
{
  
  const char * filename = fl_file_chooser("Image Filename","*.raw","");
 
  if( !filename )
    {
    return;
    }

    this->Load( filename );
}



void
RawImageReaderViewer
::Save( void )
{
  
  const char * filename = fl_file_chooser("Image Filename","*.raw","");
 
  if( !filename )
    {
    return;
    }

    this->Save( filename );
}



void
RawImageReaderViewer
::Load( const char * filename )
{

  m_RawImageSource->SetFileName( filename );
  m_RawReader->SetDimensions( 0, m_NumberOfPixelsInX );
  m_RawReader->SetDimensions( 1, m_NumberOfPixelsInY );
  m_RawReader->SetDimensions( 2, m_NumberOfPixelsInZ );

  // Attempt to read
  try
    {
    m_RawImageSource->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    fl_alert( e.GetDescription() );
    }

}




void
RawImageReaderViewer
::Save( const char * filename )
{

  m_RawImageSink->SetFileName( filename );
  m_RawImageSink->SetInput( m_RawImageSource->GetOutput() );

  // Attempt to Write
  m_RawImageSink->Write();

}




