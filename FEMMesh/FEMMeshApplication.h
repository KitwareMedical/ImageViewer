/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMeshApplication.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef FEMMeshApplication_H
#define FEMMeshApplication_H

#include "FEMMeshApplicationGUI.h"

class FEMMeshApplication : public FEMMeshApplicationGUI 
{

public:

  FEMMeshApplication();
  ~FEMMeshApplication();
  
  void CreateFEMMesh(void);
  void DisplayFEMMesh(void);
  void ComputeArea(void) const;
  void ComputeHeatConduction(void) const;

private:
 

};


#endif

