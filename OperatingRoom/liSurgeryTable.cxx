/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSurgeryTable.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include "liSurgeryTable.h"
#include "liCommandEvents.h"


namespace li {
  

///--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
SurgeryTable::SurgeryTable()
{
  m_Base       = fltk::Box3D::New();
  m_FixedPile  = fltk::Box3D::New();
  m_Pile       = fltk::Box3D::New();
  m_BedSupport = fltk::Box3D::New();
  m_Bed        = fltk::Box3D::New();

  // all the values in millimeters
  m_Base->SetLength( 400.0f );
  m_Base->SetWidth(  400.0f );
  m_Base->SetHeight( 100.0f );

  m_FixedPile->SetLength( 300.0f );
  m_FixedPile->SetWidth(  300.0f );
  m_FixedPile->SetHeight( 500.0f );
  
  m_Pile->SetLength( 200.0f  );
  m_Pile->SetWidth(  200.0f  );
  m_Pile->SetHeight( 500.0f  );
  
  m_BedSupport->SetLength(  1500.0f  );
  m_BedSupport->SetWidth(    600.0f  );
  m_BedSupport->SetHeight(   150.0f  );
  
  m_Bed->SetLength( 2000.0f );
  m_Bed->SetWidth(   500.0f );
  m_Bed->SetHeight(   50.0f );

  AddComponent(  m_Base.GetPointer() );
  AddComponent(  m_FixedPile.GetPointer() );
  AddComponent(  m_Pile.GetPointer() );
  AddComponent(  m_BedSupport.GetPointer() );
  AddComponent(  m_Bed.GetPointer() );

  ColorType color;
  color.Set( 0.8f, 0.9f, 0.8f );

  this->SetColor( color );

  m_Lateral  =   0.0;
  m_Axial    = 600.0;
  m_Vertical =  50.0;

  }





//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
SurgeryTable::~SurgeryTable() 
{

}




//--------------------------------------------------
//
//    Draw the Geometry of the Shape
//
//--------------------------------------------------
void 
SurgeryTable::DrawGeometry(void) const 
{

  m_Base->glDraw();
  glTranslated( 0.0, 0.0, m_Base->GetHeight() );
  
  m_FixedPile->glDraw();
  glTranslated( 0.0, 0.0, m_Vertical);
  
  m_Pile->glDraw();
  glTranslated( 0.0, m_Lateral, m_Pile->GetHeight());
  
  m_BedSupport->glDraw();
  const double border = m_BedSupport->GetHeight() - m_Bed->GetHeight() * 0.8;
  glTranslated( m_Axial, 0.0, border );
  
  m_Bed->glDraw();
  
  // at this level the patient will be drawn.
  glTranslated( 0.0, 0.0 , m_Bed->GetHeight() );
 

}



//--------------------------------------------------
//
//    Set the Axial (Cranio-Caudal) displacement
//
//--------------------------------------------------
void SurgeryTable::SetAxial( double axial ) 
{
  m_Axial = axial;
  InvokeEvent( TableMovedEvent() );
}




//--------------------------------------------------
//
//    Set the Vertical position
//
//--------------------------------------------------
void SurgeryTable::SetVertical( double vertical ) 
{
  m_Vertical = vertical;
  InvokeEvent( TableMovedEvent() );
}




//--------------------------------------------------
//
//    Draw the Geometry of the Shape
//
//--------------------------------------------------
void SurgeryTable::SetLateral( double lateral ) 
{
  m_Lateral = lateral;
  InvokeEvent( TableMovedEvent() );
}





//--------------------------------------------------
//
//    Get the Axial (Cranio-Caudal) displacement
//
//--------------------------------------------------
double SurgeryTable::GetAxial( void ) const
{
  return m_Axial;
}




//--------------------------------------------------
//
//    Get the Vertical position
//
//--------------------------------------------------
double SurgeryTable::GetVertical( void ) const
{
  return m_Vertical; 
}




//--------------------------------------------------
//
//    Draw the Geometry of the Shape
//
//--------------------------------------------------
double SurgeryTable::GetLateral( void ) const
{
  return m_Lateral;
}



   
} // end namespace li


