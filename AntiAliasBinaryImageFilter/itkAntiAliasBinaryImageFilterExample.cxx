/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkAntiAliasBinaryImageFilterExample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAntiAliasBinaryImageFilter.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkRawImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "param.h"
#include "itkImageRegionIterator.h"
#include <string>

void die(char *s)
{
  std::cerr << s << std::endl;
  exit(-1);
}

//
// This example demonstrates how to apply the AntiAliasBinaryImageFilter to a
// binary volume.  See the file README.txt for more information.
//
int main(int argc, char *argv[] )
{
  //
  // Change this typedef and recompile to read different image types.
  //
  typedef char InputDataType;
  
  typedef itk::Image<InputDataType, 3> BinaryImageType;
  typedef itk::Image<float, 3> RealImageType;
  
  itk::RawImageIO<InputDataType, 3>::Pointer input_io
    = itk::RawImageIO<InputDataType, 3>::New();

  itk::RawImageIO<float, 3>::Pointer output_io
    = itk::RawImageIO<float, 3>::New();

  itk::ImageFileReader<BinaryImageType>::Pointer reader
    = itk::ImageFileReader<BinaryImageType>::New();

  itk::ImageFileWriter<RealImageType>::Pointer writer
    = itk::ImageFileWriter<RealImageType>::New();

  unsigned long size[3];

  float max_error;
  unsigned int i;
  unsigned int number_of_layers;
  int output_type;

  std::string input_filename, output_filename;

  itk::AntiAliasBinaryImageFilter<BinaryImageType, RealImageType>::Pointer
    antialiaser = itk::AntiAliasBinaryImageFilter<BinaryImageType, RealImageType>::New();

  itk::ZeroCrossingImageFilter<RealImageType, RealImageType>::Pointer
    zerocrossings = itk::ZeroCrossingImageFilter<RealImageType, RealImageType>::New();

  // Open and parse the parameter file.
  if (argc != 2) die("usage: AntiAliasBinaryImageFilterExample parameter_file");
  VPF::ParameterFile pf(argv[1]);
  for (i = 0; i < 3; ++i)
    {
      if (VPF::set(size[i] , pf["IMAGE_SIZE"][i]) == VPF::INVALID)
        die ("IMAGE_SIZE not fully specified");
    }
  if (VPF::set(input_filename, pf["INPUT_FILENAME"][0]) == VPF::INVALID)
    die ("INPUT_FILENAME not specified");
  if (VPF::set(output_filename, pf["OUTPUT_FILENAME"][0]) == VPF::INVALID)
    die ("OUTPUT_FILENAME not specified");
  if (VPF::set(max_error, pf["MAXIMUM_ERROR"][0]) == VPF::INVALID)
    die ("MAXIMUM_ERROR not specified");
  if (VPF::set(number_of_layers, pf["NUMBER_OF_LAYERS"][0]) == VPF::INVALID)
    die ("NUMBER_OF_LAYERS not specified");
  if (VPF::set(output_type, pf["OUTPUT_TYPE"][0]) == VPF::INVALID)
    die ("OUTPUT_TYPE not specified. 0 level set, 1 zero crossings");

  // Construct and run the pipeline.
  try {
      
    // Set up the io classes and the filter
    input_io->SetDimensions(0, size[0]);
    input_io->SetDimensions(1, size[1]);
    input_io->SetDimensions(2, size[2]);
    input_io->SetFileTypeToBinary();
    input_io->SetByteOrderToLittleEndian();
    input_io->SetFileDimensionality(3);
    input_io->SetNumberOfComponents(1);
    
    reader->SetImageIO(input_io);
    reader->SetFileName(input_filename.c_str());
    
    antialiaser->SetInput(reader->GetOutput());
    antialiaser->SetMaximumRMSError(max_error);
    antialiaser->SetNumberOfLayers(number_of_layers);

    output_io->SetByteOrderToLittleEndian();
    output_io->SetFileTypeToBinary();
    output_io->SetFileDimensionality(3);

    if (output_type == 1)  // output zero crossings of the level set image
      {
        zerocrossings->SetInput(antialiaser->GetOutput()); 
        zerocrossings->SetForegroundValue(1.0);
        zerocrossings->SetBackgroundValue(-1.0);

        writer->SetImageIO(output_io);
        writer->SetFileName(output_filename.c_str());
        writer->SetInput(zerocrossings->GetOutput());
        writer->Write();
      }
    else // output the level set image
      {
        writer->SetImageIO(output_io);
        writer->SetFileName(output_filename.c_str());
   
        writer->SetInput(antialiaser->GetOutput());
        writer->Write();
      }

    std::cout << "Last RMS change value was: "
              << antialiaser->GetRMSChange() << std::endl;
    
  }
  catch (itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      die ("Exception.  Abort.");
    }
    
  return 0;
}
