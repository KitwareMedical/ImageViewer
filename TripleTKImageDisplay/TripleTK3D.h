/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTK3D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
   /** Constructor: Calls FLVTKVolRenWin's constructor with fluid produced
    * parameters that specify the location, boundaries, and label of the 3D
    * window.  */
  TripleTK3D(int x, int y, int w, int h, const char *l);

  /**Deconstuctor.*/
  ~TripleTK3D(void);
    
  /**Centers the planes in the vtk window.*/
  void centerPlanes(void);
};

#endif

