/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IView.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
