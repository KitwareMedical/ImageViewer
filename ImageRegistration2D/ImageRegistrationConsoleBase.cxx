/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistrationConsoleBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <ImageRegistrationConsoleBase.h>
#include <FL/fl_ask.H>
#include <itkMetaImageIOFactory.h>
#include <itkRawImageIO.h>


/************************************
 *
 *  Constructor
 *
 ***********************************/
ImageRegistrationConsoleBase 
::ImageRegistrationConsoleBase()
{


  m_FixedImageReader                  = FixedImageReaderType::New();
  
  m_MovingImageReader                 = MovingImageReaderType::New();
  
  m_MovingImageWriter                 = MovingImageWriterType::New();
  
  m_ResampleMovingImageFilter = ResampleFilterType::New();

  m_ResampleMovingImageFilter->SetInput( m_MovingImageReader->GetOutput() );

  m_ImageRegistrationMethod     = ImageRegistrationMethodType::New();

  m_ImageRegistrationMethod->SetFixedImage(  m_FixedImageReader->GetOutput() );
  m_ImageRegistrationMethod->SetMovingImage( m_MovingImageReader->GetOutput() );

  m_MovingImageWriter->SetInput( m_ResampleMovingImageFilter->GetOutput() );


  itk::MetaImageIOFactory::RegisterOneFactory();

  itk::RawImageIOFactory< MovingImageType::PixelType, 
                          MovingImageType::ImageDimension >::RegisterOneFactory();

  m_FixedImageIsLoaded  = false;
  m_MovingImageIsLoaded = false;

  m_ImageRegistrationMethod->SetOptimizer( 
             itk::RegularStepGradientDescentOptimizer::New() ); 

  this->SelectMetric( meanSquares );
  this->SelectTransform( translationTransform );
  this->SelectInterpolator( nearestNeighborInterpolator );


}




/************************************
 *
 *  Destructor
 *
 ***********************************/
ImageRegistrationConsoleBase 
::~ImageRegistrationConsoleBase()
{

}



 
/************************************
 *
 *  Load Fixed Image
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::LoadFixedImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_FixedImageReader->SetFileName( filename );
  m_FixedImageReader->Update();
  
  m_FixedImageIsLoaded = true;

}


 
/************************************
 *
 *  Load Moving Image
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::LoadMovingImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_MovingImageReader->SetFileName( filename );
  m_MovingImageReader->Update();

  m_MovingImageIsLoaded = true;

}

 
/************************************
 *
 *  Save Moving Image
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::SaveRegisteredMovingImage( const char * filename )
{
  if( !filename )
  {
    return;
  }


/*
  std::ofstream ofs( filename );
  MovingImageType * movingImage = 
      m_ResampleInputMovingImageFilter->GetOutput().GetPointer();
  ofs.write( (char*)movingImage->GetBufferPointer(), 
             movingImage->GetBufferedRegion().GetNumberOfPixels() 
             * sizeof( MovingImageType::PixelType ) );
            
  ofs.close();
  std::cout << "Image was writen to : " << filename << std::endl;
*/

  m_MovingImageWriter->SetFileName( filename );
  m_MovingImageWriter->Write();

}



 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::ShowStatus( const char * )
{

}

 


/************************************
 *
 *  Stop Registration
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::Stop( void )
{
  // TODO: add a Stop() method to Optimizers 
  //m_ImageRegistrationMethod->GetOptimizer()->Stop();

}




/************************************
 *
 *  Execute
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::Execute( void )
{
  m_ImageRegistrationMethod->StartRegistration();
}




 
/************************************
 *
 *  Generate Registered MovingImage image
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::GenerateRegisteredMovingImage( void )
{

  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  this->ShowStatus("Transforming the reference image...");

  m_ResampleMovingImageFilter->Update();

  this->ShowStatus("MovingImage Image Transformation done");

}


 
/************************************
 *
 *  Select the metric to be used to 
 *  compare the images during the 
 *  registration process 
 *
 ***********************************/
void
ImageRegistrationConsoleBase 
::SelectMetric( MetricIdentifier metricId )
{

  m_SelectedMetric = metricId;

  switch( m_SelectedMetric )
  {
  case mutualInformation:
    {
    m_ImageRegistrationMethod->SetMetric( 
                 MutualInformationMetricType::New() );
    break;
    }
  case normalizedCorrelation:
    {
    m_ImageRegistrationMethod->SetMetric( 
                 NormalizedCorrelationImageMetricType::New() );
    break;
    }

  case patternIntensity:
    {
    m_ImageRegistrationMethod->SetMetric( 
                 PatternIntensityImageMetricType::New() );
    break;
    }

  case meanSquares:
    {
    m_ImageRegistrationMethod->SetMetric( 
                 MeanSquaresMetricType::New() );
    break;
    }
  default:
    fl_alert("Unkown type of metric was selected");
    return;
  }

}





/*****************************************
 *
 *  Select the interpolator to be used 
 *  to evaluate images at non-grid positions
 *
 ****************************************/
void
ImageRegistrationConsoleBase 
::SelectInterpolator( InterpolatorIdentifier interpolatorId )
{

  m_SelectedInterpolator = interpolatorId;

  switch( m_SelectedInterpolator )
  {
  case linearInterpolator:
    {
    m_ImageRegistrationMethod->SetInterpolator( 
        itk::LinearInterpolateImageFunction<MovingImageType, 
                                            double >::New() );
    break;
    }
  case nearestNeighborInterpolator:
    {
    m_ImageRegistrationMethod->SetInterpolator( 
        itk::NearestNeighborInterpolateImageFunction<MovingImageType, 
                                            double >::New() );
    break;
    }

  default:
    fl_alert("Unkown type of optimizer was selected");
    return;
  }

}


 
/*****************************************
 *
 *  Select the Transform  to be used 
 *  to map the moving image into the 
 *  fixed image
 *
 ****************************************/
void
ImageRegistrationConsoleBase 
::SelectTransform( TransformIdentifier transformId )
{

  m_SelectedTransform = transformId;

  switch( m_SelectedTransform )
  {
  case translationTransform:
    {
    typedef itk::TranslationTransform<double,ImageDimension> TransformType;
    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();
    m_ImageRegistrationMethod->SetTransform( transform );
    m_ImageRegistrationMethod->SetInitialTransformParameters( transform->GetParameters() );
    break;
    }
  case scaleTransform:
    {
    typedef itk::ScaleTransform<double,ImageDimension> TransformType;
    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();
    m_ImageRegistrationMethod->SetTransform( transform );
    m_ImageRegistrationMethod->SetInitialTransformParameters( transform->GetParameters() );
    break;
    }
  case affineTransform:
    {
    typedef itk::AffineTransform<double,ImageDimension> TransformType;
    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();
    m_ImageRegistrationMethod->SetTransform( transform );
    m_ImageRegistrationMethod->SetInitialTransformParameters( transform->GetParameters() );
    break;
    }
  case rigidTransform:
    {
    typedef itk::Rigid2DTransform<double> TransformType;
    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();
    m_ImageRegistrationMethod->SetTransform( transform );
    m_ImageRegistrationMethod->SetInitialTransformParameters( transform->GetParameters() );
    break;
    }
  default:
    fl_alert("Unkown type of optimizer was selected");
    return;
  }

}


