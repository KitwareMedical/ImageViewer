
/**

    Illustration of the use of the class
        
    itk::DanielssonDistanceMapImageFilter

*/


#include "DistanceMapFilterBase.h"
#include "FL/fl_file_chooser.H"
#include "FL/fl_ask.H"

#include <iostream>  
#include "itkImageRegionIteratorWithIndex.h"
#include <itkMetaImageIOFactory.h>



DistanceMapFilterBase
::DistanceMapFilterBase()
{

  m_ImageLoaded = false;

  m_Reader = VolumeReaderType::New();

  m_DistanceFilter = DistanceFilterType::New();

  m_DistanceFilter->SetInput( m_Reader->GetOutput() );

  m_DistanceImage = m_DistanceFilter->GetOutput();

  m_Display.GetGlWindow()->SetBackground( 0.7, 0.8, 0.9 );
  m_Display.GetGlWindow()->SetZoom( 1.0 );
  m_Display.SetLabel("Distance Filter");


  // Slice Drawer for the input image
  m_ImageSliceDrawer = ImageSliceDrawerType::New();

  m_ImageSliceDrawer->SetInput( m_Reader->GetOutput() );

  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             m_ImageSliceDrawer->GetDrawCommand().GetPointer() );

  m_ImageSliceDrawer->AddObserver( 
                         fltk::VolumeReslicedEvent(),
                         m_Display.GetRedrawCommand() );
 
  m_ImageSliceDrawer->SetLabel("Input Image");


  // Slice Drawer for the distance map image
  m_DistanceImageSliceDrawer = ImageSliceDrawerType::New();

  m_DistanceImageSliceDrawer->SetInput( m_DistanceFilter->GetOutput() );

  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             m_DistanceImageSliceDrawer->GetDrawCommand().GetPointer() );

  m_DistanceImageSliceDrawer->AddObserver( 
                         fltk::VolumeReslicedEvent(),
                         m_Display.GetRedrawCommand() );
 
  m_DistanceImageSliceDrawer->SetLabel("Input Image");

  // Register a producer of MetaImage readers
  itk::MetaImageIOFactory::RegisterOneFactory();

}


DistanceMapFilterBase
::~DistanceMapFilterBase()
{
  this->HideDisplay();
}



/**
 *    Show the Display
 */ 
void 
DistanceMapFilterBase
::ShowDisplay(void)
{
  m_Display.Show();
}





/**
 *    Hide the Display
 */ 
void 
DistanceMapFilterBase
::HideDisplay(void)
{
  m_Display.Hide();
  m_ImageSliceDrawer->Hide();
  m_DistanceImageSliceDrawer->Hide();
}






/**
 *    Save
 */ 
void 
DistanceMapFilterBase
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
DistanceMapFilterBase
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
DistanceMapFilterBase
::Save(const char * filename) const
{
  
}



 

/**
 *    Load
 */ 
void 
DistanceMapFilterBase
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
 *    Compute Distance
 */ 
void 
DistanceMapFilterBase
::ComputeDistance(void)
{
   
  if( !m_ImageLoaded )
    {
    fl_alert("Please load an image first");
    return;
    }

  m_DistanceFilter->Update();

  m_DistanceImageSliceDrawer->SetInput( 
               m_DistanceFilter->GetOutput() );

  m_Display.Redraw();


}

  
 
 


