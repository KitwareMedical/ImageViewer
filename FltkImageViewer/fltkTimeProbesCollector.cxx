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
#include <strstream>

namespace fltk 
{


TimeProbesCollector
::TimeProbesCollector()
{
 timeResolution->value( 1.0f / CLOCKS_PER_SEC );
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
  probesPack->clear();
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

  probesPack->clear();
  probesPack->begin();  
  MapType::const_iterator probe  = m_Probes.begin();
  MapType::const_iterator end    = m_Probes.end();

  while( probe != end )
    {
    TimeProbeGUI * gui = new TimeProbeGUI();
    gui->SetName( probe->first );
    gui->SetNumberOfStarts( probe->second.GetNumberOfStarts() );
    gui->SetNumberOfStops( probe->second.GetNumberOfStops() );
    gui->SetMeanTime( probe->second.GetMeanTime() );
    ++probe;

    }
  probesPack->end();  
  probesPack->redraw();
  Fl::check();

}



}  // end namespace fltk


