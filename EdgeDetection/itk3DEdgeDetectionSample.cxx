/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itk3DEdgeDetectionSample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImageRegionIterator.h"
#include "itkLaplacianImageFilter.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkRawImageWriter.h"
#include "itkImageFileReader.h"

#include "itkSWSRawFileReader.h"
#include "itkmyRawFileWriter.h"





#include <fstream>
#include <string>

extern "C" {
#include <string.h>
#include <stdio.h>
}


main(int argc, char*argv[])
{
  
  typedef itk::Image<unsigned char, 3> UnsignedImageType;

  typedef itk::Image<float, 3> ImageType;

  //  typedef pgm_reader::ImageType ImageType;


  char  in[20], out[20];

  if( argc < 3)
    {
      cout<<"Usage: "<<argv[0]<<" inputImage outputImage"<<endl;
      exit(-1);
    }

  strcpy(in, argv[1]);
  strcpy(out,argv[2]);
  

  itk::sws::RawFileReader<UnsignedImageType>::Pointer reader 
    = itk::sws::RawFileReader<UnsignedImageType>::New();
  

  UnsignedImageType::RegionType requestedRegion;
  UnsignedImageType::SizeType  size;
  UnsignedImageType::IndexType index;


  //Set the size of the image region
  //If you want to try a 2D image, you need to change the dimension.
  size[0] = 256;
  size[1] = 128;
  size[2] = 10;
  
  index[0] = 0;
  index[1] = 0;
  index[2] = 0;

  requestedRegion.SetSize(size);
  requestedRegion.SetIndex(index);

  reader->SetFileName(in);
  reader->SetPixelSize(4);
  reader->SetRegion(requestedRegion);
  //cout<<"coming here"<<endl;
  reader->Update();


  UnsignedImageType::Pointer inPtr =  reader->GetOutput();


  ImageType::Pointer input = ImageType::New();

  input->SetLargestPossibleRegion(requestedRegion);
  input->SetRequestedRegion(requestedRegion);
  input->SetBufferedRegion(requestedRegion);
  input->Allocate();

  
  itk::ImageRegionIterator<UnsignedImageType>  uit(inPtr, inPtr->GetRequestedRegion());
  itk::ImageRegionIterator<ImageType>  it(input, input->GetRequestedRegion());

  uit.GoToBegin();
  it.GoToBegin();

  while(!uit.IsAtEnd())
    {
      it.Value() = (float)uit.Value();
      ++uit;
      ++it;
    }


  ImageType::Pointer output;

  //itk::ImageFileReader<ImageType>::Pointer  reader;
  //reader =  itk::ImageFileReader<ImageType>::New();
  //reader->SetFileName(in);
  //reader->Update();
  //ImageType::Pointer input = reader->GetOutput();
  
  float value[3];
  value[0] = 2.0f;
  value[1] = 2.0f;
  value[2] = 2.0f;

  
#ifdef LAPLACIAN

  cout<<"Laplacian filter test"<<endl;
  itk::DiscreteGaussianImageFilter<ImageType, ImageType>::Pointer
    gaussianFilter = itk::DiscreteGaussianImageFilter<ImageType, ImageType>::New();
  
  gaussianFilter->SetInput(input);
  gaussianFilter->SetVariance(value);


  itk::LaplacianImageFilter<ImageType, ImageType>::Pointer
    lapFilter = itk::LaplacianImageFilter<ImageType, ImageType>::New();

  
  lapFilter->SetInput(gaussianFilter->GetOutput());

   
  lapFilter->Update();

  output = lapFilter->GetOutput();
  cout<<output->GetLargestPossibleRegion();
#endif

#ifdef ZEROCROSSING
  itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::Pointer
    edgeFilter = itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  edgeFilter->SetInput(input);
  edgeFilter->SetVariance(value);
   
  edgeFilter->Update();
  output = edgeFilter->GetOutput();

  cout<<output->GetLargestPossibleRegion();
#endif

#ifdef SOBEL

  cout<<"sobel edge detection"<<endl;
  itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::Pointer sobelFilter
    = itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  sobelFilter->SetInput(input);
  sobelFilter->Update();
  output = sobelFilter->GetOutput();
#endif

  
#ifdef CANNY

  cout<<"Canny edge detection"<<endl;
  itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::Pointer cannyFilter
    = itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  cannyFilter->SetInput(input);
  cannyFilter->SetVariance(value);

  cannyFilter->Update();x
  output = cannyFilter->GetOutput();
#endif
  
  // Set up a .raw format writer.
  //  itk::RawImageWriter<ImageType>::Pointer writer =
  //  itk::RawImageWriter<ImageType>::New();
  //writer->SetFileName(out);
  //writer->SetInput(output);
  //writer->SetInput(input);
  //writer->Write();


  /*
  UnsignedImageType::Pointer outPtr = UnsignedImageType::New();


  outPtr->SetLargestPossibleRegion(requestedRegion);
  outPtr->SetRequestedRegion(requestedRegion);
  outPtr->SetBufferedRegion(requestedRegion);
  outPtr->Allocate();

  
  itk::ImageRegionIterator<UnsignedImageType>  oit(outPtr, outPtr->GetRequestedRegion());
  itk::ImageRegionIterator<ImageType>  iit(output, output->GetRequestedRegion());

  oit.GoToBegin();
  iit.GoToBegin();

  while(!iit.IsAtEnd())
    {
      oit.Value() = (unsigned char)iit.Value();
      ++oit;
      ++iit;
    }

  */

  itk::myRawFileWriter<ImageType>::Pointer  writer
    = itk::myRawFileWriter<ImageType>::New();

  writer->SetFileName(out);
  writer->SetInput(output);
  writer->Update();

}
