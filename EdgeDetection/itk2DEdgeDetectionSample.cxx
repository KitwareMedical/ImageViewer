/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itk2DEdgeDetectionSample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkLaplacianImageFilter.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkRawImageWriter.h"
#include "itkImageFileReader.h"

#include <fstream>
#include <string>

extern "C" {
#include <string.h>
#include <stdio.h>
}

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



void main(int argc, char*argv[])
{
  
  //typedef itk::Image<float, 2> ImageType;
    typedef pgm_reader::ImageType ImageType;

  char  in[20], out[20];

  if( argc < 3)
    {
      cout<<"Usage:"<<argv[0]<<"  inputImage outputImage"<<endl;
      exit(-1);
    }

  strcpy(in, argv[1]);
  strcpy(out,argv[2]);
  

  pgm_reader reader;
  ImageType::Pointer input = reader.read(in);
  ImageType::Pointer output;

  //itk::ImageFileReader<ImageType>::Pointer  reader;
  //reader =  itk::ImageFileReader<ImageType>::New();
  //reader->SetFileName(in);
  //reader->Update();
  //ImageType::Pointer input = reader->GetOutput();
  
  float value = 2.0f;
  //value[0] = 2.0f;
  //value[1] = 2.0f;

  //#define LAPLACIAN  
#ifdef LAPLACIAN
  //itk::DiscreteGaussianImageFilter<ImageType, ImageType>::Pointer
 //   gaussianFilter = itk::DiscreteGaussianImageFilter<ImageType,
//ImageType>::New();
  
  //gaussianFilter->SetInput(input);
  //gaussianFilter->SetVariance(value);


  itk::LaplacianImageFilter<ImageType, ImageType>::Pointer
    lapFilter = itk::LaplacianImageFilter<ImageType, ImageType>::New();

  lapFilter->SetInput(input);

  //lapFilter->SetInput(gaussianFilter->GetOutput());
  

  lapFilter->Update();

  itk::ZeroCrossingImageFilter<ImageType, ImageType>::Pointer
    zeroFilter = itk::ZeroCrossingImageFilter<ImageType, ImageType>::New();
  
  zeroFilter->SetInput(lapFilter->GetOutput());
  
  zeroFilter->Update();

  output = zeroFilter->GetOutput();
  //cout<<output->GetLargestPossibleRegion();
#endif
  
  //#define ZEROCROSSING
#ifdef  ZEROCROSSING
  itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::Pointer
    edgeFilter = itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  edgeFilter->SetInput(input);
  edgeFilter->SetVariance(value);
   
  edgeFilter->Update();
  output = edgeFilter->GetOutput();

  //cout<<output->GetLargestPossibleRegion();
#endif

#ifdef SOBEL
  itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::Pointer sobelFilter
    = itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  sobelFilter->SetInput(input);
  sobelFilter->Update();
  output = sobelFilter->GetOutput();
#endif

  //#define CANNY
#ifdef CANNY
  itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::Pointer cannyFilter
    = itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  cannyFilter->SetInput(input);
  cannyFilter->SetVariance(value);

  cannyFilter->Update();
  output = cannyFilter->GetOutput();
#endif
 
  // Set up a .raw format writer.
  itk::RawImageWriter<ImageType>::Pointer writer =
    itk::RawImageWriter<ImageType>::New();
  writer->SetFileName(out);
  writer->SetInput(output);
  
  writer->Write();

}
