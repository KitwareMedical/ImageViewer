/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ceExtractorConsoleBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <ceExtractorConsoleBase.h>


/************************************
 *
 *  Constructor
 *
 ***********************************/
ceExtractorConsoleBase 
::ceExtractorConsoleBase()
{

  m_ImageLoaded = false;

  m_Reader     = VolumeReaderType::New();
  
  m_Hx   = InputGaussianFilterType::New();
  m_Hy   = InputGaussianFilterType::New();

  m_Hx->SetDirection( 0 );
  m_Hy->SetDirection( 1 );

  m_Hxy  = GaussianFilterType::New();

  m_Hxy->SetDirection( 1 );

  m_H1x  = GaussianFilterType::New();
  m_H1y  = GaussianFilterType::New();

  m_H1x->SetDirection( 0 );
  m_H1y->SetDirection( 1 );

  m_H1x->SetOrder( GaussianFilterType::ZeroOrder );
  m_H1y->SetOrder( GaussianFilterType::ZeroOrder );

  m_H1xy = GaussianFilterType::New();
  m_H1xy->SetDirection( 1 );
  m_H1xy->SetOrder( GaussianFilterType::FirstOrder );
    
  m_H2x  = GaussianFilterType::New();
  m_H2y  = GaussianFilterType::New();

  m_H2x->SetDirection( 0 );
  m_H2y->SetDirection( 1 );

  m_H2x->SetOrder( GaussianFilterType::SecondOrder );
  m_H2y->SetOrder( GaussianFilterType::SecondOrder );

  m_Hx->SetInputImage( m_Reader->GetOutput() );
  m_Hy->SetInputImage( m_Reader->GetOutput() );

  m_Hxy->SetInputImage( m_Hx->GetOutput() );

  m_H1x->SetInputImage( m_Hy->GetOutput() );
  m_H1y->SetInputImage( m_Hx->GetOutput() );

  m_H2x->SetInputImage( m_Hy->GetOutput() );
  m_H2y->SetInputImage( m_Hx->GetOutput() );

  m_H1xy->SetInputImage( m_H1x->GetOutput() );

  
  m_Add = AddFilterType::New();

  m_Add->SetInput1( m_H2x->GetOutput() );
  m_Add->SetInput2( m_H2y->GetOutput() );

  
  m_Modulus = ModulusFilterType::New();

  m_Modulus->SetInput1( m_H1x->GetOutput() );
  m_Modulus->SetInput2( m_H1y->GetOutput() );

  
  m_Gradient = GradientFilterType::New();
  
  m_Gradient->SetInput( m_Reader->GetOutput() );

  
  m_Eigen = EigenFilterType::New();
  
  m_Eigen->SetInput1( m_H2x->GetOutput() );
  m_Eigen->SetInput2( m_H1xy->GetOutput() );
  m_Eigen->SetInput3( m_H2y->GetOutput() );
      

  m_Join = JoinFilterType::New();

  m_Join->SetInput1( m_H1x->GetOutput() );
  m_Join->SetInput2( m_H1y->GetOutput() );
  
  m_ScalarProduct = ScalarProductFilterType::New();

  m_ScalarProduct->SetInput1( m_Join->GetOutput() );
  m_ScalarProduct->SetInput2( m_Eigen->GetMaxEigenVector() );

  m_ParametricSpace = ParametricSpaceFilterType::New();

  m_ParametricSpace->SetInput( 0, m_Eigen->GetMaxEigenValue() );
  m_ParametricSpace->SetInput( 1, m_ScalarProduct->GetOutput() );
  m_ParametricSpace->SetInput( 2, m_Hxy->GetOutput() );

  m_SpatialFunctionControl = SpatialFunctionControlType::New();

  m_SpatialFunctionFilter = SpatialFunctionFilterType::New();

  m_SpatialFunctionFilter->SetInput(  
                              m_ParametricSpace->GetOutput() );

  m_SpatialFunctionControl->SetSpatialFunction( 
      m_SpatialFunctionFilter->GetSpatialFunction().GetPointer() );


  m_SpatialFunctionControl->SetAngleZ( 20.0f );
  m_SpatialFunctionControl->SetApertureAngleX( 12.0f );
  m_SpatialFunctionControl->SetApertureAngleY(  2.0f );
  m_SpatialFunctionControl->SetTopPlane( 10.0f );
  m_SpatialFunctionControl->SetBottomPlane( 110.0f );
  m_SpatialFunctionControl->SetApex( 0.0f, 0.0f, 105.0f );

  
  m_InverseParametricFilter = InverseParametricFilterType::New();

  m_InverseParametricFilter->SetInput( 
      m_SpatialFunctionFilter->GetOutput().GetPointer() );

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
ceExtractorConsoleBase 
::~ceExtractorConsoleBase()
{

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
ceExtractorConsoleBase 
::Load( const char * filename )
{

  if( !filename )
  {
    return;
  }

  m_Reader->SetFileName( filename );
  m_Reader->Update();

  InputImageType::Pointer inputImage = m_Reader->GetOutput();

  inputImage->SetRequestedRegion( 
      inputImage->GetLargestPossibleRegion() );

  m_ImageLoaded = true;

}


 
/************************************
 *
 *  Show Progress
 *
 ***********************************/
void
ceExtractorConsoleBase 
::ShowProgress( float )
{

}


 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
ceExtractorConsoleBase 
::ShowStatus( const char * )
{

}




 
/************************************
 *
 *  Set Sigma
 *
 ***********************************/
void
ceExtractorConsoleBase 
::SetSigma( RealType value )
{
  
  m_Hx->SetSigma( value );
  m_Hy->SetSigma( value );

  m_Hxy->SetSigma( value );

  m_H1x->SetSigma( value );
  m_H1y->SetSigma( value );

  m_H1xy->SetSigma( value );

  m_H2x->SetSigma( value );
  m_H2y->SetSigma( value );

  m_Gradient->SetSigma( value );

}




 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
ceExtractorConsoleBase 
::Execute( void )
{

  if( ! (m_ImageLoaded) ) 
  {
    ShowStatus("Please load an image first");
    return;
  }
  

  m_Hxy->Update();

  m_H1xy->Update();

  m_Add->Update();

  m_Modulus->Update();
  
  m_InverseParametricFilter->Update();

}



 
/************************************
 *
 *  Show Spatial Function Control
 *
 ***********************************/
void
ceExtractorConsoleBase 
::ShowSpatialFunctionControl( void )
{
  m_SpatialFunctionControl->Show();
}


 
/************************************
 *
 *  Hide Spatial Function Control
 *
 ***********************************/
void
ceExtractorConsoleBase 
::HideSpatialFunctionControl( void )
{
  m_SpatialFunctionControl->Hide();
}


