/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkTimeProbesCollector.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkTimeProbesCollector_h
#define __fltkTimeProbesCollector_h


#include "fltkTimeProbesCollectorGUI.h"

namespace fltk
{


class TimeProbesCollector : public TimeProbesCollectorGUI
{

public:

  TimeProbesCollector();
  ~TimeProbesCollector();

  void Start( const char * );
  void Stop( const char * );

  void Report(void) const;
  void Clear(void);

};

} // end namespace fltk

#endif
