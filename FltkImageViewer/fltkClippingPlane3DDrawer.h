/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkClippingPlane3DDrawer.h
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

#ifndef __fltkClippingPlane3DDrawer_h
#define __fltkClippingPlane3DDrawer_h

#include "fltkClippingPlane3DDrawerGUI.h"
#include "itkImage.h"
#include "itkCommand.h"
#include <FL/gl.h>



namespace fltk {


class ClippingPlane3DDrawer : public fltkClippingPlane3DDrawerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef  ClippingPlane3DDrawer       Self;

 

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltkClippingPlane3DDrawerGUI  Superclass;


  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(ClippingPlane3DDrawer, fltkClippingPlane3DDrawerGUI);


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  



  /**
   * Command that will draw the object
   */
  typedef itk::SimpleConstMemberCommand< Self >   DrawCommandType;
  typedef DrawCommandType::Pointer                DrawCommandPointer;


  /**
   * Update internal data from the GUI
   */
   void UpdateFromGUI(void);


  /**
   * Update the GUI from the internal data 
   */
   void UpdateTheGUI(void);


  /**
   * Draw the target in Open Gl
   */
   void glDraw(void) const;


  /**
   * Get the Observer/Command that will redraw the object
   */
  DrawCommandPointer GetDrawCommand(void);


protected:

  /**
   * Default constructor
   */
   ClippingPlane3DDrawer();


  /**
   * Destructor
   */
  ~ClippingPlane3DDrawer();



private:

  double                 m_X;  // Plane orientation
  double                 m_Y;
  double                 m_Z;

  double                 m_R;  // Distance from the center

  bool                   m_Enabled;
   
  unsigned int           m_Id;

  static unsigned int    counter;
  
  DrawCommandPointer     m_DrawCommand;

};


} // end namespace fltk





#endif




