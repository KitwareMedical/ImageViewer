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
  
  m_ResamplingInputMovingImageFilter  = ResamplingFilterType::New();

  m_ResamplingInputMovingImageFilter->SetInput( m_MovingImageReader->GetOutput() );

  m_ResamplingMovingImageFilter = ResamplingFilterType::New();

  m_ResamplingMovingImageFilter->SetInput( m_MovingImageReader->GetOutput() );



  m_ImageRegistrationMethod     = ImageRegistrationMethodType::New();

  m_ImageRegistrationMethod->SetFixedImage(  m_FixedImageReader->GetOutput() );
  m_ImageRegistrationMethod->SetMovingImage( m_ResamplingInputMovingImageFilter->GetOutput() );


  m_SelectedMetric = meanSquares;

  // Register a producer of MetaImage readers
  itk::MetaImageIOFactory::RegisterOneFactory();

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

}


 
/************************************
 *
 *  Show Progress
 *
 ***********************************/
void
liImageRegistrationConsoleBase 
::ShowProgress( float )
{

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

  this->ShowStatus("Transforming the original image...");

  m_ResamplingMovingImageFilter->Update();

  this->ShowProgress( 1.0 );
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

  this->ShowStatus("Transforming the reference image...");

  m_ResamplingMovingImageFilter->Update();

  this->ShowProgress( 1.0 );
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



