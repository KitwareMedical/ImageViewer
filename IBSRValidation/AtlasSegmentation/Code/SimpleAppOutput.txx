/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SimpleAppOutput.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SimpleAppOutput_txx
#define _SimpleAppOutput_txx

#include "SimpleAppOutput.h"

#include "itkWarpImageFilter.h"
#include "itkExceptionObject.h"

#include "PGMVolumeWriter.h"

namespace itk
{

template <typename TImage, typename TDeformationField>
SimpleAppOutput<TImage,TDeformationField>
::SimpleAppOutput()
{
  m_FixedImage = NULL;
  m_MovingImage = NULL;
  m_PaddingValue = 0;
  m_ResampledImage = NULL;

  m_DeformationField = NULL;

  m_DirectoryName = "";
}


template <typename TImage, typename TDeformationField>
void
SimpleAppOutput<TImage,TDeformationField>
::Execute()
{

  if ( !m_MovingImage || !m_FixedImage || !m_DeformationField ||
    m_DirectoryName == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  // set up the warper
  typedef WarpImageFilter<ImageType,ImageType,DeformationFieldType>
    WarperType;

  WarperType::Pointer warper = WarperType::New();

  warper->SetInput( m_MovingImage );
  warper->SetDeformationField( m_DeformationField );
  warper->SetOutputOrigin( m_FixedImage->GetOrigin() );
  warper->SetOutputSpacing( m_FixedImage->GetSpacing() );
  warper->SetEdgePaddingValue( m_PaddingValue );

  // warp the moving image
  warper->Update();

  m_ResampledImage = warper->GetOutput();


  // write out image as pgm files
  typedef PGMVolumeWriter<ImageType> PGMWriterType;
  PGMWriterType::Pointer writer = PGMWriterType::New();

  try
    {

    writer->SetImage( m_FixedImage );
    writer->SetDirectoryName( m_DirectoryName.c_str() );
    writer->SetFilePrefix( "target" );
    writer->Execute();

    writer->SetImage( m_MovingImage );
    writer->SetDirectoryName( m_DirectoryName.c_str() );
    writer->SetFilePrefix( "source" );
    writer->Execute();

    writer->SetImage( m_ResampledImage );
    writer->SetDirectoryName( m_DirectoryName.c_str() );
    writer->SetFilePrefix( "register" );
    writer->Execute();


    }
  catch(...)
    {
     std::cout << "Error occured while writing PGM files." << std::endl;
     std::cout << "Please make sure path " << m_DirectoryName.c_str()
               << " is valid. " << std::endl;              
     throw;
    }

}


} // namespace itk

#endif
