/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFloodFilledSpatialFunctionExample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*
This file demonstrates the various inclusion strategies available
to itkFloodFilledSpatialFunctionConditionalIterator. Please see
the README for a more detailed description.
*/

#include <stdio.h>

#include "itkSize.h"
#include "itkIndex.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkPoint.h"
#include "itkSphereSpatialFunction.h"
#include "itkFloodFilledSpatialFunctionConditionalIterator.h"

int main()
{
  const unsigned int dim = 2;

  // Image size and spacing parameters
  unsigned long sourceImageSize[]  = { 5, 5};
  double sourceImageSpacing[] = { 1.0,1.0 };
  double sourceImageOrigin[] = { 0,0 };

  // Image typedef
  typedef itk::Image< bool, dim > ImageType;

  // Creates the sourceImage (but doesn't set the size or allocate memory)
  ImageType::Pointer sourceImage = ImageType::New();
  sourceImage->SetOrigin(sourceImageOrigin);
  sourceImage->SetSpacing(sourceImageSpacing);

  // Create a size object native to the sourceImage type
  ImageType::SizeType sourceImageSizeObject;

  // Set the size object to the array defined earlier
  sourceImageSizeObject.SetSize( sourceImageSize );

  // Create a region object native to the sourceImage type
  ImageType::RegionType largestPossibleRegion;

  // Resize the region
  largestPossibleRegion.SetSize( sourceImageSizeObject );

  // Set the largest legal region size (i.e. the size of the whole sourceImage) to what we just defined
  sourceImage->SetLargestPossibleRegion( largestPossibleRegion );

  // Set the buffered region
  sourceImage->SetBufferedRegion( largestPossibleRegion );

  // Set the requested region
  sourceImage->SetRequestedRegion( largestPossibleRegion );

  // Now allocate memory for the sourceImage
  sourceImage->Allocate();

  // Loop over all available iterator strategies
  for(int strat = 0; strat < 4; strat++)
  {
    // Initialize the image to hold all 0's
    itk::ImageRegionIterator<ImageType> it = 
      itk::ImageRegionIterator<ImageType>(sourceImage, largestPossibleRegion);

    for(it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
      it.Set(0);
      }

    // Create and initialize a spatial function
    typedef itk::SphereSpatialFunction<dim> FunctionType;
    typedef FunctionType::InputType FunctionPositionType;

    FunctionType::Pointer spatialFunc = FunctionType::New();
    spatialFunc->SetRadius( 1.0 );

    FunctionPositionType center;
    center[0] = 2.5;
    center[1] = 2.5;
    spatialFunc->SetCenter(center);

    // Create and initialize a spatial function iterator
    ImageType::IndexType seedPos;
    const ImageType::IndexValueType pos[] = {2,2};
    seedPos.SetIndex(pos);

    typedef itk::FloodFilledSpatialFunctionConditionalIterator
      <ImageType, FunctionType> ItType;

    ItType sfi = ItType(sourceImage, spatialFunc, seedPos);

    switch(strat){
    case 0:
      {
        printf("Origin Inclusion Strategy\n");
        sfi.SetOriginInclusionStrategy();
      }
    break;
    case 1:
      {
        printf("Center Inclusion Strategy\n");
        sfi.SetCenterInclusionStrategy();
      }
    break;
    case 2:
      {
        printf("Complete Inclusion Strategy\n");
        sfi.SetCompleteInclusionStrategy();
      }
    break;
    case 3:
      {
        printf("Intersect Inclusion Strategy\n");
        sfi.SetIntersectInclusionStrategy();
      }
    } // end switch inclusion strategy
    
    // Iterate through the entire image and set interior pixels to 1
    for( sfi.GoToBegin(); !( sfi.IsAtEnd() ); ++sfi)
      {
      sfi.Set(1);
      }
  
    // Dump the image to the console
    ImageType::IndexType pix;

    for( int r = 0; r < 5; r++)
      {
      for( int c = 0; c < 5; c++)
        {
        ImageType::IndexValueType pos[] = {r,c};
        pix.SetIndex(pos);
        printf("%i ", sourceImage->GetPixel(pix) );
        }
      printf("\n");
      }
  } // end loop over iterator strategies

  return 0;
}
