/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSlice2DDrawer.h
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

#ifndef __fltkSlice2DDrawer_h
#define __fltkSlice2DDrawer_h

#include <fltkSlice2DDrawerGUI.h>
#include <itkImage.h>
#include "itkCommand.h"
#include <FL/gl.h>



namespace fltk {


template <class TImage>
class Slice2DDrawer : public fltkSlice2DDrawerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef Slice2DDrawer<TImage>       Self;

 
  /**
   *  Type of the image to draw slices from
   */
  typedef  TImage   ImageType;
 

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltkSlice2DDrawerGUI  Superclass;


  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Slice2DDrawer, fltkSlice2DDrawerGUI ); 


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  


  /**
   *  Pixel type from the image
   */
  typedef  typename ImageType::PixelType  PixelType;
 

  /**
   *  Region Type from the image
   */
  typedef  typename ImageType::RegionType  RegionType;
 

  /**
   *  Index Type from the image
   */
  typedef  typename ImageType::IndexType  IndexType;
 
  
  /**
   *  Size Type from the image
   */
  typedef  typename ImageType::SizeType  SizeType;
 

  /**
   * Command that will draw the object
   */
  typedef itk::SimpleConstMemberCommand< Self >   DrawCommandType;
  typedef typename DrawCommandType::Pointer       DrawCommandPointer;


  /**
   * Show
   */
  void Show(void);


  /**
   * Hide
   */
  void Hide(void);


  /**
   * Set Input Image
   */
  void SetInput( ImageType * );


  /**
   * Set Enabled
   */
  void SetEnabled( bool enabled );


  /**
   * Draw the target in Open Gl
   */
   void glDraw(void) const;


  /**
   * Bind texture for Slice 
   */
   void BindTexture(void) const;

  /**
   * Get the Observer/Command that will redraw the object
   */
  DrawCommandPointer GetDrawCommand(void);


protected:

  /**
   * Default constructor
   */
   Slice2DDrawer();


  /**
   * Destructor
   */
  ~Slice2DDrawer();



private:

   typename ImageType::Pointer      m_Image;
   
   int                    m_Nx;
   int                    m_Ny;
   
   int                    m_Nxr;
   int                    m_Nyr;
   
   float                  m_Dx;
   float                  m_Dy;

   float                  m_Cx;
   float                  m_Cy;
   
   float                  m_Ox;  // Origin in World Coordinates
   float                  m_Oy;
   
   unsigned char        * m_Slice;

   unsigned char        * m_Texture;
   mutable GLuint         m_TextureName;

   PixelType              m_Max_Value;

   mutable bool           m_TexturesGenerated;

   bool                   m_Enabled;

  DrawCommandPointer      m_DrawCommand;

};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkSlice2DDrawer.txx>
#endif




#endif




