/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWatershedSegmentationExample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkWatershedImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRawImageIO.h"
#include "itkImageFileWriter.h"
#include <fstream>
#include <string>
#include "itkCommand.h"

extern "C" {
#include <string.h>
#include <stdio.h>
}

namespace itk {
class PrintProgressCommand : public Command
{
public:
  /** Smart pointer declaration methods */
  typedef PrintProgressCommand Self;
  typedef Command Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  itkTypeMacro( PrintProgressCommand, Command );
  itkNewMacro(Self);

  /** Standard Command virtual methods */
  void Execute(Object *caller, const EventObject &event);
  void Execute(const Object *caller, const EventObject &event);

protected:
  PrintProgressCommand() {}
  virtual ~PrintProgressCommand() {}
  
private:
};
  
void PrintProgressCommand
::Execute(Object *caller, const EventObject &event)
{
  ProcessObject *po = dynamic_cast<ProcessObject *>(caller);
  if (! po) return;
  
  if( typeid(event) == typeid ( ProgressEvent)  )
    { 
      std::cout << ".";
      std::cout.flush();
    }
}

void PrintProgressCommand
::Execute(const Object *caller, const EventObject &event)
{
  ProcessObject *po = dynamic_cast<ProcessObject *>(const_cast<Object *>(caller));
  if (! po) return;
  
  if( typeid(event) == typeid ( ProgressEvent)  )
    {
      std::cout << ".";
      std::cout.flush();
    }
}
}// namespace itk


/**
 * The following is a very simple (& slow) pgm reader
 * that generates an itk::Image from an ascii
 * (P2) pgm file.
 */
class pgm_reader
{
  void getln(std::ifstream &in, char *buf) const;
  
public:
  typedef itk::Image<float, 2> ImageType;
  static inline void die(char *s) { std::cerr << s << std::endl; exit(1); }
  static unsigned int m_BufferSize;
  pgm_reader() {}
  ImageType::Pointer read(const char *fn) const;
};

unsigned int pgm_reader::m_BufferSize = 256;

pgm_reader::ImageType::Pointer pgm_reader::read(const char *fn) const
{
  unsigned int x, y, col_depth, tmp;
  char buf[256];

  // Open input file
  std::ifstream in;
  in.open(fn);
  if (! in ) die ("Could not open input image for reading.");

  // Read the PGM header
  getln(in, buf);
  if ( strcmp(buf, "P2") != 0 )
    die ("Input file format not recognized, must be PGM ascii (P2)");
  getln(in, buf);
  sscanf(buf, "%d%d", &x, &y);
  std::cout << "IMAGE DIMENSIONS = " << x << " " << y << std::endl;
  getln(in, buf);
  sscanf(buf, "%d", &col_depth);

  // Eat up any additional comment lines
  while ( (char)(in.peek()) == '#' )  getln(in,buf);

  // Construct an itkImage
  ImageType::Pointer input_image = ImageType::New();
   ImageType::RegionType region;
   ImageType::IndexType  idx;
   ImageType::SizeType    sz;
   idx[0] = idx[1] = 0;  // start index for the region to be processed
   sz[0] = x; sz[1] = y; // dimensions of the region to be processed
   region.SetIndex(idx);
   region.SetSize(sz);
   input_image->SetLargestPossibleRegion(region);
   input_image->SetBufferedRegion(region);
   input_image->SetRequestedRegion(region);
   input_image->Allocate();
  
  // Read data into this image
  itk::ImageRegionIterator<ImageType>
    it(input_image, input_image->GetRequestedRegion());
  it = it.Begin();
  
  while ( !in.eof() && !it.IsAtEnd() )
    {
      in >> tmp;
      it.Value() = (float)tmp;
      ++it;
    }
  in.close();
  return input_image;
}
 
void
pgm_reader::getln(std::ifstream &in, char *buf) const
{
  char *c;
  buf[0] = '\0';  // clear the buffer
  while( (strcmp(buf,"") == 0) && !in.eof() )
    {
      in.getline(buf, 256, '\n');
      for (c=buf; *c != '#' && *c != '\0'; c++);  // strip out comments
      *c = '\0';
    }
}

int main(int argc, char *argv[])
{
  float conductance_term; 
  float lower_threshold; 
//  float flood_level;
  unsigned int diffusion_iterations;
  typedef pgm_reader::ImageType ImageType;
  typedef itk::Image<unsigned long, 2> UnsignedImageType;
  std::string outname;
  itk::PrintProgressCommand::Pointer c = itk::PrintProgressCommand::New();

  if (argc != 6)
  pgm_reader::die ("Use: watershed input_file output_file_prefix conductance_term diffusion_iterations lower_threshold");
  sscanf(argv[3], "%f", &conductance_term);
  sscanf(argv[4], "%d", &diffusion_iterations);
  sscanf(argv[5], "%f", &lower_threshold);
  //  sscanf(argv[6], "%f", &flood_level);
  
  pgm_reader reader;
  ImageType::Pointer input_image = reader.read(argv[1]);
  
  // Set up the file writer
  itk::RawImageIO<unsigned long, 2>::Pointer rawio = itk::RawImageIO<unsigned long, 2>::New();
  rawio->SetByteOrderToLittleEndian();
  rawio->SetFileDimensionality(2);
  rawio->SetFileTypeToBinary();
  itk::ImageFileWriter<UnsignedImageType>::Pointer writer =
    itk::ImageFileWriter<UnsignedImageType>::New();
  writer->SetImageIO(rawio);
  
  // Set up an anisotropic diffusion image filter.
  itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType>::Pointer
    diffusion = itk::CurvatureAnisotropicDiffusionImageFilter<ImageType,
    ImageType>::New();
  diffusion->SetTimeStep(0.125f);
  diffusion->SetIterations(diffusion_iterations);
  diffusion->SetConductanceParameter(conductance_term);
  diffusion->SetInput(input_image);

  // Set up a gradient magnitude image filter.
  itk::GradientMagnitudeImageFilter<ImageType, ImageType>::Pointer magnitude
    = itk::GradientMagnitudeImageFilter<ImageType, ImageType>::New();
  magnitude->SetInput(diffusion->GetOutput());

  // Set up the watershed image filter.
  itk::WatershedImageFilter<ImageType>::Pointer watershed
    = itk::WatershedImageFilter<ImageType>::New();
  watershed->SetThreshold(lower_threshold);
  watershed->SetLevel(0.50);
  watershed->SetInput(magnitude->GetOutput());
  watershed->AddObserver(itk::ProgressEvent(), c);

  std::cout << "Calculating at Level 0.50";
  outname = std::string(argv[2]) + "0.50.raw";
  writer->SetFileName(outname.c_str());
  writer->SetInput(watershed->GetOutput());
  
  // Execute the pipeline and spit out the results.
  writer->Write();
  
  // Now modify the level parameter and spit out a
  // range of labeled images at differing levels
  // in the segmentation hierarchy
  std::cout << std::endl;
  std::cout << "Calculating at Level 0.40";
  watershed->SetLevel(0.40);
  outname = std::string(argv[2]) + "0.40.raw";
  writer->SetFileName(outname.c_str());
  writer->Write();

  std::cout << std::endl;
  std::cout << "Calculating at Level 0.30";
  watershed->SetLevel(0.30);
  outname = std::string(argv[2]) + "0.30.raw";
  writer->SetFileName(outname.c_str());
  writer->Write();

  std::cout << std::endl;
  std::cout << "Calculating at Level 0.20";
  watershed->SetLevel(0.20);
  outname = std::string(argv[2]) + "0.20.raw";
  writer->SetFileName(outname.c_str());
  writer->Write();

  std::cout << std::endl;
  std::cout << "Calculating at Level 0.10";
  watershed->SetLevel(0.10);
  outname = std::string(argv[2]) + "0.10.raw";
  writer->SetFileName(outname.c_str());
  writer->Write();

  std::cout << std::endl;
  std::cout << "Calculating at Level 0.05";
  watershed->SetLevel(0.05);
  outname = std::string(argv[2]) + "0.05.raw";
  writer->SetFileName(outname.c_str());
  writer->Write();
  std::cout << std::endl;
  return 0;
}
