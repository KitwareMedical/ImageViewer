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


#include <FL/gl.h>
#include <FL/fl_ask.H>
#include <liVertebralPedicle.h>
#include <fstream>

namespace li {


//----------------------------------------------------
//
//    Creator
//
//----------------------------------------------------
VertebralPedicle::VertebralPedicle() 
{
  
  m_VerticalPosition      =  10.0;
  m_HorizontalPosition    =  10.0;
  m_FrontalPosition       =   0.0;
  m_VerticalAngle         = -15.0;
  m_HorizontalAngle       =   0.0;
  m_VerticalRadius        =   5.0;
  m_HorizontalRadius      =   5.0;
  m_Length                =  10.0;
  m_NumberOfSectors       =   6;

  m_LeftTube   = Cylinder3D::New();
  m_RightTube  = Cylinder3D::New();

  m_LeftTube->SetNumberOfSectors( m_NumberOfSectors );
  m_LeftTube->SetNumberOfSectors( m_NumberOfSectors );

  AddComponent( m_LeftTube.GetPointer() );
  AddComponent( m_RightTube.GetPointer() );

}





//----------------------------------------------------
//
//    Destructor
//
//----------------------------------------------------
VertebralPedicle::~VertebralPedicle() 
{


}




//----------------------------------------------------
//
//    Set Number of Sectors
//
//----------------------------------------------------
void VertebralPedicle::SetNumberOfSectors(unsigned int number)
{
  m_NumberOfSectors = number;
  m_LeftTube->SetNumberOfSectors( m_NumberOfSectors );
  m_RightTube->SetNumberOfSectors( m_NumberOfSectors );
}





//----------------------------------------------------
//
//    Set Vertical Position
//
//----------------------------------------------------
void VertebralPedicle::SetVerticalPosition(double val) 
{
  m_VerticalPosition = val;
}



//----------------------------------------------------
//
//    Set Horizontal Position
//
//----------------------------------------------------
void VertebralPedicle::SetHorizontalPosition(double val) 
{
  m_HorizontalPosition = val;
}


//----------------------------------------------------
//
//    Set Frontal Position
//
//----------------------------------------------------
void VertebralPedicle::SetFrontalPosition(double val) 
{
  m_FrontalPosition = val;
}



//----------------------------------------------------
//
//    Set Vertical Angle
//
//----------------------------------------------------
void VertebralPedicle::SetVerticalAngle(double val) 
{
  m_VerticalAngle = val;
}



//----------------------------------------------------
//
//    Set Horizontal Angle
//
//----------------------------------------------------
void VertebralPedicle::SetHorizontalAngle(double val) 
{
  m_HorizontalAngle = val;
}



//----------------------------------------------------
//
//    Set Vertical Radius
//
//----------------------------------------------------
void VertebralPedicle::SetVerticalRadius(double val) 
{
  m_VerticalRadius = val;
}



//----------------------------------------------------
//
//    Set Horizontal Radius
//
//----------------------------------------------------
void VertebralPedicle::SetHorizontalRadius(double val) 
{
  m_HorizontalRadius = val;
}



//----------------------------------------------------
//
//    Set Length
//
//----------------------------------------------------
void VertebralPedicle::SetLength(double val) 
{
  m_Length = val;
}


//----------------------------------------------------
//
//    Get the number of sectors
//
//----------------------------------------------------
unsigned int VertebralPedicle::GetNumberOfSectors(void) const 
{
  return m_NumberOfSectors;
}



//----------------------------------------------------
//
//    Get Vertical Position
//
//----------------------------------------------------
double VertebralPedicle::GetVerticalPosition(void) const 
{
  return m_VerticalPosition;
}



//----------------------------------------------------
//
//    Get Horizontal Position
//
//----------------------------------------------------
double VertebralPedicle::GetHorizontalPosition(void) const 
{
  return m_HorizontalPosition;
}



//----------------------------------------------------
//
//    Get Frontal Position
//
//----------------------------------------------------
double VertebralPedicle::GetFrontalPosition(void) const 
{
  return m_FrontalPosition;
}


//----------------------------------------------------
//
//    Get Vertical Angle
//
//----------------------------------------------------
double VertebralPedicle::GetVerticalAngle(void) const 
{
  return m_VerticalAngle;
}



//----------------------------------------------------
//
//    Get Horizontal Angle
//
//----------------------------------------------------
double VertebralPedicle::GetHorizontalAngle(void) const 
{
  return m_HorizontalAngle;
}

//----------------------------------------------------
//
//    Get Vertical Radius
//
//----------------------------------------------------
double VertebralPedicle::GetVerticalRadius(void) const 
{
  return m_VerticalRadius;
}

//----------------------------------------------------
//
//    Get Horizontal Radius
//
//----------------------------------------------------
double VertebralPedicle::GetHorizontalRadius(void) const 
{
  return m_HorizontalRadius;
}



//----------------------------------------------------
//
//    Get Length
//
//----------------------------------------------------
double VertebralPedicle::GetLength(void) const 
{
  return m_Length;
}


//----------------------------------------------------
//
//    Draw Geometry in OpenGL
//
//----------------------------------------------------
void VertebralPedicle::DrawGeometry(void) const 
{
  glRotated( -90.0, 1.0, 0.0, 0.0 );
  glPushMatrix();
    glTranslated( m_HorizontalPosition, -m_VerticalPosition, m_FrontalPosition );
    glRotated( m_HorizontalAngle, 0.0, 1.0, 0.0 );
    glRotated( m_VerticalAngle, 1.0, 0.0, 0.0 );
    glScaled( m_HorizontalRadius, m_VerticalRadius, m_Length );
    m_LeftTube->glDraw();
  glPopMatrix();

  glTranslated( -m_HorizontalPosition, -m_VerticalPosition, m_FrontalPosition );
  glRotated( -m_HorizontalAngle, 0.0, 1.0, 0.0 );
  glRotated( m_VerticalAngle, 1.0, 0.0, 0.0 );
  glScaled( m_HorizontalRadius, m_VerticalRadius, m_Length);
  m_RightTube->glDraw();

}




   


//----------------------------------------------------
//
//    Write to stream
//
//----------------------------------------------------
int 
VertebralPedicle::Write( Shape3D::OfstreamType &os, int level ) const 
{

  int stat = 1;

  char *indent = new char[level+1];
  for(int i=0; i<level; i++) indent[i]='\t';
  indent[level]='\0';

  os << indent << "VertebralPedicles { "  << std::endl;
  os << indent << '\t' << "VerticalPosition  " << m_VerticalPosition << std::endl;
  os << indent << '\t' << "HorizontalPosition  " << m_HorizontalPosition << std::endl;
  os << indent << '\t' << "FrontalPosition  " << m_FrontalPosition << std::endl;
  os << indent << '\t' << "VerticalAngle  " << m_VerticalAngle << std::endl;
  os << indent << '\t' << "HorizontalAngle  " << m_HorizontalAngle << std::endl;
  os << indent << '\t' << "HorizontalRadius " << m_HorizontalRadius << std::endl;
  os << indent << '\t' << "VerticalRadius " << m_VerticalRadius << std::endl;
  os << indent << '\t' << "Length " << m_Length << std::endl;
  os << indent << '\t' << "Sectors " << m_NumberOfSectors << std::endl;
  os << indent << '\t' << "DrawingMode  " << GetDrawingMode() << std::endl;
  os << indent << '\t' << "Color  " << GetColor()  << std::endl;

  os << indent << '\t' << "}" << std::endl;

  delete [] indent;

  if( os.fail() ) {
    fl_alert("Error encountered writing Vertebral Body");
    return 0;
    }


  return stat;

}





//----------------------------------------------------
//
//    Read from stream
//
//----------------------------------------------------
int 
VertebralPedicle::Read( Shape3D::IfstreamType &is ) 
{

  char field[200];

  int endOfGroup=0;

  is >> field;
  if( strcmp(field,"{")!=0 ) { 
    fl_alert("Problem reading Vertebral Body, missing '{'");
    return 0;
    }

  while( !endOfGroup ) {

    is >> field;

    if( is.eof() ) {
      fl_alert("Unexpected end of file reading Vertebral Body");
      return 0;
      }
      
    if( is.fail() ) {
      fl_alert("Error found reading Vertebral Body");
      return 0;
      }

    if( strcmp(field,"VerticalPosition")==0 ) { is >> m_VerticalPosition; continue; }
    if( strcmp(field,"HorizontalPosition")==0 ) { is >> m_HorizontalPosition; continue; }
    if( strcmp(field,"FrontalPosition")==0 ) { is >> m_FrontalPosition; continue; }
    if( strcmp(field,"VerticalAngle")==0 ) { is >> m_VerticalAngle; continue; }
    if( strcmp(field,"HorizontalAngle")==0 ) { is >> m_HorizontalAngle; continue; }
    if( strcmp(field,"VerticalRadius")==0 ) { is >> m_VerticalRadius; continue; }
    if( strcmp(field,"HorizontalRadius")==0 ) { is >> m_HorizontalRadius; continue; }
    if( strcmp(field,"Length")==0 ) { is >> m_Length; continue; }
    if( strcmp(field,"Sectors")==0 ) { is >> m_NumberOfSectors;
                        SetNumberOfSectors(m_NumberOfSectors); continue; }
    
    if( strcmp(field,"DrawingMode")==0 ) {     long mode; is >> mode; 
                 SetDrawingMode( static_cast<drawingModes>(mode) ); continue; }

    if( strcmp(field,"Color")==0 ) { ColorType color; 
                         is >> color; SetColor( color ); continue; }

    if( strcmp(field,"}")==0 ) { endOfGroup=1; }
    }

  return 1;
}




}   // end namespace li



