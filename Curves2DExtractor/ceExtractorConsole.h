/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ceExtractorConsole.h
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




#ifndef ce_ExtractorConsole_h
#define ce_ExtractorConsole_h

#include <fltkImage2DViewer.h>
#include <ceExtractorConsoleGUI.h>



class ceExtractorConsole : public ceExtractorConsoleGUI 
{

public:

  typedef fltk::Image2DViewer< PixelType > ImageViewerType;

  typedef fltk::Image2DViewer< InputPixelType > InputImageViewerType;

  typedef ImageViewerType::Pointer ImageViewerPointer;

  typedef InputImageViewerType::Pointer InputImageViewerPointer;

public:
  ceExtractorConsole();
  virtual ~ceExtractorConsole();
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Load(void);
  virtual void Quit(void);
  virtual void ShowStatus(const char * text);
  virtual void ShowInput(void);
  virtual void ShowFilteredX(void);
  virtual void ShowFilteredY(void);
  virtual void ShowSecondDerivativeX(void);
  virtual void ShowSecondDerivativeY(void);
  virtual void ShowCrossDerivativeXY(void);
  virtual void ShowLaplacian(void);
  virtual void ShowSmoothed(void);
  virtual void ShowGradientModulus(void);
  virtual void ShowMaxEigenValue(void);
  virtual void ShowMinEigenValue(void);
  virtual void Execute(void);

private:

  InputImageViewerPointer      m_InputViewer;

  ImageViewerPointer           m_Viewer_H1x;
  ImageViewerPointer           m_Viewer_H1y;

  ImageViewerPointer           m_Viewer_H2x;
  ImageViewerPointer           m_Viewer_H2y;

  ImageViewerPointer           m_Viewer_Hxy;
  ImageViewerPointer           m_Viewer_H1xy;

  ImageViewerPointer           m_Viewer_Smoothed;

  ImageViewerPointer           m_Viewer_Laplacian;

  ImageViewerPointer           m_Viewer_Gradient_Modulus;

  ImageViewerPointer           m_Viewer_Max_EigenValue;
  ImageViewerPointer           m_Viewer_Min_EigenValue;

};



#endif
