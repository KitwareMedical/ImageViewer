/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IView.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2000 National Library of Medicine
All rights reserved.

See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __IView_h
#define __IView_h

#include "IViewGUI.h"

template <class ImagePixelType, class OverlayPixelType>
class IView : public IViewGUI
{
public:
  
  typedef itk::PhysicalImage< ImagePixelType, 3 >   ImageType;
  typedef itk::PhysicalImage< OverlayPixelType, 3 > OverlayType;
  typedef GLSliceView< ImagePixelType, OverlayPixelType > GLSliceViewType;
    
  IView();
  virtual ~IView();
  virtual void open(void);
  virtual void SetImage(ImageType::Pointer img);
  virtual void SetOverlay(OverlayType::Pointer overlay);
  virtual void show(void);
  virtual void hide(void);
  virtual void update(void);
  virtual void synchronize(void);
  virtual void imageMode(ImageModeType mode);
  virtual void selectSlice(unsigned int num);
  virtual void setIntensityWindowingMin(float val);
  virtual void setIntensityWindowingMax(float val);
  virtual void centerWindow(void);
  virtual void zoomIn(void);
  virtual void zoomOut(void);
  virtual void shiftUp(void);
  virtual void shiftDown(void);
  virtual void shiftLeft(void);
  virtual void shiftRight(void);
  virtual void CreateGLSliceView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void SetLabel( const char * );
  virtual void FlipX(void);
  virtual void FlipY(void);
  virtual void Orientation(unsigned int);

private:
  GLSliceViewType * glSliceView;
};


#ifndef ITK_MANUAL_INSTANTIATION
#include <IView.txx>
#endif


#endif
