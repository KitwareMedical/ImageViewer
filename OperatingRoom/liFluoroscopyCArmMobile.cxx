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


#include <liFluoroscopyCArmMobile.h>
#include <liCommandEvents.h>


namespace li {
  

///--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
FluoroscopyCArmMobile::FluoroscopyCArmMobile()
{

  m_CArm                  = FluoroscopyCArm::New();
  m_Base                  = fltk::Box3D::New();
  m_Pile                  = fltk::Box3D::New();

  // The following values are in millimeters

  m_Base->SetLength( 400.0f );
  m_Base->SetWidth(  800.0f );
  m_Base->SetHeight( 100.0f );

  m_Pile->SetLength( 250.0f );
  m_Pile->SetWidth(  600.0f );
  m_Pile->SetHeight( 600.0f );

  m_CArm->SetCranioCaudalAngle( 45.0 );

  AddComponent(  m_Base.GetPointer() );
  AddComponent(  m_Pile.GetPointer() );
  AddComponent(  m_CArm.GetPointer() );

  m_AxisTilt     = 45; // degrees
  m_AzimuthAngle =  0; // degrees

  m_Position[0] = 0;
  m_Position[1] = 0;

}





//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
FluoroscopyCArmMobile::~FluoroscopyCArmMobile() 
{

}





//--------------------------------------------------
//
//    Set the cranio/caudal position of the machine on the floor
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::RefreshObservers( void ) const
{
  Redraw();
  InvokeEvent( FluoroscopyUnitMovedEvent() );
}



//--------------------------------------------------
//
//    Set the visibility of the Field of View Cone
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetFovVisible( bool visibility )
{
  m_CArm->SetFovVisible( visibility );
  Redraw();
}



//--------------------------------------------------
//
//    Set the cranio/caudal position of the machine on the floor
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetCranioCaudalPosition( double position )
{
  m_Position[0] = position;
  RefreshObservers();
}


//--------------------------------------------------
//
//    Set the Lateral position of the machine on the floor
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetLateralPosition( double position )
{
  m_Position[1] = position;
  RefreshObservers();
}






//--------------------------------------------------
//
//    Set the position of the machine on the floor
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetPosition(const Point2DType & position)
{
  m_Position = position;
  RefreshObservers();
}


//--------------------------------------------------
//
//    Set the orientation of the machine on the floor
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetAzimuthAngle( double angle )
{
  m_AzimuthAngle = angle;
  RefreshObservers();
}



//--------------------------------------------------
//
//    Set the tilt of the axis supporting the C-Arm
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetAxisTilt( double angle )
{
  m_AxisTilt = angle;
  RefreshObservers();
}


//--------------------------------------------------
//
//    Set the Rao/Lao angle
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetRaoLaoAngle( double angle )
{
  m_CArm->SetRaoLaoAngle( angle );
  RefreshObservers();
}




//--------------------------------------------------
//
//    Set the Cranio/Caudal angle
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetCranioCaudalAngle( double angle )
{
  m_CArm->SetCranioCaudalAngle( angle );
  RefreshObservers();
}




//--------------------------------------------------
//
//    Set Source to Film Distance 
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetSourceToFilmDistance( double distance )
{
  m_CArm->SetSourceToFilmDistance( distance );
  RefreshObservers();
}


//--------------------------------------------------
//
//    Get Source to Film Distance 
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetSourceToFilmDistance( void ) const
{
  return m_CArm->GetSourceToFilmDistance();
}




//--------------------------------------------------
//
//    Set Detector Radius
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::SetDetectorRadius( double radius )
{
  m_CArm->SetDetectorRadius( radius );
  RefreshObservers();
}


//--------------------------------------------------
//
//    Set Detector Position
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::
SetDetectorPosition( FluoroscopyCArm::DetectorConfiguration updown )
{
  m_CArm->SetDetectorPosition( updown );
  RefreshObservers();
}





//--------------------------------------------------
//
//    Get the position of the machine on the floor
//
//--------------------------------------------------
const FluoroscopyCArmMobile::Point2DType &
FluoroscopyCArmMobile::GetPosition(void) const
{
  return m_Position;
}


//--------------------------------------------------
//
//    Get the cranio/caudal position of the machine on the floor
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetCranioCaudalPosition( void ) const
{
  return m_Position[0];
}


//--------------------------------------------------
//
//    Get the Lateral position of the machine on the floor
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetLateralPosition( void ) const
{
  return m_Position[1]; 
}




//--------------------------------------------------
//
//    Get the orientation of the machine on the floor
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetAzimuthAngle( void ) const
{
  return m_AzimuthAngle;
}



//--------------------------------------------------
//
//    Get the tilt of the axis supporting the C-Arm
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetAxisTilt( void ) const
{
  return m_AxisTilt;
}


//--------------------------------------------------
//
//    Get the Rao/Lao angle
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetRaoLaoAngle( void ) const
{
  return m_CArm->GetRaoLaoAngle();
}




//--------------------------------------------------
//
//    Get the Cranio/Caudal angle
//
//--------------------------------------------------
double 
FluoroscopyCArmMobile::GetCranioCaudalAngle( void ) const
{
  return m_CArm->GetCranioCaudalAngle();
}






//--------------------------------------------------
//
//    Get position of the X-ray source
//
//--------------------------------------------------
const FluoroscopyCArmMobile::PointType &
FluoroscopyCArmMobile::GetSourcePosition( void ) const
{
  return m_CArm->GetSourcePosition();
}




//--------------------------------------------------------
//
// Get radius of the X-ray detector (image intensifier)
//
//--------------------------------------------------------
double
FluoroscopyCArmMobile::GetDetectorRadius( void ) const
{
  return m_CArm->GetDetectorRadius();
}



//--------------------------------------------------------
//
// Get position of the X-ray detector (image intensifier)
//
//--------------------------------------------------------
const FluoroscopyCArmMobile::PointType &
FluoroscopyCArmMobile::GetDetectorPosition( void ) const
{
  return m_CArm->GetDetectorPosition();
}




//--------------------------------------------------------
//
// Get vertical direction of the X-ray detector (image intensifier)
//
//--------------------------------------------------------
const FluoroscopyCArmMobile::VectorType &
FluoroscopyCArmMobile::GetDetectorVerticalDirection( void ) const
{
  return m_CArm->GetDetectorVerticalDirection();
}





//--------------------------------------------------
//
//    Draw the Geometry of the Shape
//
//--------------------------------------------------
void 
FluoroscopyCArmMobile::DrawGeometry(void) const 
{

  glPushMatrix();

  glTranslated( m_Position[0], m_Position[1], 0 );
  glRotated( m_AzimuthAngle, 0.0, 0.0, 1.0 );

  m_Base->DrawGeometry();

  glTranslated( 0.0, 0.0, m_Base->GetHeight() );
  m_Pile->DrawGeometry();

  glTranslated( 0.0, -m_Pile->GetWidth()/2.0f , m_Pile->GetHeight() );
  glRotated( 90+m_AxisTilt, -1.0, 0.0, 0.0 );

  m_CArm->DrawGeometry();

  glPopMatrix();

}



  
} // end namespace li




