/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GradientEatingBacteria.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef GradientEatingBacteria_H
#define GradientEatingBacteria_H

#include "PressureSensitiveBacteria.h"

namespace bio {

/** \class Bacteria 
 * \brief This class implements the basic behavior of 
 * pressure sensitive cells. This cells will arrest in
 * G1 if the pressure exceeds a limit.
 */
class GradientEatingBacteria  : public PressureSensitiveBacteria
{
public:
  typedef  PressureSensitiveBacteria  SuperClass;

  GradientEatingBacteria();
  virtual ~GradientEatingBacteria();

  virtual bool CheckPointDNAReplication(void);

  virtual void ClearForce(void);
  virtual void AddForce(const VectorType & force);
  virtual void ReceptorsReading(void);
  virtual void Mitosis( void );

  static  Cell * CreateEgg(void);

  static const char * GetSpeciesName(void) 
                          { return "Gradient Eating Bacteria"; }
private:
  virtual Cell * CreateNew(void);
 
  bool                     m_ScheduleApoptosis;
  double                   m_ChemoAttractantLevel;

  static ColorType         WellNourishedColor;
  static ColorType         HopefullColor;
  static ColorType         StarvingColor;

  static double            ChemoAttractantLowThreshold;
  static double            ChemoAttractantHighThreshold;
 
};


} // end namespace bio

#endif
