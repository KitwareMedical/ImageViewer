/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsoleBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <liImageRegistrationConsoleBase.h>
#include <FL/fl_ask.H>
#include <itkMetaImageIOFactory.h>
#include <itkRawImageIO.h>


/************************************
 *
 *  Constructor
 *
 ***********************************/
liImageRegistrationConsoleBase 
::liImageRegistrationConsoleBase()
{


  m_FixedImageReader                  = FixedImageReaderType::New();
  
  m_MovingImageReader                 = MovingImageReaderType::New();
  
  m_MovingImageWriter                 = MovingImageWriterType::New();
  
  m_ResampleInputMovingImageFilter  = ResampleFilterType::New();

  m_ResampleInputMovingImageFilter->SetInput( m_MovingImageReader->GetOutput() );

  m_ResampleMovingImageFilter = ResampleFilterType::New();

  m_ResampleMovingImageFilter->SetInput( m_MovingImageReader->GetOutput() );

  m_InputTransform = AffineTransformType::New();

  m_ResampleInputMovingImageFilter->SetTransform( m_InputTransform );

  m_ImageRegistrationMethod     = ImageRegistrationMethodType::New();

  m_ImageRegistrationMethod->SetFixedImage(  m_FixedImageReader->GetOutput() );
  m_ImageRegistrationMethod->SetMovingImage( m_ResampleInputMovingImageFilter->GetOutput() );

  m_MovingImageWriter->SetInput( m_ResampleInputMovingImageFilter->GetOutput() );


  itk::MetaImageIOFactory::RegisterOneFactory();

  itk::RawImageIOFactory< MovingImageType::PixelType, 
                          MovingImageType::ImageDimension >::RegisterOneFactory();

  m_FixedImageIsLoaded  = false;
  m_MovingImageIsLoaded = false;

  this->SelectMetric( meanSquares );
  this->SelectTransform( translationTransform );
  this->SelectOptimizer( regularStepGradientDescent );
  this->SelectInterpolator( nearestNeighborInterpolator );

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liImageRegistrationConsoleBase 
::~liImageRegistrationConsoleBase()
{

}



 
/************************************
 *
 *  Load Fixed Image
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
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
liImageRegistrationConsoleBase 
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
liImageRegistrationConsoleBase 
::SaveMovingImage( const char * filename )
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
liImageRegistrationConsoleBase 
::ShowStatus( const char * )
{

}


/************************************
 *
 *  Generate reference image
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::GenerateMovingImage( void )
{
  
  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  this->ShowStatus("Transforming the original image...");

  m_ResampleInputMovingImageFilter->SetOutputSpacing( 
        m_MovingImageReader->GetOutput()->GetSpacing() );

  m_ResampleInputMovingImageFilter->SetOutputOrigin( 
        m_MovingImageReader->GetOutput()->GetOrigin() );

  m_ResampleInputMovingImageFilter->SetSize( 
      m_MovingImageReader->GetOutput()->GetLargestPossibleRegion().GetSize() );

  m_ResampleInputMovingImageFilter->Update();

  this->ShowStatus("FixedImage Image Transformation done");

}


 


/************************************
 *
 *  Stop Registration
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
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
liImageRegistrationConsoleBase 
::Execute( void )
{
  m_ImageRegistrationMethod->StartRegistration();
}




 
/************************************
 *
 *  Generate Mapped MovingImage image
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::GenerateMappedMovingImage( void )
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
 *  Update the parameters of the 
 *  Transform
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::UpdateTransformParameters( void )
{

}


 
/************************************
 *
 *  Select the metric to be used to 
 *  compare the images during the 
 *  registration process 
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
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
 *  Select the optimizer to be used 
 *  to explore the transform parameter
 *  space and optimize the metric
 *
 ****************************************/
void
liImageRegistrationConsoleBase 
::SelectOptimizer( OptimizerIdentifier optimizerId )
{

  m_SelectedOptimizer = optimizerId;

  switch( m_SelectedOptimizer )
  {
  case gradientDescent:
    {
    m_ImageRegistrationMethod->SetOptimizer( 
                 itk::GradientDescentOptimizer::New() );
    break;
    }
  case regularStepGradientDescent:
    {
    m_ImageRegistrationMethod->SetOptimizer( 
             itk::RegularStepGradientDescentOptimizer::New() );
    break;
    }

  case conjugateGradient:
    {
    m_ImageRegistrationMethod->SetOptimizer( 
                 itk::ConjugateGradientOptimizer::New() );
    break;
    }
  default:
    fl_alert("Unkown type of optimizer was selected");
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
liImageRegistrationConsoleBase 
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
liImageRegistrationConsoleBase 
::SelectTransform( TransformIdentifier transformId )
{

  m_SelectedTransform = transformId;

  switch( m_SelectedTransform )
  {
  case translationTransform:
    {
    m_ImageRegistrationMethod->SetTransform( 
        itk::TranslationTransform<double,ImageDimension>::New() );
    break;
    }
  case scaleTransform:
    {
    m_ImageRegistrationMethod->SetTransform( 
        itk::ScaleTransform<double,ImageDimension>::New() );
    break;
    }
  case affineTransform:
    {
    m_ImageRegistrationMethod->SetTransform( 
        itk::AffineTransform<double,ImageDimension>::New() );
    break;
    }
  case rigidTransform:
    {
    m_ImageRegistrationMethod->SetTransform( 
        itk::Rigid3DTransform<double>::New() );
    break;
    }
  default:
    fl_alert("Unkown type of optimizer was selected");
    return;
  }

}


