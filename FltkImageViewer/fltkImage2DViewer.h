/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkImage2DViewer.h
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
#ifndef __fltkImage2DViewer_h
#define __fltkImage2DViewer_h

#include "itkObject.h"
#include "itkImage.h"
#include "fltkImage2DViewerGUI.h"
#include "itkCommand.h"

namespace fltk {

template <class ImagePixelType>
class Image2DViewer : public itk::Object, public fltkImage2DViewerGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef Image2DViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(Image2DViewer, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Image Type
   */
  typedef itk::Image< ImagePixelType, 2 >   ImageType;
  
  
  /**
   * Observer Command
   */
  typedef itk::SimpleMemberCommand< Self >      ObserverCommandType;


  /**
   * Observer Command
   */
  virtual void SetImage(ImageType * image);
  

  /**
   * Update the image rendering
   */
  virtual void Update(void);

  /**
   * Return the notifier of the Window, in order
   * to register additional drawers
   */
  itk::Object::Pointer GetNotifier(void);

  /**
   * Return the Command responsible for redrawing
   * the window. This command is intended to be registered
   * as observer of the objects whose changes will modify 
   * the display of the image.
   */
  itkGetObjectMacro( RedrawCommand, ObserverCommandType );

protected:
  
  Image2DViewer();
  virtual ~Image2DViewer();


private:
  typename ImageType::Pointer              m_Image;
  typename ObserverCommandType::Pointer    m_RedrawCommand; 
  unsigned long                            m_Tag;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkImage2DViewer.txx>
#endif




#endif
