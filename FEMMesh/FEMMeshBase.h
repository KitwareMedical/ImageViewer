/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMeshBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef FEMMeshBase_H
#define FEMMeshBase_H


#include "itkMacro.h"

#include "vtkRenderWindow.h"

// This is the base classe for the Application

class FEMMeshBase 
{

public:

  FEMMeshBase();
  ~FEMMeshBase();
  
  virtual void CreateSphere(void);
  virtual void CreateTriangle(void);

protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;

};


#endif
