/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPrincipalAxesResampler.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


/*--------------------------------------------------------------------------
This will eventually be a program to read an image file, find its
principal axes, and resample the image in principal axes coordinates.
For the moment, though, we're happy just to be able to compile it
within Insight.
---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "itkAffineTransform.h"
#include "itkImage.h"
#include "itkImageMoments.h"
#include "itkImageRegionSimpleIterator.h"


enum {NDimensions = 3};

typedef unsigned char                       PixelType;
typedef itk::Image<PixelType, NDimensions>  ImageType;
typedef ImageType::RegionType               ImageRegionType;
typedef ImageType::SizeType                 ImageSizeType;

typedef itk::Index<NDimensions>             ImageIndexType;
typedef itk::ImageRegionSimpleIterator<ImageType>  ImageIteratorType;
typedef itk::ImageMoments<PixelType, NDimensions>  ImageMomentsType;


int
main(int argc, char *argv[])
{
    /* Tell user I'm here! */
    printf("Hello, world!\n");

    /* Set image parameters */
    long ImageWidth     = 256;
    long ImageHeight    = 256;
    long NumberOfSlices = 192;

    /* Open image file and read image */
    /* FIXME: Translate into C++ style */
    if (argc < 2)
        {
            fprintf(stderr, "You must give the input file name\n");
            exit(EXIT_FAILURE);
        }
    char *Filename = argv[1];
    FILE *file = fopen(Filename, "rb");
    if (file == 0) 
        {
            fprintf(stderr, "Unable to open input file\n");
            exit(EXIT_FAILURE);
        }

    /* Allocate an image object to store the input file in */
    ImageIndexType base = {{0,0,0}};
    ImageSizeType  size = {{ImageWidth, ImageHeight, NumberOfSlices}};
    ImageType::Pointer image = ImageType::New();
    ImageRegionType region;
    region.SetIndex(base);
    region.SetSize(size);
    image->SetLargestPossibleRegion(region);
    image->SetBufferedRegion(region);
    image->Allocate();

    /* Read the image file into an itkImage object */
    /* FIXME: Find or write Insightful tools for this */
    std::cout << "Reading image file." << std::endl;
    ImageIndexType index;        // Index to current pixel
    long point[3];               // Location of current pixel
    unsigned char bytes[2];      // Upper and lower bytes
    unsigned short value;
    size_t count;
    for (long slice = 0; slice < NumberOfSlices; slice++) {
        point[2] = slice;
        for (long row = 0; row < ImageHeight; row++) {
            point[1] = row;
            for (long col = 0; col < ImageWidth; col++) {
                point[0] = col;
                index.SetIndex(point);
                count = fread(bytes, 1, 2, file);
                if (count != 2) {
                    fprintf(stderr, "Error reading input file\n");
                    exit(EXIT_FAILURE); }
                // Assume little endian data
                value = bytes[1]<<8 + bytes[0];
                image->SetPixel(index, value);
            }
        }
    }
    

#if 0    // Broken version using iterators
    region = image->GetRequestedRegion();
    ImageIteratorType iterator(image, region);
    unsigned char bytes[2];      // Upper and lower bytes
    size_t count;
    unsigned short value;
    for (iterator.Begin(); !iterator.IsAtEnd(); ++iterator)
        {
            count = fread(bytes, 1, 2, file);
            if (count != 2) {
                fprintf(stderr, "Error reading input file\n");
                exit(EXIT_FAILURE); }
            // Assume little endian data
            value = bytes[1]<<8 + bytes[0];
            iterator.Set(value);
        }
#endif

    /* Close the input file */
    fclose(file);

    /* Compute principal moments and axes */
    std::cout << "Computing moments and principal axes." << std::endl;
    ImageMomentsType moments(image);
    double ctm = moments.GetTotalMass();
    vnl_vector_fixed<double,3>
        ccg = moments.GetCenterOfGravity();
    vnl_vector_fixed<double,3>
        cpm = moments.GetPrincipalMoments();
    vnl_matrix_fixed<double,3,3>
        cpa = moments.GetPrincipalAxes();

    /* Report moments information to the user */
    std::cout << "\nTotal mass = "        << ctm << std::endl;
    std::cout << "\nCenter of gravity = " << ccg << std::endl;
    std::cout << "\nPrincipal moments = " << cpm << std::endl;
    std::cout << "\nPrincipal axes = \n";
    std::cout << cpa << "\n";

    /* Resample image in principal axes coordinates */

    /* Write resampled image to a file */
    
    return 0;
}
