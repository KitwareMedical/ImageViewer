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


#include <liFluoroscopyCArm.h>



namespace li {
  

///--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
FluoroscopyCArm::FluoroscopyCArm()
{

  m_CArm                  = C_Arm3D::New();
  m_Intensifier           = fltk::Cylinder3D::New();
  m_IntensifierSupport    = fltk::Box3D::New();
  m_Detector              = fltk::Cylinder3D::New();
  m_DetectorSupport       = fltk::Cylinder3D::New();
  m_Source                = fltk::Box3D::New();
  m_SourceSupport         = fltk::Box3D::New();
  m_FovCone               = fltk::Cone3D::New();
  m_TurningBase           = fltk::Box3D::New();

  m_IntensifierOffset     = 0;

  // The following values are in millimeters

  m_TurningBase->SetLength( 200.0f );
  m_TurningBase->SetWidth(  200.0f );
  m_TurningBase->SetHeight( 200.0f );

  m_IntensifierSupport->SetLength( 140.0f );
  m_IntensifierSupport->SetWidth(  140.0f );
  m_IntensifierSupport->SetHeight( 150.0f );

  m_Intensifier->SetRadius(  70.0f );
  m_Intensifier->SetHeight( 200.0f );
  m_Intensifier->SetNumberOfSectors( 16 );

  m_Detector->SetRadius(  100.0f );
  m_Detector->SetHeight(  10.0f );
  m_Detector->SetNumberOfSectors( 16 );

  m_DetectorSupport->SetRadius( m_Detector->GetRadius() * 1.1 );
  m_DetectorSupport->SetHeight( 100.0f );
  m_DetectorSupport->SetNumberOfSectors( 16 );

  m_Source->SetLength( 150.0f );
  m_Source->SetWidth(  150.0f );
  m_Source->SetHeight( 150.0f );

  m_SourceSupport->SetLength( 200.0f );
  m_SourceSupport->SetWidth(  200.0f );
  m_SourceSupport->SetHeight( 100.0f );

  m_CArm->SetRadius( 590.0f );
  m_CArm->SetRadialWidth( 100.0f );
  m_CArm->SetWidth( 100.0f );
  m_CArm->SetNumberOfSectors( 20 );


  m_FovCone->SetRadius( m_Detector->GetRadius() );
  m_FovCone->SetHeight( m_CArm->GetRadius() * 2.0 );
  m_FovCone->SetNumberOfSectors( 16 );

  AddComponent(  m_TurningBase.GetPointer() );
  AddComponent(  m_CArm.GetPointer() );
  AddComponent(  m_IntensifierSupport.GetPointer() );
  AddComponent(  m_Intensifier.GetPointer() );
  AddComponent(  m_Detector.GetPointer() );
  AddComponent(  m_DetectorSupport.GetPointer() );
  AddComponent(  m_Source.GetPointer() );
  AddComponent(  m_SourceSupport.GetPointer() );
  AddComponent(  m_FovCone.GetPointer() );

  ColorType color;
  color.Set( 0.8f, 0.8f, 0.9f );

  this->SetColor( color );

  m_CranioCaudalAngle  = 0.0f;
  m_RaoLaoAngle        = 0.0f;

  m_FovIsVisible       = true;
}





//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
FluoroscopyCArm::~FluoroscopyCArm() 
{

}




//--------------------------------------------------
//
//    Get the Rao/Lao angle
//
//--------------------------------------------------
double 
FluoroscopyCArm::GetRaoLaoAngle( void ) const
{
  return m_RaoLaoAngle;
}




//--------------------------------------------------
//
//    Get the Cranio/Caudal angle
//
//--------------------------------------------------
double 
FluoroscopyCArm::GetCranioCaudalAngle( void ) const
{
  return m_CranioCaudalAngle;
}




//--------------------------------------------------
//
//    Get Source to Film distance
//
//--------------------------------------------------
double
FluoroscopyCArm::GetSourceToFilmDistance( void ) const
{
  return 2.0 * m_CArm->GetRadius();
}




//--------------------------------------------------
//
//    Get position of the X-ray source
//
//--------------------------------------------------
const FluoroscopyCArm::PointType &
FluoroscopyCArm::GetSourcePosition( void ) const
{
  return m_Source->GetCurrentPosition();
}




//--------------------------------------------------------
//
// Get position of the X-ray detector (image intensifier)
//
//--------------------------------------------------------
const FluoroscopyCArm::PointType &
FluoroscopyCArm::GetDetectorPosition( void ) const
{
  return m_Detector->GetCurrentPosition();
}




//--------------------------------------------------------
//
// Get vertical direction of the X-ray detector (image intensifier)
//
//--------------------------------------------------------
const FluoroscopyCArm::VectorType &
FluoroscopyCArm::GetDetectorVerticalDirection( void ) const
{
  return m_DetectorVerticalDirection;
}


//--------------------------------------------------------
//
// Get radius of the X-ray detector (image intensifier)
//
//--------------------------------------------------------
double
FluoroscopyCArm::GetDetectorRadius( void ) const
{
  return m_Detector->GetRadius();
}





//--------------------------------------------------
//
//    Set the Rao/Lao angle
//
//--------------------------------------------------
void 
FluoroscopyCArm::SetRaoLaoAngle( double angle )
{
  m_RaoLaoAngle = angle;
}




//--------------------------------------------------
//
//    Set the Cranio/Caudal angle
//
//--------------------------------------------------
void 
FluoroscopyCArm::SetCranioCaudalAngle( double angle )
{
  m_CranioCaudalAngle = angle;
}



//--------------------------------------------------
//
//    Set the visibility of the Field of View Cone
//
//--------------------------------------------------
void 
FluoroscopyCArm::SetFovVisible( bool visibility )
{
  m_FovIsVisible = visibility;
}



//--------------------------------------------------
//
//    Set Source to Film Distance
//
//--------------------------------------------------
void 
FluoroscopyCArm::SetSourceToFilmDistance( double distance )
{
  m_CArm->SetRadius( distance / 2.0 );
}




//--------------------------------------------------
//
//    Set Detector Radius
//
//--------------------------------------------------
void 
FluoroscopyCArm::SetDetectorRadius( double radius )
{
  m_Detector->SetRadius( radius );
}





//--------------------------------------------------
//
//    Draw the Geometry of the Shape
//
//--------------------------------------------------
void 
FluoroscopyCArm::DrawGeometry(void) const 
{

  glRotated( m_RaoLaoAngle, 0.0, 0.0, 1.0 );
  m_TurningBase->glDraw();

  glTranslated( 0.0, 0.0, -m_CArm->GetRadius() );
  glRotated( m_CranioCaudalAngle, 1.0, 0.0, 0.0 );
  m_CArm->glDraw();

  if( m_DetectorConfiguration == DetectorDown )
  {
    glRotated( 180, 1.0, 0.0, 0.0 );
  }
  
  
  DrawDetector();
  DrawSource();

}


//--------------------------------------------------
//
//    Draw the Detector (Intensifier)
//
//--------------------------------------------------
void 
FluoroscopyCArm::DrawDetector(void) const 
{


  glPushMatrix();
    const GLdouble posIntensifier = 
                      - m_CArm->GetRadius() 
                      - m_CArm->GetRadialWidth() 
                      + m_IntensifierOffset; 

    glTranslated( 0.0, posIntensifier, 0.0 );
    glRotated( -90.0, 1.0, 0.0, 0.0 );
    
    m_Intensifier->glDraw();
    glTranslated( 0.0, 0.0, m_Intensifier->GetHeight() );
    
    m_DetectorSupport->glDraw();
    glTranslated( 0.0, 0.0, m_DetectorSupport->GetHeight() );

    m_Detector->glDraw();
    
    if( GetAutoSensing() ) 
    {
      VectorType vertical;
      vertical[0] =  1;
      vertical[1] =  0;
      vertical[2] =  0;
      m_DetectorVerticalDirection = 
        m_Detector->ComputeCurrentOrientation( vertical );
    }
  glPopMatrix();

}


//--------------------------------------------------
//
//    Draw the Source 
//
//--------------------------------------------------
void 
FluoroscopyCArm::DrawSource(void) const 
{

  glPushMatrix();
    const GLdouble posSource  =  
                      m_CArm->GetRadius() + m_CArm->GetRadialWidth();

    glTranslated( 0.0, posSource, 0.0 );
    glRotated( 90.0, 1.0, 0.0, 0.0 );

    m_SourceSupport->glDraw();
    glTranslated( 0.0, 0.0, m_SourceSupport->GetHeight() );

    m_Source->glDraw();

    if( m_FovIsVisible ) 
    {
      GLdouble distance = m_Source->GetCurrentPosition().EuclideanDistanceTo(
                          m_Detector->GetCurrentPosition() );

      m_FovCone->SetHeight( distance );
      glRotated( 180.0, 1.0, 0.0, 0.0 );
      glTranslated( 0.0, 0.0, -m_FovCone->GetHeight() );

      m_FovCone->glDraw();
    }
  glPopMatrix();

}


  
} // end namespace li




