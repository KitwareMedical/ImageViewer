/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkImageViewer.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2000 National Library of Medicine
All rights reserved.

See COPYRIGHT.txt for copyright details.

=========================================================================*/

#include "fltkImageViewer.h"


namespace fltk {
  
template <class ImagePixelType, class OverlayPixelType>
ImageViewer<ImagePixelType,OverlayPixelType>
::ImageViewer()
{
  glSliceView = 0;
  createGUI();
}


  
template <class ImagePixelType, class OverlayPixelType>
ImageViewer<ImagePixelType,OverlayPixelType>
::~ImageViewer()
{

}



  
template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::open(void)
{
}




template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetImage(ImageBase<3> * img)
{
  ImageType * image = dynamic_cast<ImageType *>( img );
  glSliceView->SetInputImage( image );
  synchronize();
}





template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::show(void)
{
  static bool firstTime = true;
  iviewWindow->show();
  glSliceView->show();
  glSliceView->update();
  if( firstTime )
  {
    firstTime = false;
    Fl::check();
    glSliceView->redraw();
    Fl::check();
  }
}





template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::hide(void)
{
  iviewWindow->hide();
  clickedPointsWindow->hide();
}






template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::update(void)
{
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::imageMode(ImageModeType mode)
{
  glSliceView->imageMode(mode);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
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
ImageViewer<ImagePixelType,OverlayPixelType>
::selectSlice(unsigned int num)
{
  sliceNumberSlider->value(num);
  glSliceView->sliceNum((int)sliceNumberSlider->value());
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::setIntensityWindowingMin(float val)
{
  intensityWindowingMinSlider->value(val);
  glSliceView->iwMin(intensityWindowingMinSlider->value());  
  glSliceView->update();
}








template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::setIntensityWindowingMax(float val)
{
  intensityWindowingMaxSlider->value(val);
  glSliceView->iwMax(intensityWindowingMaxSlider->value());  
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::centerWindow(void)
{
  glSliceView->winCenter();
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::zoomIn(void)
{
  glSliceView->winZoom(glSliceView->winZoom()*2.0f);
 glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::zoomOut(void)
{
  glSliceView->winZoom(glSliceView->winZoom()/2.0f);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::shiftUp(void)
{
  glSliceView->winShift(1,0);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::shiftDown(void)
{
  glSliceView->winShift(-1,0);
  glSliceView->update();
}








template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::shiftLeft(void)
{
  glSliceView->winShift(0,-1);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::shiftRight(void)
{
  glSliceView->winShift(0,1);
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetLabel(const char * label)
{
  iviewWindow->label( label );
}





template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::setOrientation(void)
{
  glSliceView->orientation( orientationChoice->value() );
  glSliceView->update();
  synchronize();
}









template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
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







template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::showClickedPointsWindow(void)
{
  clickedPointsWindow->show();
}





template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::clearClickedPoints(void)
{
  glSliceView->clearClickedPointsStored();
  clickedPointsBrowser->clear();
}




template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::updateClickedPoints(void)
{
  clickedPointsBrowser->clear();
  char buffer[200];
  const int numPoints = glSliceView->numClickedPointsStored();

  for(int i=numPoints-1; i>=0; i--)
  {
    const ClickPoint * point = glSliceView->getClickedPoint(i);
	const int x = (int)(point->x);
	const int y = (int)(point->y);
	const int z = (int)(point->z);
	const float value = point->value;
    sprintf(buffer,"%3d %3d %3d %f",x,y,z,value);
    clickedPointsBrowser->add( buffer );
  }

}



} // end namespace itk
