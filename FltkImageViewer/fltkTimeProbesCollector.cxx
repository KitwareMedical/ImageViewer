/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkTimeProbesCollector.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "fltkTimeProbesCollector.h"
#include "fltkTimeProbeGUI.h"
#include <iostream>
#include <strstrea.h>

namespace fltk 
{


TimeProbesCollector
::TimeProbesCollector()
{
}



TimeProbesCollector
::~TimeProbesCollector()
{
}



void
TimeProbesCollector
::Clear(void) 
{
  TimeProbesCollectorBase::Clear();
  probesBrowser->clear();
  controlPanel->redraw();
  Fl::check();
}





void
TimeProbesCollector
::Stop(const char * nameid) 
{
  TimeProbesCollectorBase::Stop( nameid );
  if( continuousCheckButton->value() )
    {
    this->Report();
    }
}





void
TimeProbesCollector
::Start(const char * nameid) 
{
  TimeProbesCollectorBase::Start( nameid );
  if( continuousCheckButton->value() )
    {
    this->Report();
    }
}





void
TimeProbesCollector
::Report(void) const
{

  probesBrowser->clear();
  
  MapType::const_iterator pb  = m_Probes.begin();
  MapType::const_iterator end = m_Probes.end();

  while( pb != end )
    {
    *pb;
    const unsigned int len = 1000;
    char buffer[ len ];
    ostrstream text( buffer, len );
    text.width(20);
    text <<  pb->first.c_str() << "  ";
    text.width(10);
    text <<  pb->second.GetNumberOfCalls() <<  "   ";
    text.width(15);
    text <<  pb->second.GetMeanTime();
    text << '\0';
    probesBrowser->add( text.str() );
    ++pb;

    }
  
  probesBrowser->redraw();
  Fl::check();

}




}  // end namespace fltk


