/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ITKRegistrationLib.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// The first #include must be itkImage.h
#include "itkImage.h"
#include "itkArray.h"
#include "itkPoint.h"

#include "itkImageRegionIterator.h"
#include "itkStatisticsImageFilter.h"

// Modules of the registration pipeline
#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"
#include "itkLinearInterpolateImageFunction.h"

// This class wraps the multi-res MI registration method of itk
#include "MIMRegistrator.h"

#include <iostream>

/**
 * Transform the moving image into the space of the fixed image via
 * the given transform. The result is stored in the outIm image.
 **/
template <class dataT>
bool transformImage(typename itk::Image<dataT,3>::Pointer fixedIm, 
                    typename itk::Image<dataT,3>::Pointer movingIm, 
                    itk::SmartPointer< itk::AffineTransform<double, 3> > 
                         transform,
                    typename itk::Image<dataT,3>::Pointer outIm)
  {
  typedef itk::Image<dataT, 3> ImageType;
  typedef itk::AffineTransform<double, 3> TransformType;
  typedef itk::LinearInterpolateImageFunction<ImageType,double>  
          InterpolatorType;
  typedef itk::ResampleImageFilter<ImageType,ImageType> ResamplerType;

  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();

  typename ResamplerType::Pointer resampler = ResamplerType::New();
  resampler->SetInput( movingIm );

  resampler->SetTransform( transform.GetPointer() );
  resampler->SetInterpolator( interpolator.GetPointer() );
  resampler->SetSize( fixedIm->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin( fixedIm->GetOrigin() );
  resampler->SetOutputSpacing( fixedIm->GetSpacing() );
  resampler->SetDefaultPixelValue( 0 );

  resampler->GraftOutput(outIm);

  // resample the moving image
  resampler->Update();

  return true;
  }

/**
 * Function to register two images. The resultImage contains the
 *   the outcome of a rigid transformation of the moving image into
 *   alignement with the fixed image.
 **/
bool ITKRegistrationLib(int * fixedDimSize, float * fixedOrigin, 
                        float * fixedSpacing, 
                        unsigned short * fixedImageData, 
                        int * movingDimSize, float * movingOrigin, 
                        float * movingSpacing, 
                        unsigned short * movingImageData,
                        unsigned short * resultImageData)
  {
  typedef unsigned short PixelType;
  typedef itk::Image<PixelType,3>      ImageType;
  typedef itk::MIMRegistrator<ImageType, ImageType> RegistratorType;
  typedef itk::StatisticsImageFilter<ImageType> StatisticsImageFilterType;
  enum { ImageDimension = ImageType::ImageDimension };

  // Setup the itkImage for the fixedImageData, i.e., the fixedImage
  ImageType::SizeType fixedSize = {{fixedDimSize[0],
                                    fixedDimSize[1],
                                    fixedDimSize[2]}};
  int fixedQuantity = fixedDimSize[0]*fixedDimSize[1]*fixedDimSize[2];
  ImageType::Pointer fixedImage = ImageType::New();
  fixedImage->SetRegions( fixedSize );
  fixedImage->SetOrigin( fixedOrigin );
  fixedImage->SetSpacing( fixedSpacing );
  fixedImage->GetPixelContainer( )->SetImportPointer( fixedImageData,
                                                      fixedQuantity );
  
  // Setup the itkImage for the movingImageData, i.e., the movingImage
  ImageType::SizeType movingSize = {{movingDimSize[0],
                                     movingDimSize[1],
                                     movingDimSize[2]}};
  int movingQuantity = fixedDimSize[0]*movingDimSize[1]*movingDimSize[2];
  ImageType::Pointer movingImage = ImageType::New();
  movingImage->SetRegions( movingSize );
  movingImage->SetOrigin( movingOrigin );
  movingImage->SetSpacing( movingSpacing );
  movingImage->GetPixelContainer( )->SetImportPointer( movingImageData, 
                                                       movingQuantity );

  // Setup the itkImage for the resultsImageData, i.e., the resultsImage
  ImageType::Pointer resultImage = ImageType::New();
  resultImage->SetRegions( fixedSize );
  resultImage->SetOrigin( fixedOrigin );
  resultImage->SetSpacing( fixedSpacing );
  resultImage->GetPixelContainer( )->SetImportPointer( resultImageData, 
                                                       fixedQuantity );

  // Create the registration method
  RegistratorType::Pointer miReg = RegistratorType::New();

  miReg->SetFixedImage ( fixedImage );
  miReg->SetMovingImage ( movingImage ) ;

  // The mutual information metric uses a Parzen approach to density
  //   estimation - it requires a bandwidth estimate (i.e., a kernel
  //   size). We use the standard deviation of the image intensities
  //   as a basis for specifying the bandwidth.
  StatisticsImageFilterType::Pointer fixedImageStatisticsFilter = 
        StatisticsImageFilterType::New();
  fixedImageStatisticsFilter->SetInput( fixedImage );
  fixedImageStatisticsFilter->Update(); // gotta call this since pipelining
                                        //  of none image data doesn't
                                        //  work...yet...
  miReg->SetFixedImageStandardDeviation(
        fixedImageStatisticsFilter->GetSigma() * 0.4 );

  StatisticsImageFilterType::Pointer movingImageStatisticsFilter = 
        StatisticsImageFilterType::New();
  movingImageStatisticsFilter->SetInput( movingImage );
  movingImageStatisticsFilter->Update();
  miReg->SetMovingImageStandardDeviation( 
        movingImageStatisticsFilter->GetSigma() * 0.4 );

  // set multiresolution related parameters
  const int nLevels = 4;
  miReg->SetNumberOfLevels( nLevels );

  // permute the shrink factors  
  unsigned int permutedFactors[ ImageDimension ];
  permutedFactors[ 0 ] = 2; 
  permutedFactors[ 1 ] = 2;
  permutedFactors[ 2 ] = 1;
  miReg->SetFixedImageShrinkFactors( permutedFactors );
  miReg->SetMovingImageShrinkFactors( permutedFactors );

  itk::Array< unsigned int > nIterations( 5 );
  nIterations[ 0 ] = 1000; // 2500
  nIterations[ 1 ] = 1000;
  nIterations[ 2 ] = 1000;
  nIterations[ 3 ] = 1000;
  nIterations[ 4 ] = 1000;
  miReg->SetNumberOfIterations( nIterations );

  // Hint: if the program crashes...make these smaller :)
  itk::Array< double > learningRates( 5 );
  learningRates[ 0 ] = 0.001;
  learningRates[ 1 ] = 0.0001;
  learningRates[ 2 ] = 0.0001;
  learningRates[ 3 ] = 0.00001;
  learningRates[ 4 ] = 0.000001;
  miReg->SetLearningRates( learningRates );

  // Rigid registration consists of quaternion rotation estimate
  //   and an a translation vector. These must be scaled to have
  //   "commensurate"/unbiased units during optimization.  Here we
  //   specify a scale for the translation vector based on the size
  //   of the image.
  double scale = 1.0 / vnl_math_sqr( fixedDimSize[ 0 ] * 0.4 );
  miReg->SetTranslationScale( scale );

  miReg->Execute();

  std::cout << "Final tranformation matrix: " << std::endl
            << miReg->GetAffineTransform()->GetMatrix() << std::endl;

  itk::Point<double, 3> pnt;
  itk::Point<double, 3> pntTransformed;

  pnt.Fill( 0.0 );
  pntTransformed = miReg->GetAffineTransform()->TransformPoint(pnt);

  std::cout << "Final offset: " << std::endl
            << pntTransformed << std::endl;

  transformImage<unsigned short>(fixedImage, movingImage, 
                                 miReg->GetAffineTransform(), 
                                 resultImage);

  return true;
  }

