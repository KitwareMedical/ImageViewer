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
  targetLandMarkIdCounter->range( 0, m_TargetLandMarks->GetNumberOfPoints()-1 );
  sourceLandMarkIdCounter->range( 0, m_SourceLandMarks->GetNumberOfPoints()-1 );
  this->SelectSourceLandMark();
  this->SelectTargetLandMark();
}



void 
ThinPlateSplinesApplication
::MapPoints()
{
  this->ThinPlateSplinesApplicationBase::MapPoints();
  this->RemoveActors();
  this->DisplayAxes();
  this->DisplayPoints();
  this->DisplayLandMarks();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}



void
ThinPlateSplinesApplication
::MapPointsITK(void)
{
  const int splineType = splineKernelITKChoice->value();
  switch( splineType )
  {
  case 0:
    this->MapPointsThinPlateSplineITK();
    break;
  case 1:
    this->MapPointsThinPlateR2LogRSplineITK();
    break;
  case 2:
    this->MapPointsElasticBodySplineITK();
    break;
  case 3:
    this->MapPointsElasticBodyReciprocalSplineITK();
    break;
  case 4:
    this->MapPointsVolumeSplineITK();
    break;
  }
}
  


void
ThinPlateSplinesApplication
::MapPointsVTK(void)
{
  const int splineType = splineKernelVTKChoice->value();
  switch( splineType )
  {
  case 0:
    m_ThinPlateSplineTransformVTK->SetBasisToR();
    break;
  case 1:
    m_ThinPlateSplineTransformVTK->SetBasisToR2LogR();
    break;
  }
  this->ThinPlateSplinesApplicationBase::MapPointsVTK();
}




void 
ThinPlateSplinesApplication
::CreateSourcePoints()
{
  this->ThinPlateSplinesApplicationBase::CreateSourcePoints();
  this->DisplayPoints();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
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
  const unsigned int landMarkId = 
          static_cast<unsigned int>( targetLandMarkIdCounter->value() );

  const CoordinateRepresentationType x =  xTargetValueInput->value();
  const CoordinateRepresentationType y =  yTargetValueInput->value();
  const CoordinateRepresentationType z =  zTargetValueInput->value();

  PointSetType::PointsContainer::Pointer targetLandmarks = 
                                            m_TargetLandMarks->GetPoints();

  PointType & landmark = targetLandmarks->ElementAt( landMarkId );
  landmark[0] = x;
  landmark[1] = y;
  landmark[2] = z;

  this->RemoveActors();
  this->CreateSourcePoints();
  this->DisplayLandMarks();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}


void 
ThinPlateSplinesApplication
::SelectTargetLandMark()
{
  const unsigned int landMarkId = 
          static_cast<unsigned int>( targetLandMarkIdCounter->value() );

  PointSetType::PointsContainer::Pointer targetLandmarks = 
                                            m_TargetLandMarks->GetPoints();

  PointType & landmark = targetLandmarks->ElementAt( landMarkId );

  const CoordinateRepresentationType x = landmark[0];
  const CoordinateRepresentationType y = landmark[1];
  const CoordinateRepresentationType z = landmark[2];

  xTargetValueInput->value( x );
  yTargetValueInput->value( y );
  zTargetValueInput->value( z );

  Fl::check();

}




void 
ThinPlateSplinesApplication
::UpdateSelectedSourceLandMark()
{
  const unsigned int landMarkId = 
          static_cast<unsigned int>( sourceLandMarkIdCounter->value() );

  const CoordinateRepresentationType x =  xSourceValueInput->value();
  const CoordinateRepresentationType y =  ySourceValueInput->value();
  const CoordinateRepresentationType z =  zSourceValueInput->value();

  PointSetType::PointsContainer::Pointer sourceLandmarks = 
                                            m_SourceLandMarks->GetPoints();

  PointType & landmark = sourceLandmarks->ElementAt( landMarkId );
  landmark[0] = x;
  landmark[1] = y;
  landmark[2] = z;

  this->RemoveActors();
  this->CreateSourcePoints();
  this->DisplayLandMarks();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}


void 
ThinPlateSplinesApplication
::SelectSourceLandMark()
{
  const unsigned int landMarkId = 
          static_cast<unsigned int>( sourceLandMarkIdCounter->value() );

  PointSetType::PointsContainer::Pointer sourceLandmarks = 
                                            m_SourceLandMarks->GetPoints();

  PointType & landmark = sourceLandmarks->ElementAt( landMarkId );

  const CoordinateRepresentationType x = landmark[0];
  const CoordinateRepresentationType y = landmark[1];
  const CoordinateRepresentationType z = landmark[2];

  xSourceValueInput->value( x );
  ySourceValueInput->value( y );
  zSourceValueInput->value( z );

  Fl::check();
}




void 
ThinPlateSplinesApplication
::Show(void)
{
  m_FlRenderWindowInteractor->Initialize();
  this->ThinPlateSplinesApplicationGUI::Show();
  this->CreateSpline();
}
