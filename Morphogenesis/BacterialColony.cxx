/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BacterialColony.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "BacterialColony.h"
#include "FL/gl.h"


namespace bio {




BacterialColony
::BacterialColony()
{

  Cell::ColorType color;
  color.SetRed(0.9);
  color.SetGreen(0.0);
  color.SetBlue(0.0);
  Cell::SetDefaultColor( color );

  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetCallbackFunction( this, &CellularAggregate::Draw );
}




BacterialColony
::~BacterialColony()
{
}




void
BacterialColony
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}




void
BacterialColony
::Show(void)
{
  m_GUI.Show();
}



void
BacterialColony
::Hide(void)
{
  m_GUI.Hide();
}





} // end namespace bio

