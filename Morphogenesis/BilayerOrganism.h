/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BilayerOrganism.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef BilayerOrganism_H
#define BilayerOrganism_H

#include "Eukariote.h"

namespace bio {

/** \class BilayerOrganism 
 * \brief This class implements the basic behavior of Eukariote cells. */
class BilayerOrganism  : public Eukariote
{
public:
  typedef  Cell  Eukariote;

  BilayerOrganism();
  virtual ~BilayerOrganism();

  static  Cell * CreateEgg(void);

  virtual void ComputeGeneNetwork(void);

  static const char * GetSpeciesName(void) 
                          { return "Primitive BilayerOrganism"; }
private:
  virtual Cell * CreateNew(void);


};


} // end namespace bio

#endif
