/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IView.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2000 National Library of Medicine
All rights reserved.

See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef _IView_txx
#define _IView_txx

#include "IView.h"

  
template <class ImagePixelType, class OverlayPixelType>
IView<ImagePixelType,OverlayPixelType>
::IView()
{
  glSliceView = 0;
  createGUI();
}


  
template <class ImagePixelType, class OverlayPixelType>
IView<ImagePixelType,OverlayPixelType>
::~IView()
{

}



  
template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::open(void)
{
}




template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::SetImage(ImageType::Pointer img)
{ 
  /*Set a spacing if there is no define in the header file*/
  double *spacing = (double*)img->GetSpacing();

  if (spacing[0] == NULL )
    spacing[0] = 1;
  if (spacing[1] == NULL )
    spacing[1] = 1;
  if (spacing[2] == NULL )
    spacing[2] = 1;

  img->SetSpacing(spacing);
  glSliceView->SetInputImage( img );
  synchronize();
}





template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::show(void)
{
  iviewWindow->show();
  glSliceView->show();
  glSliceView->update();

}





template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::hide(void)
{
  iviewWindow->hide();
}






template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::update(void)
{
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::imageMode(ImageModeType mode)
{
  glSliceView->imageMode(mode);
  glSliceView->update();
}





template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::synchronize(void) 
{
  float iwDiff  = glSliceView->iwMax() - glSliceView->iwMin();
  float b       = (float)((int)log10(iwDiff)-2);
  double iwMin  = ((int)(glSliceView->iwMin()*pow(10, -b)))/pow(10,-b);
  double iwMax  = ((int)(glSliceView->iwMax()*pow(10, -b)))/pow(10,-b);
  double iwStep = (iwMax-iwMin)/100.0;
  sliceNumberSlider->range( 0.0f, glSliceView->numSlices() );
  intensityWindowingMinSlider->range(iwMin-iwStep,iwMax+iwStep);
  intensityWindowingMaxSlider->range(iwMin-iwStep,iwMax+iwStep);
  sliceNumberSlider->value((float)glSliceView->sliceNum());
  intensityWindowingMinSlider->step(iwStep);
  intensityWindowingMaxSlider->step(iwStep);
  intensityWindowingMinSlider->value(iwMin);
  intensityWindowingMaxSlider->value(iwMax);
}







template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::selectSlice(unsigned int num)
{
  sliceNumberSlider->value(num);
  glSliceView->sliceNum((int)sliceNumberSlider->value());
  glSliceView->update();
}





template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::setIntensityWindowingMin(float val)
{
  intensityWindowingMinSlider->value(val);
  glSliceView->iwMin(intensityWindowingMinSlider->value());  
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void 
IView<ImagePixelType,OverlayPixelType>
::setIntensityWindowingMax(float val)
{
  intensityWindowingMaxSlider->value(val);
  glSliceView->iwMax(intensityWindowingMaxSlider->value());  
  glSliceView->update();
}





template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::centerWindow(void)
{
  glSliceView->winCenter();
  glSliceView->update();
}




template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::zoomIn(void)
{
  glSliceView->winZoom(glSliceView->winZoom()*2.0f);
  glSliceView->update();
}





template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::zoomOut(void)
{
  glSliceView->winZoom(glSliceView->winZoom()/2.0f);
  glSliceView->update();
}




template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::shiftUp(void)
{
  glSliceView->winShift(1,0);
  glSliceView->update();
}




template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::shiftDown(void)
{
  glSliceView->winShift(-1,0);
  glSliceView->update();
}





template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::shiftLeft(void)
{
  glSliceView->winShift(0,-1);
  glSliceView->update();
}




template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::shiftRight(void)
{
  glSliceView->winShift(0,1);
  glSliceView->update();
}




template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::SetLabel(const char * label)
{
  iviewWindow->label( label );
}


template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::FlipX(void)
{
  glSliceView->flipX(!glSliceView->flipX());
  glSliceView->update();
}

template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::FlipY(void)
{
  glSliceView->flipY(!glSliceView->flipY());
  glSliceView->update();
}

template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::Orientation(unsigned int value)
{
  glSliceView->orientation(value);
  glSliceView->update();
}


template <class ImagePixelType, class OverlayPixelType>
void
IView<ImagePixelType,OverlayPixelType>
::CreateGLSliceView( Fl_Group * group, Fl_Gl_Window * original )
{

  short int x = original->x();
  short int y = original->y();
  short int w = original->w();
  short int h = original->h();

  const char * label = original->label();


  this->glSliceView = new GLSliceViewType(x,y,w,h,label);

  glSliceView->box( FL_EMBOSSED_BOX );

  group->remove( original );

  delete original;

}

#endif
