/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTK3D.h
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
#ifndef _TRIPLETK3D_H
#define _TRIPLETK3D_H

#include "3D/FLVTKVolRenWin.h"

/** \class TripleTK3D
 * \brief 3D Window.
 *
 * This class is an abstraction of the class FLVTKVolRenWin.  It is
 * derived of this class for the purposes of being used with 
 * the TripleTKGUI structure.  This class will allow the viewing of
 * image data in 3D through its FLVTKVolRenWin inheritance and allows
 * for any necessary additions to FLVTKVolRenWin without actually 
 * having to manipulate FLVTKVolRenWin.
 */
class TripleTK3D : public FLVTKVolRenWin<short>
  {

  public:
        
   /**
    * Constructor:  Calls FLVTKVolRenWin's constructor with fluid produced
    * parameters that specify the location, boundaries, and label of the 3D window.
    */
    TripleTK3D(int x, int y, int w, int h, const char *l);

    /**Deconstuctor.*/
    ~TripleTK3D(void);
    
    /**Centers the planes in the vtk window.*/
    void centerPlanes(void);

  };


#endif

