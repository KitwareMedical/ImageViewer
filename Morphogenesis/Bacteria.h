/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Bacteria.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Bacteria_H
#define Bacteria_H

#include "Prokariote.h"

namespace bio {

/** \class Bacteria 
 * \brief This class implements the basic behavior of Prokariote cells.
 */
class Bacteria  : public Prokariote
{
public:
  typedef  Prokariote  SuperClass;

  Bacteria();
  virtual ~Bacteria();

  static  Cell * CreateEgg(void);
  virtual void ComputeGeneNetwork(void);

  static const char * GetSpeciesName(void) 
                          { return "Primitive Bacteria"; }
private:
  virtual Cell * CreateNew(void);

};


} // end namespace bio

#endif
