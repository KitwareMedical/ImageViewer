/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liOperatingRoomView.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liOPERATINGROOMVIEWCLASS
#define liOPERATINGROOMVIEWCLASS

#include "itkWin32Header.h"
#include "itkObject.h"
#include "fltkGlWindowInteractive.h"

namespace li 
{
/** This class emulates a video camera observing the operating room. It
 * allows the user to interactively change positions, zoom and angles of
 * view.  */
class OperatingRoomView : public fltk::GlWindowInteractive
{
public:
  OperatingRoomView(int x,int y,int w,int h, const char * label=0);
  void draw(void);
  typedef itk::Object lightObject;
  lightObject::Pointer  GetNotifier(void);

private:
  lightObject::Pointer  m_Notifier;

};

} // end namespace li


#endif   



