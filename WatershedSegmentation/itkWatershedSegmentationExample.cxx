/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWatershedSegmentationExample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "itkWatershedImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRawImageWriter.h"
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
  void getln(ifstream &in, char *buf) const;
  
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
  char buf[m_BufferSize];

  // Open input file
  ifstream in;
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
pgm_reader::getln(ifstream &in, char *buf) const
{
  char *c;
  buf[0] = '\0';  // clear the buffer
  while( (strcmp(buf,"") == 0) && !in.eof() )
    {
      in.getline(buf, m_BufferSize, '\n');
      for (c=buf; *c != '#' && *c != '\0'; c++);  // strip out comments
      *c = '\0';
    }
}

int main(int argc, char *argv[])
{
  float conductance_term, lower_threshold, flood_level;
  unsigned int diffusion_iterations;
  typedef pgm_reader::ImageType ImageType;

  
  if (argc != 7)
    pgm_reader::die ("Use: watershed input_file output_file conductance_term diffusion_iterations lower_threshold flood_level");
  sscanf(argv[3], "%f", &conductance_term);
  sscanf(argv[4], "%d", &diffusion_iterations);
  sscanf(argv[5], "%f", &lower_threshold);
  sscanf(argv[6], "%f", &flood_level);
  
  pgm_reader reader;
  ImageType::Pointer input_image = reader.read(argv[1]);

  // Set up an anisotropic diffusion image filter.
  itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType>::Pointer
    diffusion = itk::GradientAnisotropicDiffusionImageFilter<ImageType,
    ImageType>::New();
  diffusion->SetTimeStep(0.125f);
  diffusion->SetIterations(diffusion_iterations);
  diffusion->SetConductanceParameter(conductance_term);
  diffusion->SetNumberOfThreads(2);
  diffusion->SetInput(input_image);
  
  // Set up a gradient magnitude image filter.
  itk::GradientMagnitudeImageFilter<ImageType, ImageType>::Pointer magnitude
    = itk::GradientMagnitudeImageFilter<ImageType, ImageType>::New();
  magnitude->SetInput(diffusion->GetOutput());
  
  // Set up the watershed image filter.
  itk::WatershedImageFilter<ImageType, ImageType>::Pointer watershed
    = itk::WatershedImageFilter<ImageType, ImageType>::New();
  watershed->SetThreshold(lower_threshold);
  watershed->SetLevel(flood_level);
  watershed->SetInput(magnitude->GetOutput());
  
  // Set up a .raw format writer.
  itk::RawImageWriter<ImageType>::Pointer writer =
    itk::RawImageWriter<ImageType>::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(watershed->GetOutput());
  
  // Execute the pipeline and spit out the results.
  writer->Write();
  
  return 0;
}
