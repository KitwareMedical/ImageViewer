/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkDrawer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef fltkDRAWERCLASS
#define fltkDRAWERCLASS

namespace fltk {


class Drawer 
{

public:
  Drawer() {};
  virtual ~Drawer() {};
  virtual void Draw(void) const =0;
};


}

#endif
