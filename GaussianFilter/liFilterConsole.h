/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole.h
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




#ifndef liFILTERCONSOLE
#define liFILTERCONSOLE

#include <fltkImageViewer.h>
#include <liFilterConsoleGUI.h>



class liFilterConsole : public liFilterConsoleGUI {

public:
  typedef unsigned char  OverlayPixelType;
  
  typedef fltk::ImageViewer< PixelType,
                       OverlayPixelType > ImageViewerType;

  typedef fltk::ImageViewer< InputPixelType,     
                       OverlayPixelType > InputImageViewerType;

public:
  liFilterConsole();
  virtual ~liFilterConsole();
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Load(void);
  virtual void Quit(void);
  virtual void ShowStatus(const char * text);
  virtual void ShowInput(void);
  virtual void ShowFilteredX(void);
  virtual void ShowFilteredY(void);
  virtual void ShowFilteredZ(void);
  virtual void ShowSecondDerivativeX(void);
  virtual void ShowSecondDerivativeY(void);
  virtual void ShowSecondDerivativeZ(void);
  virtual void ShowLaplacian(void);
  virtual void ShowGradientModulus(void);
  virtual void ShowAbout(void);
  virtual void Execute(void);
  virtual void WriteLaplacian(void);
  virtual void WriteGradientX(void);
  virtual void WriteGradientY(void);
  virtual void WriteGradientZ(void);
  virtual void WriteGradientModulus(void);

private:

  InputImageViewerType           * m_InputViewer;

  ImageViewerType           * m_Viewer_H1x;
  ImageViewerType           * m_Viewer_H1y;
  ImageViewerType           * m_Viewer_H1z;

  ImageViewerType           * m_Viewer_H2x;
  ImageViewerType           * m_Viewer_H2y;
  ImageViewerType           * m_Viewer_H2z;

  ImageViewerType           * m_Viewer_Laplacian;

  ImageViewerType           * m_Viewer_Gradient_Modulus;

};



#endif
