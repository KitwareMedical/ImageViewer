/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Prokariote.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Prokariote_H
#define Prokariote_H

#include "Cell.h"

namespace bio {


/** \class Prokariote 
 * \brief This class implements the basic behavior of Prokariote cells. */
class Prokariote  : public Cell
{
public:
  typedef  Cell  SuperClass;

  Prokariote();
  virtual ~Prokariote();

  virtual void Grow(void);
  virtual void Mitosis(void);
  virtual void Apoptosis(void);

  virtual bool CheckPointDNAReplication(void);
  virtual bool CheckPointApoptosis(void);

  static  Cell * CreateEgg(void);

  static void SetDivisionMaximumLatencyTime( unsigned long );
  static unsigned long GetDivisionMaximumLatencyTime(void);

  static const char * GetSpeciesName(void) 
                          { return "Primitive Prokariote"; }
private:
  virtual Cell * CreateNew(void);
  unsigned long m_DivisionLatencyTime;

  static unsigned long DivisionMaximumLatencyTime;

};


} // end namespace bio

#endif
