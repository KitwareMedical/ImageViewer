#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkPNGImageIO.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkCastImageFilter.h"
#include <iostream>
extern "C" {
#include <stdio.h>
}

int main(int argc, char *argv[]) {

  // Test for command line arguments.
  if (argc != 5)
    {
      std::cerr << "Usage: " << argv[0]
                << "input_filename output_filename iterations conductance" << std::endl;
      return 1;
    }

  // Grab the number of iterations and conductance term
  int iterations;
  float conductance;
  ::sscanf(argv[3], "%d", &iterations);
  ::sscanf(argv[4], "%f", &conductance);

  // Convenient typedefs
  typedef itk::Image<float, 2> RealImageType;
  typedef itk::Image<unsigned char, 2>  CharImageType;
  typedef itk::CastImageFilter<CharImageType, RealImageType> CastToRealFilterType;
  typedef itk::CastImageFilter<RealImageType, CharImageType> CastToCharFilterType;
  typedef itk::GradientAnisotropicDiffusionImageFilter<RealImageType, RealImageType>
    DiffusionFilterType;

  // Create a file IO object for reading/writing PNG.
  itk::PNGImageIO::Pointer io = itk::PNGImageIO::New();
  io->SetNumberOfDimensions(2);

  // Create a file reader.
  itk::ImageFileReader<CharImageType>::Pointer reader
    = itk::ImageFileReader<CharImageType>::New();
  reader->SetImageIO(io);
  reader->SetFileName(argv[1]);

  // Create a file writer
  itk::ImageFileWriter<CharImageType>::Pointer writer
    = itk::ImageFileWriter<CharImageType>::New();
  writer->SetImageIO(io);
  writer->SetFileName(argv[2]);

  // Create the casting filters
  CastToRealFilterType::Pointer to_real = CastToRealFilterType::New();
  CastToCharFilterType::Pointer to_char = CastToCharFilterType::New();

  // Create the diffusion filter
  DiffusionFilterType::Pointer diffusion = DiffusionFilterType::New();
  diffusion->SetTimeStep(0.25);
  diffusion->SetIterations(iterations);
  diffusion->SetConductanceParameter(conductance);

  // Connect the pipeline
  to_real->SetInput(reader->GetOutput());
  diffusion->SetInput(to_real->GetOutput());
  to_char->SetInput(diffusion->GetOutput());
  writer->SetInput(to_char->GetOutput());

  // Execute the pipeline
  try {
    writer->Write();
  }
  catch(itk::ExceptionObject &e)
    {
      std::cerr << "Caught ITK exception: " << e << std::endl;
      return 1;
    }
  
  return 0;
}
