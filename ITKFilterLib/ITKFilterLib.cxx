/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ITKFilterLib.cxx
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
#include <itkImage.h>

#include <iostream>

#include "ITKFilterLib.h"

// Nasty local globals inside .cxx file to hide itk from the real world.  
//   Can't put this variable in the class definition otherwise we'd have to 
//   include itkImage.h in every program using this class.
// The typedef h_itkFilterLibeDataType is in itkFilterLib.h to hide 
//   templates
typedef itk::Image<g_ItkFilterLibDataType,3>   g_ImageType;
g_ImageType::Pointer g_InputImage;
g_ImageType::Pointer g_OutputImage;


//
//
// The first filter in the .lib
//
//

/** The next lines are used to include the definition of the filter
 *   we want to install in the .lib, define a typedef for it, and create
 *   a "smart" pointer to hold it.
 * CHANGE HERE: The filter to be instantiated.
 **/
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"     
typedef itk::CurvatureAnisotropicDiffusionImageFilter<g_ImageType, 
    g_ImageType> g_AnisoFilterType;    
g_AnisoFilterType::Pointer g_anisoFilter;


//
//
// The second filter in the .lib
//
//

/** Repeat the sequence of includes, typedefs, and filter pointer 
 *   declarations for each filter type to be supported.
 * CHANGE HERE: The filter to be instantiated.
 **/
#include "itkRecursiveGaussianImageFilter.h"     
typedef itk::RecursiveGaussianImageFilter<g_ImageType, g_ImageType> 
    g_GaussianFilterType;    
g_GaussianFilterType::Pointer g_gaussianFilter;



/** Constructor: Input image and output image must be of the same size
 *     This could easily be changed, but holds true for most filters.
 **/
itkFilterLib::
itkFilterLib(int * imageDimSize, float * imageOrigin, float * imageSpacing)
  {
  g_ImageType::SizeType imageSize = {{imageDimSize[0],
                                      imageDimSize[1],
                                      imageDimSize[2]}};

  // Keep track of the number of voxels in the image 
  m_DataQuantity = imageDimSize[0]*imageDimSize[1]*imageDimSize[2];

  g_InputImage = g_ImageType::New();
  g_InputImage->SetRegions( imageSize );
  g_InputImage->SetOrigin( imageOrigin );
  g_InputImage->SetSpacing( imageSpacing );
  g_InputImage->SetReferenceCount(2);

  g_OutputImage = g_ImageType::New();
  g_OutputImage->SetRegions( imageSize );
  g_OutputImage->SetOrigin( imageOrigin );
  g_OutputImage->SetSpacing( imageSpacing );
  g_OutputImage->SetReferenceCount(2);

  // CHANGE HERE: The filters are allocated
  g_anisoFilter = g_AnisoFilterType::New();
  g_anisoFilter->SetReferenceCount(2);  // needed so that the filter 
                                        //   persists outside of this 
                                        //   function..."smart" pointers
                                        //   ...sigh...

  g_gaussianFilter = g_GaussianFilterType::New();
  g_gaussianFilter->SetReferenceCount(2);  // needed so that the filter
                                           //   persists outside of this 
                                           //   function..."smart" pointers
                                           //   ...sigh...
  }

void itkFilterLib::
SetInputData(g_ItkFilterLibDataType * imageData)
  {
  g_InputImage->GetPixelContainer()->SetImportPointer( imageData, 
                                                       m_DataQuantity );
  }

void itkFilterLib::
SetOutputData(g_ItkFilterLibDataType * imageData)
  {
  g_OutputImage->GetPixelContainer()->SetImportPointer( imageData, 
                                                        m_DataQuantity );
  }


//
//
// The first filter in the .lib
//
//

/**
 * CHANGE HERE: Specify member functions to set each filter's parameters
 **/
void itkFilterLib::
SetIterationsOfAnisotropicDiffusionFilter(int iterations)
  {
  g_anisoFilter->SetNumberOfIterations(iterations);
  }

void itkFilterLib::
SetTimeStepOfAnisotropicDiffusionFilter(float timeStep)
  {
  g_anisoFilter->SetTimeStep(timeStep);
  }

void itkFilterLib::
SetConductanceOfAnisotropicDiffusionFilter(float conductance)
  {
  g_anisoFilter->SetConductanceParameter(conductance);
  }

void itkFilterLib::
SetFixedAverageGradientMagnitudeOfAnisotropicDiffusionFilter(
    float fixedAverageGradientMagnitude)
  {
  g_anisoFilter->SetFixedAverageGradientMagnitude(
      fixedAverageGradientMagnitude);
  }

/**
 * ExecuteFilter: Called to calculate the results and store them in the 
 *   memory pointed to by the SetOutputData(pntr) call.  Note: Doesn't 
 *   check for valid input and output memory/images....UserIQ Error if 
 *   you don't call SetInputData and SetOutputData before calling this 
 *   function.
 * CHANGE HERE: Add in new instances of this member function for each 
 *   new filter added.
 */
void itkFilterLib::
ExecuteAnisotropicDiffusionFilter(void)
  {
  g_anisoFilter->SetInput ( g_InputImage );
  g_anisoFilter->GraftOutput ( g_OutputImage );
  g_anisoFilter->Update();
  }


//
//
// The second filter in the .lib
//
//

/**
 * CHANGE HERE: Specify member functions to set each filter's parameters
 **/
void itkFilterLib::
SetSigmaOfGaussianBlurFilter(float sigma)
  {
  g_gaussianFilter->SetSigma(sigma);
  }

/**
 * ExecuteFilter: Called to calculate the results and store them in the 
 *   memory pointed to by the SetOutputData(pntr) call.  Note: Doesn't 
 *   check for valid input and output memory/images....UserIQ Error if 
 *   you don't call SetInputData and SetOutputData before calling this 
 *   function.
 * CHANGE HERE: Add in new instances of this member function for each 
 *   new filter added.
 */
void itkFilterLib::
ExecuteGaussianBlurFilter(void)
  {
  g_gaussianFilter->SetInput ( g_InputImage );
  g_gaussianFilter->GraftOutput ( g_OutputImage );
  g_gaussianFilter->Update();
  }
