/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MetaImageReadWrite.cxx
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

#include <itkImageRegionIterator.h>

int main()
  {
  // Declare the image
  typedef itk::Image<short, 3> ImageType;

  ImageType::SizeType imSize = {{256, 256, 10}};
  ImageType::Pointer im = ImageType::New();
  im->SetRegions(imSize);
  im->Allocate();

  // Set some values in the image
  itk::ImageRegionIterator< ImageType > it(im, 
        im->GetLargestPossibleRegion());

  it.GoToBegin();
  int c = 0;
  while(!it.IsAtEnd())
    {
    it.Set( c++/1000 );
    ++it;
    }

  // Declare a Writer
  typedef itk::ImageFileWriter< ImageType > VolumeWriterType;
  VolumeWriterType::Pointer writer = VolumeWriterType::New();

  // Add the MetaImage format to the writer
  itk::MetaImageIO::Pointer metaWriter = itk::MetaImageIO::New();
  writer->SetImageIO( metaWriter );

  // Uncomment the next line to write the data to one file.
  // metaWriter->SetDataFileName( "LOCAL" );

  // Set the filename
  writer->SetFileName( "test.mhd" );

  // Set the image to write
  writer->SetInput( im );

  //   Update function.   Writers are the exception to that rule
  writer->Update();

  // Make sure the image in memory hasn't been corrupted by writing
  it.GoToBegin();
  c = 0;
  while(!it.IsAtEnd())
    {
    if(it.Get() != c++/1000)
      {
      std::cout << "Memory is " << it.Get() 
                << " expected " << c++/1000 << std::endl;
      }
    ++it;
    }

  // Declare a reader
  typedef itk::ImageFileReader< ImageType > VolumeReaderType;
  VolumeReaderType::Pointer reader = VolumeReaderType::New();

  // Set the name of the file to read
  reader->SetFileName( "test.mhd" );

  // Set the image into which the results should be stored
  ImageType::Pointer imOut;
  imOut = reader->GetOutput();
 
  // See if the file can be read - "try" otherwise program will 
  //   mysteriously exit on failure in the Object factory
  try
    {
    reader->Update();
    }
  catch( ... )
    {
    std::cout << "Problems reading file format" << std::endl;
    return 1;
    }

  // See if the data read is valid
  itk::ImageRegionIterator< ImageType > itOut(imOut, 
        imOut->GetLargestPossibleRegion());

  it.GoToBegin();
  itOut.GoToBegin();
  while(!it.IsAtEnd())
    {
    if(it.Get() != itOut.Get())
      {
      std::cout << "Read " << itOut.Get() 
                << " expected " << it.Get() << std::endl;
      }
    ++it;
    ++itOut;
    }

  return 1;
  }
