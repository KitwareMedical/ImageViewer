
/**

    Illustration of the use of the class
        
    itk::GradientRecursiveGaussianFilter

*/


#include "GradientRecursiveGaussianFilterBase.h"
#include "FL/fl_file_chooser.H"
#include "FL/fl_ask.H"

#include <iostream>  
#include "itkImageRegionIteratorWithIndex.h"


GradientRecursiveGaussianFilterBase
::GradientRecursiveGaussianFilterBase()
{

  m_ImageLoaded = false;

  m_Reader = VolumeReaderType::New();

  m_GradientFilter = GradientFilterType::New();

  m_GradientFilter->SetInput( m_Reader->GetOutput() );

  m_GradientFilter->SetSigma( 5.0 );
  m_ImageGradient = m_GradientFilter->GetOutput();

  m_Display.GetGlWindow()->SetBackground( 0.7, 0.8, 0.9 );
  m_Display.GetGlWindow()->SetZoom( 1.0 );
  m_Display.SetLabel("Gradient Gaussian Filter");


  m_ImageSliceDrawer = ImageSliceDrawerType::New();

  m_ImageSliceDrawer->SetInput( m_Reader->GetOutput() );

  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             m_ImageSliceDrawer->GetDrawCommand().GetPointer() );

  m_ImageSliceDrawer->AddObserver( 
                         fltk::VolumeReslicedEvent(),
                         m_Display.GetRedrawCommand() );
 
  m_ImageSliceDrawer->SetLabel("Input Image");


  m_ImageGradientDrawer = ImageGradientDrawerType::New();

  m_ImageGradientDrawer->SetVectorImage( m_GradientFilter->GetOutput() );
 
  
  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             m_ImageGradientDrawer->GetDrawCommand().GetPointer() );

  m_ImageGradientDrawer->AddObserver( 
                         itk::AnyEvent(),
                         m_Display.GetRedrawCommand() );
 
 
  m_ImageGradientDrawer->SetDrawingMode( fltk::Shape3D::lines );
  m_ImageGradientDrawer->SetCompileMode( fltk::Shape3D::compileExecute );
  m_ImageGradientDrawer->SetVectorLengthScale( 0.01 );

  fltk::Shape3D::ColorType vectorColor;
  vectorColor.SetRed(   1.0 );
  vectorColor.SetGreen( 0.2 );
  vectorColor.SetBlue(  0.2 );

  m_ImageGradientDrawer->SetColor( vectorColor );

}


GradientRecursiveGaussianFilterBase
::~GradientRecursiveGaussianFilterBase()
{
  this->HideDisplay();
}



/**
 *    Show the Display
 */ 
void 
GradientRecursiveGaussianFilterBase
::ShowDisplay(void)
{
  m_Display.Show();
}





/**
 *    Hide the Display
 */ 
void 
GradientRecursiveGaussianFilterBase
::HideDisplay(void)
{
  m_Display.Hide();
}






/**
 *    Save
 */ 
void 
GradientRecursiveGaussianFilterBase
::Save(void) const
{
  const char * filename = fl_file_chooser("","","");

  if( !filename )
    {
    return;
    }

  this->Save( filename );

}






/**
 *    Load
 */ 
void 
GradientRecursiveGaussianFilterBase
::Load(void) 
{
  const char * filename = fl_file_chooser("","","");

  if( !filename )
    {
    return;
    }

  this->Load( filename );

}



/**
 *    Save
 */ 
void 
GradientRecursiveGaussianFilterBase
::Save(const char * filename) const
{
  
}



 

/**
 *    Load
 */ 
void 
GradientRecursiveGaussianFilterBase
::Load(const char * filename) 
{
   
  if( !filename )
  {
    return;
  }

  m_Reader->SetFileName( filename );
  m_Reader->Update();

  m_ImageSliceDrawer->SetInput( m_Reader->GetOutput() );
 
  m_ImageLoaded = true;

 
}


 

/**
 *    Compute Gradient
 */ 
void 
GradientRecursiveGaussianFilterBase
::ComputeGradient(void)
{
   
  if( !m_ImageLoaded )
    {
    fl_alert("Please load an image first");
    return;
    }

  m_GradientFilter->Update();

  m_Display.Redraw();

  m_ImageGradientDrawer->ScheduleToUpdateDisplayList();


}

  
 
 


