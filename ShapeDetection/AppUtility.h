/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    AppUtility.h
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
#ifndef __AppUtility_h
#define __AppUtility_h

#include <stdio.h>
#include <fstream>

template <class TImage>
bool
WritePGMFiles(
typename TImage::Pointer input, 
const char * dirname,
const char * basename )
{

  // go through the image and compute the offset and scale
  // to make it normalized to between 0 and 255
  typedef TImage ImageType;
  typedef typename TImage::PixelType PixelType;
  typedef itk::ImageRegionIterator<ImageType>
   InputIterator;

  InputIterator inIter( input, input->GetBufferedRegion() );

  PixelType minValue = inIter.Get();
  PixelType maxValue = minValue;
  while( !inIter.IsAtEnd() )
    {
    PixelType value = inIter.Get();
    if( value < minValue ) minValue = value;
    if( value > maxValue ) maxValue = value;
    ++inIter;
    }

  double scale = double( maxValue - minValue );
  if( !scale ) scale = 1.0;
  double offset = double(minValue);
  
  // write image out to pgm files
  char filename[256];
  char buffer[50];
  unsigned long ncol = input->GetBufferedRegion().GetSize()[0];
  unsigned long nrow = input->GetBufferedRegion().GetSize()[1];
  unsigned long nslice = input->GetBufferedRegion().GetSize()[2];

  sprintf(buffer,"P5 %ld %ld 255\n", ncol, nrow );
  unsigned int nchar = strlen(buffer);
  unsigned long npixels = nrow * ncol;

  inIter.GoToBegin();

  for( unsigned int k = 0; k < nslice; k++ )
    {
    if( k < 10 )
      {
      sprintf(filename,"%s/%s00%d.pgm", dirname, basename, k );
      }
    else if( k < 100 )
      {
      sprintf(filename, "%s/%s0%d.pgm", dirname, basename, k );
      }
    else
      {
      sprintf(filename, "%s/%s%d.pgm", dirname, basename, k );
      }

    // open up the stream  
    std::ofstream imgStream( filename, std::ios::out | std::ios::binary );
  
    if( !imgStream.is_open() )
      {
      return false;
      }

    // writer the header
    imgStream.write( buffer, nchar );

    // write the bytes
    for( unsigned long i = 0; i < npixels; i++ )
      {
      double value = (double(inIter.Get()) - offset) / scale * 255;
      char num = vnl_math_rnd( value );
      imgStream.put( num );
      ++inIter;
      }

    // close this file
    imgStream.close();
    
    }

  return true;
}

#endif
