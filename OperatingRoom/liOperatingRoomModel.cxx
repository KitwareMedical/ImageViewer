/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liOperatingRoomModel.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "liOperatingRoomModel.h"


namespace li {

  

///--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
OperatingRoomModel::OperatingRoomModel()
{

  m_Table                 =   SurgeryTable::New();
  m_Patient               =   Patient::New();
  m_FluoroscopyUnit       =   FluoroscopyCArmMobile::New();
  m_Floor                 =   fltk::Box3D::New();


  m_Floor->SetWidth(  2000.0f );
  m_Floor->SetLength( 3000.0f );
  m_Floor->SetHeight(  100.0f );

  fltk::Shape3D::ColorType color;

  color.SetRed(   0.9f );
  color.SetGreen( 0.8f );
  color.SetBlue(  0.7f );

  m_Floor->SetColor( color );

  m_Light0 = li::Light::New();
  m_Light1 = li::Light::New();

  color.SetRed(   0.7f );
  color.SetGreen( 0.7f );
  color.SetBlue(  0.7f );

  m_Light0->SetDiffuseColor( color );
  m_Light1->SetDiffuseColor( color );

  li::Light::PointType p0;
  li::Light::PointType p1;

  p0[0] = -200.0f;
  p0[1] = -500.0f;
  p0[2] =  500.0f;

  p1[0] =  200.0f;
  p1[1] =  500.0f;
  p1[2] =  500.0f;

  m_Light0->SetPosition( p0 );
  m_Light1->SetPosition( p1 );

  AddComponent(  m_Light0.GetPointer() );
  AddComponent(  m_Light1.GetPointer() );
  AddComponent(  m_Floor.GetPointer() );
  AddComponent(  m_Table.GetPointer() );
  AddComponent(  m_Patient.GetPointer() );
  AddComponent(  m_FluoroscopyUnit.GetPointer() );

}





//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
OperatingRoomModel::~OperatingRoomModel() 
{

}




//--------------------------------------------------
//
//    Draw the Geometry of the Room
//
//--------------------------------------------------
void 
OperatingRoomModel::DrawGeometry(void) const 
{

  m_Light0->glDraw();
  m_Light1->glDraw();

  glTranslated( 0.0f, 0.0f, -1000.f );

  m_Floor->glDraw();

  glPushMatrix();
    glTranslated(  140.0f,
                   m_Floor->GetWidth()  / 3.0f + 80.0f,
                   m_Floor->GetHeight() );

    m_FluoroscopyUnit->glDraw();

    glPopMatrix();


  glPushMatrix();
    glTranslated( -m_Floor->GetLength() / 4.0f,
                   0.0,
                   m_Floor->GetHeight() );

    m_Table->glDraw();


    m_Patient->glDraw();

  glPopMatrix();



          
}





//--------------------------------------------------
//
//    Returns the fluoroscopy unit
//
//--------------------------------------------------
li::FluoroscopyCArmMobile::Pointer
OperatingRoomModel::GetFluoroscopyUnit( void ) const 
{
  return m_FluoroscopyUnit;
}



//--------------------------------------------------
//
//    Returns the patient
//
//--------------------------------------------------
li::Patient::Pointer
OperatingRoomModel::GetPatient( void ) const 
{
  return m_Patient;
}


//--------------------------------------------------
//
//    Returns the Surgery Table
//
//--------------------------------------------------
li::SurgeryTable::Pointer
OperatingRoomModel::GetSurgeryTable( void ) const 
{
  return m_Table;
}



 
} // end namespace li




