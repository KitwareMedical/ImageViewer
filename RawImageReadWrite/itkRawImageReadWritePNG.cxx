/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRawImageReadWritePNG.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkImage.h"

// Include generic Input Source
#include "itkImageFileReader.h"

// Incluide generic Output Sink
#include "itkImageFileWriter.h"

// Include file-format specific readers/writers
#include "itkRawImageIO.h"
#include "itkPNGImageIO.h"



int main(int argc, char **argv)
{

  // Taking parameters from the command line

  if( argc < 6 ) 
    {
    std::cerr << std::endl;
    std::cerr << "Usage:  RawImageReadWritePNG  inputImageFile.raw   sizeX   sizeY   sizeZ   outputImageFilesPrefix" << std::endl;
    std::cerr << std::endl;
    return -1;
    }

  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[5];

  const unsigned int nx = atoi( argv[2] );
  const unsigned int ny = atoi( argv[3] );
  const unsigned int nz = atoi( argv[4] );


  typedef  unsigned char    PixelType;
  const    unsigned int     Dimension = 3;

  typedef  itk::Image< PixelType, Dimension >     ImageType;



  // Read a Raw File
  typedef  itk::ImageFileReader< ImageType >      FileSourceType;
  typedef  itk::RawImageIO<PixelType,Dimension>   RawReaderType;


  FileSourceType::Pointer fileSource = FileSourceType::New();
  fileSource->SetFileName( inputFileName );

  RawReaderType::Pointer  rawReader  = RawReaderType::New();
  rawReader->SetFileDimensionality( 3 );
  rawReader->SetDimensions( 0, nx );
  rawReader->SetDimensions( 1, ny );
  rawReader->SetDimensions( 2, nz );

  fileSource->SetImageIO( rawReader );

  try
    {
    fileSource->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during Raw file reading " << std::endl;
    std::cerr << e << std::endl;
    return -1;
    }
  
  std::cout << "File succesfully read ! " << std::endl;


  // Write a PNG Files
  typedef  itk::ImageFileWriter< ImageType >      FileSinkType;
  typedef  itk::PNGImageIO                        PNGWriterType;


  FileSinkType::Pointer   fileSink   = FileSinkType::New();
  PNGWriterType::Pointer  pngWriter  = PNGWriterType::New();

  fileSink->SetImageIO( pngWriter );
  fileSink->SetFilePrefix( outputFileName );
  fileSink->SetInput( fileSource->GetOutput() );

  
  try
    {
    fileSink->Write();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during Raw file writing " << std::endl;
    std::cerr << e << std::endl;
    return -1;
    }

  std::cout << "File succesfully writen ! " << std::endl;

  return 0;

}



