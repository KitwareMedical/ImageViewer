/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorPedicle.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liSAMPLEGENERATORPEDICLE
#define liSAMPLEGENERATORPEDICLE

#include "liSampleGeneratorPedicleGUI.h"

class liSampleGeneratorPedicle : public liSampleGeneratorPedicleGUI 
{
public:
  liSampleGeneratorPedicle();
  virtual ~liSampleGeneratorPedicle();
  virtual void Hide( void );
  virtual void ShowParameters( void );
  virtual void GenerateSamples( SamplesType & samples );
};

#endif
