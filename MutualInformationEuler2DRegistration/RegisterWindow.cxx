#include "time.h"

#include <RegisterWindow.h>
#include <FL/fl_file_chooser.H>

RegisterWindow::RegisterWindow()
{
  m_FixedImageViewer = ImageViewerType::New() ;
  m_MovingImageViewer = ImageViewerType::New() ;
  m_RegisteredImageViewer = ImageViewerType::New() ;
  m_MixedChannelViewer = MixedChannelViewerType::New() ;

  m_FixedImageViewer->SetLabel( "Fixed Image" ) ;
  m_MovingImageViewer->SetLabel( "Moving Image" ) ;
  m_RegisteredImageViewer->SetLabel( "Registered Image" ) ;
  m_MixedChannelViewer->SetLabel( "Mixed Channel View" ) ;

  m_DebugViewer = ImageViewerType::New() ;
  m_DebugViewer->SetLabel( "Debug" ) ;

  m_ChannelSources[0] = fixed ;
  m_ChannelSources[1] = registered ;
  m_ChannelSources[2] = none ;

  m_FixedImageViewer->imageViewer->
    SetSelectionCallBack((void*) this, 
                         RegisterWindow::SelectionCallBackWrapper) ;
  buStartRegistration->deactivate() ;
  buSaveRegisteredImage->deactivate() ;
  buShowDisplay->deactivate() ;
  lsDisplay->deactivate() ;
  menuFixedImageDisplay->deactivate() ;
  menuMovingImageDisplay->deactivate() ;
  menuRegisteredImageDisplay->deactivate() ;
  menuMixedChannel->deactivate() ;

  m_IterationObserver = IterationObserverType::New();
  m_IterationObserver->SetOptimizer( m_Registrator->GetOptimizer() );
  m_IterationObserver->SetBrowser( iterationsBrowser );

  this->ShowStatus("Let's start by loading an image...");
}

RegisterWindow::~RegisterWindow()
{
}

void RegisterWindow::LoadMovingImage(void)
{
  const char * filename = 
    fl_file_chooser("Moving Image filename","*.*","");
  if( !filename )
    {
      return;
    }

  m_MovingImageFileName = filename ;

  this->ShowStatus("Loading moving image file...");
  
  try 
    {
      RegisterApplication::LoadMovingImage();
    }
  catch( ... ) 
    {
      this->ShowStatus("Problems reading file format");
      return;
    }

  if (m_FixedImage != 0 && m_MovingImage != 0)
    {
      buStartRegistration->activate() ;
    }

  lsDisplay->activate() ;
  buShowDisplay->activate() ;
  menuMovingImageDisplay->activate() ;

  this->ShowStatus("Moving Image Loaded");
}

void RegisterWindow::LoadFixedImage(void)
{
  const char * filename = 
    fl_file_chooser("Fixed Image filename","*.*","");

  if( !filename )
    {
      return;
    }

  m_FixedImageFileName = filename ;

  this->ShowStatus("Loading fixed image file...");
  
  try 
    {
      RegisterApplication::LoadFixedImage();
    }
  catch( ... ) 
    {
      this->ShowStatus("Problems reading file format");
      return;
    }

  m_TempBox.X1 = m_FixedImage->GetLargestPossibleRegion().GetIndex()[0] ;
  m_TempBox.Y1 = m_FixedImage->GetLargestPossibleRegion().GetIndex()[1] ;
  m_TempBox.X2 = m_TempBox.X1 + 
    m_FixedImage->GetLargestPossibleRegion().GetSize()[0] ;
  m_TempBox.Y2 = m_TempBox.Y1 + 
    m_FixedImage->GetLargestPossibleRegion().GetSize()[1] ;

  outSelectedRegionBeginX->value(m_TempBox.X1) ;
  outSelectedRegionBeginY->value(m_TempBox.Y1) ;
  outSelectedRegionEndX->value(m_TempBox.X2) ;
  outSelectedRegionEndY->value(m_TempBox.Y2) ;

  m_FixedImageViewer->imageViewer->SetSelectionBox(&m_TempBox) ;

  lsDisplay->activate() ;
  buShowDisplay->activate() ;
  menuFixedImageDisplay->activate() ;
  if (m_FixedImage != 0 && m_MovingImage != 0)
    {
      buStartRegistration->activate() ;
    }
  
  this->ShowStatus("Fixed Image Loaded");
}

void RegisterWindow::Show()
{
  winMain->show() ;
}

void RegisterWindow::Hide()
{
  winMain->hide();
  m_FixedImageViewer->Hide();
  m_MovingImageViewer->Hide();
  m_RegisteredImageViewer->Hide();
  m_MixedChannelViewer->Hide() ;
}

void RegisterWindow::Quit(void)
{
  this->Hide() ;
}

void RegisterWindow::ShowStatus(const char * text)
{
  outStatus->value( text );
  Fl::check();
}


void RegisterWindow::ShowFixedImage(void)
{
  if( !m_FixedImageLoaded )
    {
      return;
    }

  m_FixedImageViewer->SetImage( m_FixedImage );  
  m_FixedImageViewer->Show();
}

void RegisterWindow::ShowMovingImage(void)
{
  if( !m_MovingImageLoaded )
    {
      return;
    }

  m_MovingImageViewer->SetImage( m_MovingImage );
  m_MovingImageViewer->Show();

  if (m_FixedImage != 0)
    {
      m_TempBox.X1 = (int) outSelectedRegionBeginX->value() ;
      m_TempBox.Y1 = (int) outSelectedRegionBeginY->value() ;
      m_TempBox.X2 = (int) outSelectedRegionEndX->value() ;
      m_TempBox.Y2 = (int) outSelectedRegionEndY->value() ;
      m_MovingImageViewer->imageViewer->SetSelectionBox(&m_TempBox) ;
    }
}

void RegisterWindow::ShowRegisteredImage(void)
{
  if( !m_RegisteredImageAvailable )
    {
      return;
    }

  
  m_RegisteredImageViewer->SetImage( m_RegisteredImage );
  m_RegisteredImageViewer->Show();
}

void RegisterWindow::ShowMixedChannel(void)
{
  if( !m_RegisteredImageAvailable )
    {
      return;
    }
  int sizeX = m_FixedImage->GetLargestPossibleRegion().GetSize()[0] ;
  int sizeY = m_FixedImage->GetLargestPossibleRegion().GetSize()[1] ;

  if (m_ChannelSources[0] == fixed)
    {
      m_MixedChannelViewer->SetRedChannel( m_FixedImage) ;
    }
  else if (m_ChannelSources[0] == registered)
    {
      m_MixedChannelViewer->SetRedChannel( m_RegisteredImage ) ;
    }
  else
    {
      m_MixedChannelViewer->FillChannel(0, 0, sizeX, sizeY) ;
    }
  
  if (m_ChannelSources[1] == fixed)
    {
      m_MixedChannelViewer->SetGreenChannel( m_FixedImage) ;
    }
  else if (m_ChannelSources[1] == registered)
    {
      m_MixedChannelViewer->SetGreenChannel( m_RegisteredImage ) ;
    }
  else
    {
      m_MixedChannelViewer->FillChannel(1, 0, sizeX, sizeY) ;
    }

  if (m_ChannelSources[2] == fixed)
    {
      m_MixedChannelViewer->SetBlueChannel( m_FixedImage) ;
    }
  else if (m_ChannelSources[2] == registered)
    {
      m_MixedChannelViewer->SetBlueChannel( m_RegisteredImage ) ;
    }
  else
    {
      m_MixedChannelViewer->FillChannel(2, 0, sizeX, sizeY) ;
    }

  m_MixedChannelViewer->Show();
}

void RegisterWindow::Execute(void)
{

  iterationsWindow->show();
  Fl::check();

  clock_t time_begin ;
  clock_t time_end ;

  this->ShowStatus("Registering Moving Image against Fixed Image ...");
  grpControls->deactivate() ;
  this->UpdateParameters() ;

  time_begin = clock() ;
  RegisterApplication::Execute();
  time_end = clock() ;

  menuRegisteredImageDisplay->activate() ;
  menuMixedChannel->activate() ;
  buSaveRegisteredImage->activate() ;
  grpControls->activate() ;
  itk::OStringStream message ;
  message
    << "Registration done in " << 
    double(time_end - time_begin) / CLOCKS_PER_SEC << "seconds." 
    << std::endl ;
  message << "angle = " 
    << m_Registrator->GetTransformParameters()[0] << ", x offset = "
    << m_Registrator->GetTransformParameters()[1] << ", y offset = "
    << m_Registrator->GetTransformParameters()[2];
  this->ShowStatus(message.str().c_str());

//    m_MovingImage->SetRequestedRegion(m_SelectedRegion) ;
//    m_DebugViewer->SetImage(m_MovingImage) ;
//    m_DebugViewer->Show() ;
//    m_MovingImage->SetRequestedRegionToLargestPossibleRegion() ;
}

void RegisterWindow::UpdateParameters(void)
{

  m_RegionBegin.resize(ImageDimension) ;
  m_RegionEnd.resize(ImageDimension) ;

  m_RegionBegin[0] = (int) outSelectedRegionBeginX->value() ;
  m_RegionBegin[1] = (int) outSelectedRegionBeginY->value() ;
  m_RegionEnd[0] = (int) outSelectedRegionEndX->value() ;
  m_RegionEnd[1] = (int) outSelectedRegionEndY->value() ;

  int i ;
  // crop images from original image using selected region 
  for (i = 0 ; i < ImageDimension ; i++)
    {
      if (m_RegionEnd[i] > m_RegionBegin[i])
        {
          m_SelectedSize[i] = m_RegionEnd[i] - m_RegionBegin[i] ;
          m_SelectedIndex[i] = m_RegionBegin[i] ;
        }
      else
        {
          m_SelectedSize[i] = m_RegionBegin[i] - m_RegionEnd[i] ;
          m_SelectedIndex[i] = m_RegionEnd[i] ;
        }
    }

  m_SelectedRegion.SetIndex(m_SelectedIndex) ;
  m_SelectedRegion.SetSize(m_SelectedSize) ;

}

void RegisterWindow::ShowAdvancedOptionsWindow()
{
  winAdvancedOptions->show() ;
  inTranslationScale->value(m_TranslationScale) ;
  inRotationScale->value(m_RotationScale) ;
  inLearningRate->value(m_LearningRate) ;
  inNumberOfIterations->value(m_NumberOfIterations) ;

  if (m_ChannelSources[0] == fixed)
    {
      buRedFixedImage->setonly() ;
    }
  else if (m_ChannelSources[0] == registered)
    {
      buRedRegisteredImage->setonly() ;
    }
  else
    {
      buRedNone->setonly() ;
    }

  if (m_ChannelSources[1] == fixed)
    {
      buGreenFixedImage->setonly() ;
    }
  else if (m_ChannelSources[1] == registered)
    {
      buGreenRegisteredImage->setonly() ;
    }
  else
    {
      buGreenNone->setonly() ;
    }

  if (m_ChannelSources[2] == fixed)
    {
      buBlueFixedImage->setonly() ;
    }
  else if (m_ChannelSources[2] == registered)
    {
      buBlueRegisteredImage->setonly() ;
    }
  else
    {
      buBlueNone->setonly() ;
    }
}

void RegisterWindow::UpdateAdvancedOptions(void)
{
  m_TranslationScale = inTranslationScale->value() ;
  m_RotationScale = inRotationScale->value() ;
  m_LearningRate = inLearningRate->value() ;
  m_NumberOfIterations = (int) inNumberOfIterations->value() ;

  if (buRedFixedImage->value() == 1)
    {
      m_ChannelSources[0] = fixed ;
    }
  else if (buRedRegisteredImage->value() == 1)
    {
      m_ChannelSources[0] = registered ;
    }
  else
    {
      m_ChannelSources[0] = none ;
    }

  if (buGreenFixedImage->value() == 1)
    {
      m_ChannelSources[1] = fixed ;
    }
  else if (buGreenRegisteredImage->value() == 1)
    {
      m_ChannelSources[1] = registered ;
    }
  else
    {
      m_ChannelSources[1] = none ;
    }


  if (buBlueFixedImage->value() == 1)
    {
      m_ChannelSources[2] = fixed ;
    }
  else if (buBlueRegisteredImage->value() == 1)
    {
      m_ChannelSources[2] = registered ;
    }
  else
    {
      m_ChannelSources[2] = none ;
    }

  winAdvancedOptions->hide() ;
}

void RegisterWindow::CloseAdvancedOptionsWindow(void)
{
  winAdvancedOptions->hide() ;
}

void RegisterWindow::SaveRegisteredImage( void )
{

  const char * filename = 
    fl_file_chooser("Registered Image filename","*.*","");

  if( !filename )
    {
      return;
    }
  
  this->ShowStatus("Saving registered image file...");
  
  try 
    {
      RegisterApplication::SaveRegisteredImage( filename );
    }
  catch( ... ) 
    {
      this->ShowStatus("Problems saving file format");
      return;
    }
  
  this->ShowStatus("Registered Image Saved");
}

void RegisterWindow
::SelectionCallBackWrapper(void* ptrSelf, 
                           fltk::Image2DViewerWindow::SelectionBoxType* box)
{
  RegisterWindow* self = (RegisterWindow*) ptrSelf ;
  self->UpdateSelectedRegion(box) ;
}

void RegisterWindow
::UpdateSelectedRegion(fltk::Image2DViewerWindow::SelectionBoxType* box)
{
  outSelectedRegionBeginX->value(box->X1) ;
  outSelectedRegionBeginY->value(box->Y1) ;
  outSelectedRegionEndX->value(box->X2) ;
  outSelectedRegionEndY->value(box->Y2) ;
  m_MovingImageViewer->imageViewer->SetSelectionBox(box) ;
}





