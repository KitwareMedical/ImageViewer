/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkDrawer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

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
