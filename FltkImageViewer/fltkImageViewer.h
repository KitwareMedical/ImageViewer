/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkImageViewer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2000 National Library of Medicine
All rights reserved.

See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __fltkImageViewer_h
#define __fltkImageViewer_h

#include "fltkImageViewerGUI.h"

namespace fltk {

template <class ImagePixelType, class OverlayPixelType>
class ImageViewer : public fltkImageViewerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef ImageViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

 
  typedef itk::PhysicalImage< ImagePixelType, 3 >   ImageType;
  typedef itk::PhysicalImage< OverlayPixelType, 3 > OverlayType;
  typedef GLSliceView< ImagePixelType, OverlayPixelType > GLSliceViewType;
    
  ImageViewer();
  virtual ~ImageViewer();
  virtual void open(void);
  virtual void SetImage(ImageBase<3> * img);
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
  virtual void setOrientation(void);
  virtual void CreateGLSliceView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void SetLabel( const char * );
  virtual void showClickedPointsWindow(void);
  virtual void updateClickedPoints(void);
  virtual void clearClickedPoints(void);

private:
  GLSliceViewType * glSliceView;
};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkImageViewer.txx>
#endif




#endif
