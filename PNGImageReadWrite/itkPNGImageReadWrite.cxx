/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPNGImageReadWrite.cxx
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
#include "itkPNGImageIO.h"



int main(int argc, char **argv)
{

  // Taking parameters from the command line

  if( argc < 4 ) 
    {
    std::cerr << std::endl;
    std::cerr << "Usage:  PNGImageReadWrite  inputImageFilePrefix   numberOfSlices   outputImageFilePrefix" << std::endl;
    std::cerr << std::endl;
    return -1;
    }

  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[3];

  const unsigned int nz = atoi( argv[2] );


  typedef  unsigned char    PixelType;
  const    unsigned int     Dimension = 3;

  typedef  itk::Image< PixelType, Dimension >     ImageType;



  // Read a PNG File
  typedef  itk::ImageFileReader< ImageType >      FileSourceType;
  typedef  itk::PNGImageIO                        PNGReaderType;


  FileSourceType::Pointer fileSource = FileSourceType::New();
  fileSource->SetFileName( inputFileName );

  PNGReaderType::Pointer  pngReader  = PNGReaderType::New();
  pngReader->SetNumberOfDimensions( 3 );

  fileSource->SetImageIO( pngReader );

  try
    {
    fileSource->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during PNG file reading " << std::endl;
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
    std::cerr << "Exception caught during PNG file writing " << std::endl;
    std::cerr << e << std::endl;
    return -1;
    }

  std::cout << "File succesfully writen ! " << std::endl;

  return 0;

}



