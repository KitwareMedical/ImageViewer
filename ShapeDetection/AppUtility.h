/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    AppUtility.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
  typedef itk::ImageRegionIterator<ImageType> InputIterator;

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
