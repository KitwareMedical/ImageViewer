/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWSRawToPNG.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//
// This file converts .raw output from itkWatershedSegmentationExample to a
// full color .png file which can be viewed with standard image viewing
// software.   The input should be unsigned long integers.
//
// Integer labels from the input are converted to random RGB colors.  This is
// not necessarily the most effective way to visualize segmented output, but it
// usually gives pretty good results.  You can always rerun the program on a
// file if you don't like the particular colors chosen.
//

#include "itkRawImageIO.h"
#include "itkPNGImageIO.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkMinimumMaximumImageCalculator.h"
extern "C" {
#include <stdlib.h>
}
#include <string>

int main( int argc, char **argv )
{
  typedef itk::Image<unsigned long, 2> InputImageType;
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef itk::Image<RGBPixelType,2>   RGBImageType;

  const unsigned int color_depth = 255;
  RGBPixelType *lut;
  
  if (argc < 4)
    {
      std::cerr << "Usage: itkWSRawToPNG input_file_name x_size y_size <random number seed>" << std::endl;
      return 1;
    }
  
  // Set up reader.
  itk::RawImageIO<unsigned long, 2>::Pointer input_io
    = itk::RawImageIO<unsigned long, 2>::New();
  input_io->SetByteOrderToLittleEndian();
  input_io->SetFileDimensionality(2);
  input_io->SetFileTypeToBinary();
  input_io->SetDimensions(0, ::atoi(argv[2]));
  input_io->SetDimensions(1, ::atoi(argv[3]));
  
  itk::ImageFileReader<InputImageType>::Pointer reader
    = itk::ImageFileReader<InputImageType>::New();
 
  reader->SetImageIO(input_io);
  reader->SetFileName(argv[1]);

  // Set up writer
  itk::PNGImageIO::Pointer output_io = itk::PNGImageIO::New();
  itk::ImageFileWriter<RGBImageType>::Pointer writer
    = itk::ImageFileWriter<RGBImageType>::New();
  writer->SetImageIO(output_io);

  // Read the image
  reader->Update();

  // Create the color lookup table
  itk::MinimumMaximumImageCalculator<InputImageType>::Pointer minmax
    = itk::MinimumMaximumImageCalculator<InputImageType>::New();
  minmax->SetImage (reader->GetOutput());
  minmax->ComputeMinimum();
  minmax->ComputeMaximum();

  unsigned int table_size = minmax->GetMaximum() - minmax->GetMinimum();
  
  lut = new RGBPixelType[ table_size ];
  
  if (argc > 4)
    {
      ::srand(static_cast<unsigned int>(::atoi(argv[4])));
    }

  for (unsigned i = 0; i < table_size; ++i)
    {
      lut[i].SetRed( static_cast<char>( static_cast<unsigned int> (
                                            color_depth * (::rand() / (RAND_MAX + 1.0)) ) ) );
      lut[i].SetBlue( static_cast<char>( static_cast<unsigned int> (
                                            color_depth * (::rand() / (RAND_MAX + 1.0)) ) ) );
      lut[i].SetGreen( static_cast<char>( static_cast<unsigned int> (
                                            color_depth * (::rand() / (RAND_MAX + 1.0)) ) ) );
    }

  
  // Map the input values into RGB values
  RGBImageType::Pointer output_image = RGBImageType::New();
  output_image->SetRegions(reader->GetOutput()->GetRequestedRegion());
  output_image->Allocate();
  
  itk::ImageRegionIterator<RGBImageType>
    rgb_it(output_image, output_image->GetRequestedRegion());
  itk::ImageRegionConstIterator<InputImageType>
    input_it(reader->GetOutput(), output_image->GetRequestedRegion());

  for (; !input_it.IsAtEnd(); ++input_it, ++rgb_it)
    {      rgb_it.Set(lut[ input_it.Get() - minmax->GetMinimum() ]);   }
  delete [] lut;

  // Write the RGB image in .png format
  writer->SetInput(output_image);
  std::string output_fn = std::string(argv[1]) + ".png";
  writer->SetFileName(output_fn.c_str());
  writer->Write();
  return 0;
}
