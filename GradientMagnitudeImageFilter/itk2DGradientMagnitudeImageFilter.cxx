#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkPNGImageIO.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkCastImageFilter.h"
#include <iostream>

int main(int argc, char *argv[]) {

  // Test for command line arguments.
  if (argc != 3)
    {
      std::cerr << "Usage: " << argv[0]
                << "input_filename output_filename" << std::endl;
      return 1;
    }

  // Convenient typedefs
  typedef itk::Image<float, 2> RealImageType;
  typedef itk::Image<unsigned char, 2>  CharImageType;
  typedef itk::CastImageFilter<CharImageType, RealImageType> CastToRealFilterType;
  typedef itk::CastImageFilter<RealImageType, CharImageType> CastToCharFilterType;
  typedef itk::GradientMagnitudeImageFilter<RealImageType, RealImageType>
    GradientFilterType;

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

  // Create the gradient filter
  GradientFilterType::Pointer gradient = GradientFilterType::New();

  // Connect the pipeline
  to_real->SetInput(reader->GetOutput());
  gradient->SetInput(to_real->GetOutput());
  to_char->SetInput(gradient->GetOutput());
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
