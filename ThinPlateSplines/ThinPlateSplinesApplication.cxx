/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThinPlateSplinesApplication.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ThinPlateSplinesApplication.h"



ThinPlateSplinesApplication
::ThinPlateSplinesApplication()
{
}




ThinPlateSplinesApplication
::~ThinPlateSplinesApplication()
{
}





void 
ThinPlateSplinesApplication
::CreateLandMarks()
{
  this->ThinPlateSplinesApplicationBase::CreateLandMarks();
  targetLandMarkIdCounter->range( 0, m_TargetLandMarks.size()-1 );
  sourceLandMarkIdCounter->range( 0, m_TargetLandMarks.size()-1 );
}




void 
ThinPlateSplinesApplication
::DisplayLandMarks()
{
  this->ThinPlateSplinesApplicationBase::DisplayLandMarks();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}




void 
ThinPlateSplinesApplication
::UpdateSelectedTargetLandMark()
{
  const unsigned int landMarkId = targetLandMarkIdCounter->value();
  const CoordinateRepresentationType x =  xTargetValueInput->value();
  const CoordinateRepresentationType y =  yTargetValueInput->value();
  const CoordinateRepresentationType z =  zTargetValueInput->value();
  m_TargetLandMarks[ landMarkId ][0] = x;
  m_TargetLandMarks[ landMarkId ][1] = y;
  m_TargetLandMarks[ landMarkId ][2] = z;
  this->DisplayLandMarks();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}


void 
ThinPlateSplinesApplication
::SelectTargetLandMark()
{
  const unsigned int landMarkId = targetLandMarkIdCounter->value();
  const CoordinateRepresentationType x = m_TargetLandMarks[ landMarkId ][0];
  const CoordinateRepresentationType y = m_TargetLandMarks[ landMarkId ][1];
  const CoordinateRepresentationType z = m_TargetLandMarks[ landMarkId ][2];
  xTargetValueInput->value( x );
  yTargetValueInput->value( y );
  zTargetValueInput->value( z );
  Fl::check();
}




void 
ThinPlateSplinesApplication
::UpdateSelectedSourceLandMark()
{
  const unsigned int landMarkId = sourceLandMarkIdCounter->value();
  const CoordinateRepresentationType x =  xSourceValueInput->value();
  const CoordinateRepresentationType y =  ySourceValueInput->value();
  const CoordinateRepresentationType z =  zSourceValueInput->value();
  m_SourceLandMarks[ landMarkId ][0] = x;
  m_SourceLandMarks[ landMarkId ][1] = y;
  m_SourceLandMarks[ landMarkId ][2] = z;
  this->DisplayLandMarks();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}


void 
ThinPlateSplinesApplication
::SelectSourceLandMark()
{
  const unsigned int landMarkId = sourceLandMarkIdCounter->value();
  const CoordinateRepresentationType x = m_SourceLandMarks[ landMarkId ][0];
  const CoordinateRepresentationType y = m_SourceLandMarks[ landMarkId ][1];
  const CoordinateRepresentationType z = m_SourceLandMarks[ landMarkId ][2];
  xSourceValueInput->value( x );
  ySourceValueInput->value( y );
  zSourceValueInput->value( z );
  Fl::check();
}



