/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkTimeProbeGUI.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkTimeProbeGUI.h"


namespace fltk
{




TimeProbeGUI
::TimeProbeGUI(unsigned int line):Fl_Tile(0,0,10,10,"")
{

  const unsigned int nameWidth   = 200;
  const unsigned int countWidth  =  50;
  const unsigned int timeWidth   =  50;
  const unsigned int firstColumn =   5;

  const unsigned int rowHeight   =  25;

  const unsigned int width = nameWidth  + 
                             countWidth +
                             timeWidth;

  const unsigned int y = line * (rowHeight+2);

  this->size( width, rowHeight );

  m_NameButton = new Fl_Button(firstColumn,y,nameWidth,rowHeight);
  m_NameButton->align( FL_ALIGN_CLIP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE );
  m_NameButton->size(nameWidth,rowHeight);
  
  m_NumberOfCalls = new Fl_Value_Output(firstColumn+nameWidth,y,
                                        countWidth,rowHeight);
  m_NumberOfCalls->align( FL_ALIGN_CLIP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE );

  m_MeanTime      = new Fl_Value_Output(firstColumn+nameWidth+countWidth,y,
                                        timeWidth,rowHeight);
  m_MeanTime->align( FL_ALIGN_CLIP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE );

}



TimeProbeGUI
::~TimeProbeGUI()
{

}



void
TimeProbeGUI
::SetName( const std::string & name )
{
  m_NameButton->label( name.c_str() );
}



void 
TimeProbeGUI
::SetNumberOfCalls( unsigned int numcalls )
{
  m_NumberOfCalls->value( numcalls );
}


void 
TimeProbeGUI
::SetMeanTime( double meantime)
{
  m_MeanTime->value( meantime );

}


} // end namespace fltk
