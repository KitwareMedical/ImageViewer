/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    PressureSensitiveBacteria.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef PressureSensitiveBacteria_H
#define PressureSensitiveBacteria_H

#include "Bacteria.h"


namespace bio {


/**
 * \class Bacteria 
 * \brief This class implements the basic behavior of 
 * pressure sensitive cells. This cells will arrest in
 * G1 if the pressure exceeds a limit.
 */
class PressureSensitiveBacteria  : public Bacteria
{


public:

  typedef  Bacteria  SuperClass;

  PressureSensitiveBacteria();
  virtual ~PressureSensitiveBacteria();

  virtual void ComputeGeneNetwork(void);
  virtual bool CheckPointDNAReplication(void);


  static  Cell * CreateEgg(void);

  static const char * GetSpeciesName(void) 
                          { return "Pressure Sensitive Bacteria"; }

private:
  
  virtual Cell *   CreateNew(void);

  static double    PressureThresold;
  static double    PressureRamp;

  
};


} // end namespace bio

#endif
