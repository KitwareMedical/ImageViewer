//--------------------------------------------
//
//		  Project Gaussian Filter
//
//		  Author:  Luis Ibanez
//	
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include <liFilterConsoleBase.h>


/************************************
 *
 *  Constructor
 *
 ***********************************/
liFilterConsoleBase 
::liFilterConsoleBase()
{

  this->m_ImageLoaded = false;

  this->m_Reader     = VolumeReaderType::New();
  
  this->m_Hx   = InputGaussianFilterType::New();
  this->m_Hy   = InputGaussianFilterType::New();
  this->m_Hz   = InputGaussianFilterType::New();

  this->m_Hx->SetDirection( 0 );
  this->m_Hy->SetDirection( 1 );
  this->m_Hz->SetDirection( 2 );

  this->m_Hxy  = GaussianFilterType::New();
  this->m_Hyz  = GaussianFilterType::New();
  this->m_Hzx  = GaussianFilterType::New();

  this->m_Hxy->SetDirection( 1 );
  this->m_Hyz->SetDirection( 2 );
  this->m_Hzx->SetDirection( 0 );

  this->m_H1x  = GaussianFirstDerivativeFilterType::New();
  this->m_H1y  = GaussianFirstDerivativeFilterType::New();
  this->m_H1z  = GaussianFirstDerivativeFilterType::New();

  this->m_H1x->SetDirection( 0 );
  this->m_H1y->SetDirection( 1 );
  this->m_H1z->SetDirection( 2 );

  this->m_H2x  = GaussianSecondDerivativeFilterType::New();
  this->m_H2y  = GaussianSecondDerivativeFilterType::New();
  this->m_H2z  = GaussianSecondDerivativeFilterType::New();

  this->m_H2x->SetDirection( 0 );
  this->m_H2y->SetDirection( 1 );
  this->m_H2z->SetDirection( 2 );


  this->m_Hx->SetInputImage( this->m_Reader->GetOutput() );
  this->m_Hy->SetInputImage( this->m_Reader->GetOutput() );
  this->m_Hz->SetInputImage( this->m_Reader->GetOutput() );

  this->m_Hxy->SetInputImage( this->m_Hx->GetOutput() );
  this->m_Hyz->SetInputImage( this->m_Hy->GetOutput() );
  this->m_Hzx->SetInputImage( this->m_Hz->GetOutput() );

  this->m_H1x->SetInputImage( this->m_Hyz->GetOutput() );
  this->m_H1y->SetInputImage( this->m_Hzx->GetOutput() );
  this->m_H1z->SetInputImage( this->m_Hxy->GetOutput() );

  this->m_H2x->SetInputImage( this->m_Hyz->GetOutput() );
  this->m_H2y->SetInputImage( this->m_Hzx->GetOutput() );
  this->m_H2z->SetInputImage( this->m_Hxy->GetOutput() );

  this->m_Add = AddFilterType::New();

  this->m_Add->SetInput1( this->m_H2x->GetOutput() );
  this->m_Add->SetInput2( this->m_H2y->GetOutput() );
  this->m_Add->SetInput3( this->m_H2z->GetOutput() );

  this->m_Modulus = ModulusFilterType::New();

  this->m_Modulus->SetInput1( this->m_H1x->GetOutput() );
  this->m_Modulus->SetInput2( this->m_H1y->GetOutput() );
  this->m_Modulus->SetInput3( this->m_H1z->GetOutput() );


  this->m_Writer_Gradient_Modulus = VolumeWriterType::New();
  this->m_Writer_Gradient_X      = VolumeWriterType::New();
  this->m_Writer_Gradient_Y      = VolumeWriterType::New();
  this->m_Writer_Gradient_Z      = VolumeWriterType::New();
  this->m_Writer_Laplacian       = VolumeWriterType::New();

  this->m_Writer_Gradient_X->SetInput( this->m_H1x->GetOutput() );
  this->m_Writer_Gradient_Y->SetInput( this->m_H1y->GetOutput() );
  this->m_Writer_Gradient_Z->SetInput( this->m_H1z->GetOutput() );

  this->m_Writer_Gradient_Modulus->SetInput( this->m_Modulus->GetOutput() );

  this->m_Writer_Laplacian->SetInput( this->m_Add->GetOutput() );


}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liFilterConsoleBase 
::~liFilterConsoleBase()
{

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liFilterConsoleBase 
::Load( const char * filename )
{
  if( !filename )
  {
    return;
  }

  this->m_Reader->SetFileToLoad( filename );
  this->m_Reader->Update();

  this->m_ImageLoaded = true;

}


 
/************************************
 *
 *  Show Progress
 *
 ***********************************/
void
liFilterConsoleBase 
::ShowProgress( float )
{

}


 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
liFilterConsoleBase 
::ShowStatus( const char * )
{

}




 
/************************************
 *
 *  Set Sigma
 *
 ***********************************/
void
liFilterConsoleBase 
::SetSigma( ComputationType value )
{
  
  this->m_Hx->SetSigma( value );
  this->m_Hy->SetSigma( value );
  this->m_Hz->SetSigma( value );

  this->m_Hxy->SetSigma( value );
  this->m_Hyz->SetSigma( value );
  this->m_Hzx->SetSigma( value );

  this->m_H1x->SetSigma( value );
  this->m_H1y->SetSigma( value );
  this->m_H1z->SetSigma( value );

  this->m_H2x->SetSigma( value );
  this->m_H2y->SetSigma( value );
  this->m_H2z->SetSigma( value );

}




 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
liFilterConsoleBase 
::Execute( void )
{

  if( ! (this->m_ImageLoaded) ) 
  {
    ShowStatus("Please load an image first");
    return;
  }
  
  this->m_H1x->Update();
  this->m_H1y->Update();
  this->m_H1z->Update();

  this->m_H2x->Update();
  this->m_H2y->Update();
  this->m_H2z->Update();

  this->m_Add->Update();

  this->m_Modulus->Update();

}





 
/************************************
 *
 *  Write Gradient X
 *
 ***********************************/
void
liFilterConsoleBase 
::WriteGradientX( const char * filename )
{
  this->m_Writer_Gradient_X->SetInput( this->m_H1x->GetOutput() );
  this->m_Writer_Gradient_X->SetFileName( filename );
  this->m_Writer_Gradient_X->Write();
}


   
/************************************
 *
 *  Write Gradient Y
 *
 ***********************************/
void
liFilterConsoleBase 
::WriteGradientY( const char * filename )
{
  this->m_Writer_Gradient_Y->SetFileName( filename );
  this->m_Writer_Gradient_Y->Write();
}



/************************************
 *
 *  Write Gradient Z
 *
 ***********************************/
void
liFilterConsoleBase 
::WriteGradientZ( const char * filename )
{
  this->m_Writer_Gradient_Z->SetFileName( filename );
  this->m_Writer_Gradient_Z->Write();
}



/************************************
 *
 *  Write Gradient Modulus
 *
 ***********************************/
void
liFilterConsoleBase 
::WriteGradientModulus( const char * filename )
{
  this->m_Writer_Gradient_Modulus->SetFileName( filename );
  this->m_Writer_Gradient_Modulus->Write();
}



/************************************
 *
 *  Write Laplacian
 *
 ***********************************/
void
liFilterConsoleBase 
::WriteLaplacian( const char * filename )
{
  this->m_Writer_Laplacian->SetFileName( filename );
  this->m_Writer_Laplacian->Write();
}



