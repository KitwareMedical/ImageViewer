/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ExtractImageRegion.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>

#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkMetaImageIO.h>
#include <itkExtractImageFilter.h>

int main(int argc, char **argv)
  {

  if( argc < 9 )
    { 
    std::cerr << "Usage: ExtractImageRegion inputImageFile outputImageFile ";
    std::cerr << "xStart yStart zStart  xSize ySize zSize " << std::endl;
    return 1;
    }

  // Declare the image
  typedef itk::Image<unsigned short, 3> ImageType;


  // Declare a Reader
  typedef itk::ImageFileReader< ImageType > VolumeReaderType;
  VolumeReaderType::Pointer reader = VolumeReaderType::New();

  // Set the filename
  reader->SetFileName( argv[1] );

  // Declare the Extract filter
  typedef itk::ExtractImageFilter< ImageType, ImageType > ExtractFilterType;
  ExtractFilterType::Pointer extractor = ExtractFilterType::New();

  extractor->SetInput( reader->GetOutput() );

  ImageType::SizeType  size;
  ImageType::IndexType index;

  index[0] = atoi( argv[3] );
  index[1] = atoi( argv[4] );
  index[2] = atoi( argv[5] );

  size[0] = atoi( argv[6] );
  size[1] = atoi( argv[7] );
  size[2] = atoi( argv[8] );

  ImageType::RegionType region;
  region.SetIndex( index );
  region.SetSize( size );

  extractor->SetExtractionRegion( region );

  // Declare a Writer
  typedef itk::ImageFileWriter< ImageType > VolumeWriterType;
  VolumeWriterType::Pointer writer = VolumeWriterType::New();

  // Add the MetaImage format to the writer
  writer->SetImageIO( itk::MetaImageIO::New() );

  // Set the filename
  writer->SetFileName( argv[2] );

  // Set the image to write
  writer->SetInput( extractor->GetOutput() );

  // Write the image 
  writer->Update();

  }
