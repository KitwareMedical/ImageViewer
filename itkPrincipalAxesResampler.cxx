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
This command line program reads an image file, finds its principal
axes, and resamples the image into principal axes coordinates.  It
takes two arguments: the names of the input and output image files.
The image dimensions are fixed at 192 slices by 256 rows by 256
columns and the data format is fixed at bigendian 16-bit unsigned
integer.  (But see below for hooks to change these parameters.)
---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "itkAffineTransform.h"
#include "itkPhysicalImage.h"
#include "itkImageMomentsCalculator.h"
#include "itkSimpleImageRegionIterator.h"
#include "itkLinearInterpolateImageFunction.h"


enum {NDimensions = 3};

typedef unsigned short                      PixelType;
typedef itk::PhysicalImage<PixelType, NDimensions>  ImageType;
typedef ImageType::RegionType               ImageRegionType;
typedef ImageType::SizeType                 ImageSizeType;
typedef ImageType::AffineTransformType      AffineTransformType;

typedef itk::Index<NDimensions>                    ImageIndexType;
typedef itk::SimpleImageRegionIterator<ImageType>  ImageIteratorType;
typedef itk::ImageMomentsCalculator<PixelType, NDimensions>  ImageMomentsCalculatorType;
typedef itk::LinearInterpolateImageFunction<ImageType>  InterpolatorType;
typedef itk::Point<double, NDimensions>            PointType;


int
main(int argc, char *argv[])
{
    /* Set image and other parameters */
    long ImageWidth     = 256;
    long ImageHeight    = 256;
    long NumberOfSlices = 192;
    int bigend  = 1;             // Bigendian data in external files?
    int verbose = 1;             // Show intermediate results?

    /* Get input and output file names from the command line */
    if (argc < 2)
        {
            fprintf(stderr, "You must give input and output file names\n");
            exit(EXIT_FAILURE);
        }
    char *filename1 = argv[1];
    char *filename2 = argv[2];

    /* Open image file */
    /* FIXME: Translate into C++ */
    FILE *infile = fopen(filename1, "rb");
    if (infile == 0) 
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
    unsigned long point[3];      // Location of current pixel
    unsigned char bytes[2];      // First and second bytes of pixel
    PixelType value;             // Value of pixel
    size_t count;
#if 1       // Version using explicit loops
    for (long slice = 0; slice < NumberOfSlices; slice++) {
        point[2] = slice;
        for (long row = 0; row < ImageHeight; row++) {
            point[1] = row;
            for (long col = 0; col < ImageWidth; col++) {
                point[0] = col;
                index.SetIndex(point);
                count = fread(bytes, 1, 2, infile);
                if (count != 2) {
                    fprintf(stderr, "Error reading input file\n");
                    exit(EXIT_FAILURE); }
                if (bigend)
                    value = (bytes[0] << 8) + bytes[1];
                else
                    value = (bytes[1] << 8) + bytes[0];
                image->SetPixel(index, value);
            }
        }
    }
#else    // Broken version using iterators
    region = image->GetRequestedRegion();
    ImageIteratorType iterator(image, region);
    for (iterator.Begin(); !iterator.IsAtEnd(); ++iterator)
        {
            count = fread(bytes, 1, 2, infile);
            if (count != 2) {
                fprintf(stderr, "Error reading input file\n");
                exit(EXIT_FAILURE); }
                if (bigend)
                    value = (bytes[0] << 8) + bytes[1];
                else
                    value = (bytes[1] << 8) + bytes[0];
            iterator.Set(value);
        }
#endif

    /* Close the input file */
    fclose(infile);

    /* Compute principal moments and axes */
    std::cout << "Computing moments and transformation." << std::endl;
    ImageMomentsCalculatorType moments(image);
    double ctm = moments.GetTotalMass();
    itk::Vector<double,3>
        ccg = moments.GetCenterOfGravity();
    itk::Vector<double,3>
        cpm = moments.GetPrincipalMoments();
    itk::Matrix<double,3,3>
        cpa = moments.GetPrincipalAxes();

    /* Report moments information to the user */
    if (verbose) {
        std::cout << "\nTotal mass = "        << ctm << std::endl;
        std::cout << "\nCenter of gravity = " << ccg << std::endl;
        std::cout << "\nPrincipal moments = " << cpm << std::endl;
        std::cout << "\nPrincipal axes = \n";
        std::cout << cpa << "\n";
    }

    /* Allocate resampled image (same size as original) */
    ImageType::Pointer resamp = ImageType::New();
    resamp->SetLargestPossibleRegion(region);
    resamp->SetBufferedRegion(region);
    resamp->Allocate();

    /* Compute the transform from principal axes to original axes */
    double pi = 3.14159265359;
    AffineTransformType trans = resamp->GetIndexToPhysicalTransform();
    itk::Vector<double,3> center;
    center[0] = -ImageWidth / 2.0;
    center[1] = -ImageHeight / 2.0;
    center[2] = -NumberOfSlices / 2.0;
    trans.Translate(center);
    trans.Rotate(1, 0, pi/2.0);   // Rotate into radiological orientation
    trans.Rotate(2, 0, -pi/2.0);
    AffineTransformType pa2phys =
        moments.GetPrincipalAxesToPhysicalAxesTransform();
    if (verbose) {
        std::cout << "Principal axes to physical axes transform"
                  << std::endl << pa2phys << std::endl;
    }
    trans.Compose(pa2phys);
    trans.Compose(image->GetPhysicalToIndexTransform());
    if (verbose) {
        std::cout << "Backprojection transform:" << std::endl;
        std::cout << trans << std::endl;
    }

    /* Create and initialize the interpolator */
    InterpolatorType::Pointer interp = InterpolatorType::New();
    interp->SetInputImage(image);

    /* Resample image in principal axes coordinates */
    std::cout << "Resampling the image" << std::endl;
    PointType pos;                       // Position in resampled image
    ImageIndexType ipos;                 // Ditto, but as index
    PointType opos;                      // Position in original image
    double    dpos[NDimensions];         // Ditto, but as double
    for (long slice = 0; slice < NumberOfSlices; slice++) {
        pos[2] = ipos[2] = slice;
        for (long row = 0; row < ImageHeight; row++) {
            pos[1] = ipos[1] = row;
            for (long col = 0; col < ImageWidth; col++) {
                pos[0] = ipos[0] = col;
                opos = trans.Transform(pos);
                for (int i = 0; i < NDimensions; ++i) {
                    dpos[i] = opos[i];
                }
                value = (PixelType)(interp->Evaluate(dpos));
                resamp->SetPixel(ipos, value);
            }
        }
    }

    /* Open the output file */
    /* FIXME: Translate into C++ */
    std::cout << "Writing the output file" << std::endl;
    FILE *outfile = fopen(filename2, "wb");
    if (outfile == 0) 
        {
            fprintf(stderr, "Unable to open output file\n");
            exit(EXIT_FAILURE);
        }

    /* Write resampled image to a file */
    for (long slice = 0; slice < NumberOfSlices; slice++) {
        point[2] = slice;
        for (long row = 0; row < ImageHeight; row++) {
            point[1] = row;
            for (long col = 0; col < ImageWidth; col++) {
                point[0] = col;
                index.SetIndex(point);
                value = resamp->GetPixel(index);
                if (bigend) {
                    bytes[0] = value >> 8;
                    bytes[1] = value & 0xff;
                }
                else {
                    bytes[1] = value >> 8;
                    bytes[0] = value & 0xff;
                }
                count = fwrite(bytes, 1, 2, outfile);
                if (count != 2) {
                    fprintf(stderr, "Error reading input file\n");
                    exit(EXIT_FAILURE); }
            }
        }
    }
    fclose(outfile);
    
    return 0;
}
