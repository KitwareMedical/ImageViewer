#ifndef __RegisterWindow_h
#define __RegisterWindow_h

#include "itkRGBPixel.h"
#include <fltkImage2DViewer.h>
#include <fltkRGBImage2DViewer.h>
#include <registerGUI.h>

class RegisterWindow : public RegisterGUI
{
public:
  typedef enum {fixed, registered, none} ChannelSourceIdentifierType ;
  typedef unsigned char PixelType ;
  typedef unsigned char OverlayPixelType ;
  typedef itk::RGBPixel< PixelType > MixedChannelPixelType ;
  typedef fltk::Image2DViewer< PixelType > ImageViewerType ;
  typedef fltk::RGBImage2DViewer< MixedChannelPixelType > 
  MixedChannelViewerType ;

  RegisterWindow() ;
  virtual ~RegisterWindow() ;
  virtual void Show(void);
  virtual void Hide(void);

  virtual void LoadFixedImage(void);
  virtual void LoadMovingImage(void);

  virtual void Quit(void);
  virtual void ShowStatus(const char * text);

  virtual void ShowFixedImage(void);
  virtual void ShowMovingImage(void);
  virtual void ShowRegisteredImage(void);
  virtual void ShowMixedChannel(void) ;

  virtual void Execute(void);
  virtual void UpdateParameters(void);

  virtual void ShowAdvancedOptionsWindow(void) ;
  virtual void UpdateAdvancedOptions(void) ;
  virtual void CloseAdvancedOptionsWindow(void) ;
  virtual void SaveRegisteredImage(void) ;
  
  static void SelectionCallBackWrapper(void* ptrSelf, 
                                       fltk::Image2DViewerWindow::SelectionBoxType* box) ;
  virtual void UpdateSelectedRegion(fltk::Image2DViewerWindow::SelectionBoxType* box) ;

private:
  ImageViewerType::Pointer m_MovingImageViewer ;
  ImageViewerType::Pointer m_FixedImageViewer ;
  ImageViewerType::Pointer m_RegisteredImageViewer ;
  MixedChannelViewerType::Pointer m_MixedChannelViewer ;
  ImageViewerType::Pointer m_DebugViewer ;
  itk::FixedArray< ChannelSourceIdentifierType, 3 > m_ChannelSources ;
  fltk::Image2DViewerWindow::SelectionBoxType m_TempBox ;
} ; // end of class

#endif







