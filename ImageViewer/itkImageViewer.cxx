/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageViewer.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageViewer.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

#include <iostream>

int main(int argc, char **argv)
{
  if( argc < 2 )
    {
    std::cerr << "Usage: ImageViewer    ImageFileName" << std::endl;
    return EXIT_FAILURE;
    }

  //Allocate Images
  const unsigned int Dimension = 3;
  typedef float                             PixelType;
  typedef itk::Image<PixelType,Dimension>   ImageType;
  typedef itk::ImageFileReader<ImageType>   ReaderType;
  typedef itk::ImageViewer<ImageType>       ViewerType;
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<ImageType> FilterType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );
   
  ViewerType::Pointer inputViewer = ViewerType::New();
  inputViewer->SetInput( reader->GetOutput() );
  inputViewer->SetLabel( "Input Image" );

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetSigma( 3.0 );

  ViewerType::Pointer outputViewer = ViewerType::New();
  outputViewer->SetInput( filter->GetOutput() );
  outputViewer->SetLabel( "Output Image" );
  
  

  try
    {

    inputViewer->Update();
    outputViewer->Update();

    ViewerType::StartInteraction();
    
    }
  catch( itk::ExceptionObject & ex )
    {
    std::cerr << "Exception caught while trying to read file " << std::endl;
    std::cerr << argv[1] << std::endl;
    std::cerr << ex << std::endl;
    }
  catch( std::exception & ex )
    {
    std::cerr << "Exception caught while trying to read file " << std::endl;
    std::cerr << argv[1] << std::endl;
    std::cerr << ex.what() << std::endl;
    }
  catch( ... )
    {
    std::cerr << "Unknown Exception caught while trying to read file " << std::endl;
    std::cerr << argv[1] << std::endl;
    }
  

  return EXIT_SUCCESS;

}
