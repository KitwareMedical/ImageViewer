/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSampleDataGenerator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <fstream>
#include "itkImage.h"
#include "itkRawImageIO.h"
#include "itkImageFileWriter.h"

const unsigned int V_WIDTH  = 64;
const unsigned int V_HEIGHT = 64;
const unsigned int V_DEPTH  = 64;

float sphere(float x, float y, float z)
{
  float dis;
  dis = (x - (float)V_WIDTH/2.0)*(x - (float)V_WIDTH/2.0)
    /((0.2f*V_WIDTH)*(0.2f*V_WIDTH)) + 
    (y - (float)V_HEIGHT/2.0)*(y - (float)V_HEIGHT/2.0)
    /((0.2f*V_HEIGHT)*(0.2f*V_HEIGHT)) + 
    (z - (float)V_DEPTH/2.0)*(z - (float)V_DEPTH/2.0)
    /((0.2f*V_DEPTH)*(0.2f*V_DEPTH));

  return(1.0f-dis);
}

void evaluate_function(itk::Image<char, 3> *im,
          float (*f)(float, float, float) )
  
{
  itk::Image<char, 3>::IndexType idx;

  for(unsigned int z = 0; z < V_DEPTH; ++z)
    {
      idx[2] = z;
      for (unsigned int y = 0; y < V_HEIGHT; ++y)
        {
          idx[1] = y;
          for (unsigned int x = 0; x < V_WIDTH; ++x)
            {
              idx[0] = x;
              if ( f((float)x,(float)y,(float)z) >= 0.0 )
                {  im->SetPixel(idx, 1 ); }
              else
                {  im->SetPixel(idx, 0 ); }
            }
        }
    }
}


int main()
{
  typedef itk::Image<char, 3> BinaryImageType;
  
  // Create a binary image of a sphere.
  BinaryImageType::Pointer image = BinaryImageType::New();
  BinaryImageType::RegionType region;
  BinaryImageType::RegionType::SizeType sz;
  BinaryImageType::RegionType::IndexType idx;
  for (unsigned k = 0; k < 3; ++k)
    {
      sz[k] = 64;
      idx[k] = 0;
    }

  region.SetSize(sz);
  region.SetIndex(idx);
  image->SetRegions(region);
  image->Allocate();
  evaluate_function(image, sphere);

  // Write to a file.
  itk::RawImageIO<char, 3>::Pointer output_io = itk::RawImageIO<char, 3>::New();
  output_io->SetFileTypeToBinary();
  output_io->SetByteOrderToLittleEndian();
  output_io->SetFileDimensionality(3);

  itk::ImageFileWriter<BinaryImageType>::Pointer writer =
    itk::ImageFileWriter<BinaryImageType>::New();
  writer->SetImageIO(output_io);
  writer->SetFileName("sample_input.64x64x64.char");
  writer->SetInput(image);
  writer->Write();
  return 0;
}
