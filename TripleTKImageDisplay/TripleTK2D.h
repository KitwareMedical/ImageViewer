/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTK2D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _TRIPLETK2D_H
#define _TRIPLETK2D_H

#include <GLSliceView.h>

/** \class TripleTK2D
 * \brief 2D Window.
 *
 * This class is an abstraction of the class GLSliceView.  It is
 * derived of this class for the purposes of being used with 
 * the TripleTKGUI structure.  This class will allow the viewing of
 * image data in 2D through its GLSliceView inheritance and allows
 * for any necessary additions to GLSliceView without actually 
 * having to manipulate GLSliceView.
 */
class TripleTK2D : public GLSliceView<short, unsigned char>
{
public:
  /** Constructor: Calls GLSliceView's constructor with fluid produced
    * parameters that specify the location, boundaries, and label of the 2D
    * window.  */
  TripleTK2D(int x, int y, int w, int h, const char *l);
    
  /**Destructor.*/
  ~TripleTK2D();
    
  /**Return the original level value.*/
  float getOriginalLevel(void);
    
  /**Return the original window value.*/
  float getOriginalWindow(void);
    
  /**Set the original level value.*/
  void setOriginalLevel(float level);
    
  /**Set the original window value.*/
  void setOriginalWindow(float window);
    
  /**Return the view overlay data boolean from GLSliceView.*/
  bool getOverlayBoolean(void);
   
protected:
  float originalLevelValue;
  float originalWindowValue;
          
};


#endif
