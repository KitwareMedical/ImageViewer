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


#include <liVertebralSegment.h>
#include <liSampleGeneratorPedicle.h>
#include <liSampleGeneratorBodyPlane.h>
#include <fstream>


namespace li {


//----------------------------------------------------
//
//    Creator
//
//----------------------------------------------------
VertebralSegment::VertebralSegment() 
{

  m_Shift       = 30.0;
  m_Rotation    = 0.0;
  m_Name        = 0;
  m_Body        = VertebralBody::New();
  m_Pedicles    = VertebralPedicle::New();

  AddComponent( m_Body.GetPointer() );
  AddComponent( m_Pedicles.GetPointer() );

  m_LeftPediclePointSet     = PointSet3D::New();
  m_RightPediclePointSet    = PointSet3D::New();
  m_TopBodyPointSet         = PointSet3D::New();
  m_BottomBodyPointSet      = PointSet3D::New();

  AddComponent( m_LeftPediclePointSet.GetPointer()  );
  AddComponent( m_RightPediclePointSet.GetPointer() );
  AddComponent( m_TopBodyPointSet.GetPointer()      );
  AddComponent( m_BottomBodyPointSet.GetPointer()   );

  m_AxisVisible    = true;
  m_SamplesVisible = true;

}




//----------------------------------------------------
//
//    Destructor
//
//----------------------------------------------------

VertebralSegment::~VertebralSegment() 
{
  if( m_Name ) 
  { 
    delete [] m_Name; 
    m_Name=0; 
  }
    
}



//----------------------------------------------------
//
//    Draw the model using OpenGL commands
//
//----------------------------------------------------
void 
VertebralSegment::DrawGeometry(void) const 
{

  if( m_AxisVisible ) 
  {
    DrawAxis();
  }

  if( m_SamplesVisible ) 
  {
    m_LeftPediclePointSet->glDraw();
    m_RightPediclePointSet->glDraw();
    m_TopBodyPointSet->glDraw();
    m_BottomBodyPointSet->glDraw();
  }

  glPushMatrix();
    m_Body->glDraw();
  glPopMatrix();

  glPushMatrix();
    m_Pedicles->glDraw();
  glPopMatrix();

  glTranslated( 0.0, 0.0, m_Shift );
  glRotated( m_Rotation, 1.0, 0.0, 0.0 );

}




//----------------------------------------------------
//
//    Get Vertebra Name
//
//----------------------------------------------------
const char * 
VertebralSegment::GetName( void ) 
{
  return m_Name;
}



//----------------------------------------------------
//
//     Set Name
//
//----------------------------------------------------
void 
VertebralSegment::SetName(const char *newName) 
{

  if( m_Name ) 
  { 
    delete [] m_Name; 
  }

  m_Name = new char[ strlen( newName ) + 1 ];

  strcpy( m_Name, newName );

}



//----------------------------------------------------
//
//    Get Shift
//
//----------------------------------------------------
double 
VertebralSegment::GetShift( void ) 
{
  return m_Shift;
}



//----------------------------------------------------
//
//    Set Shift
//
//----------------------------------------------------
void 
VertebralSegment::SetShift( double val ) 
{
  m_Shift = val;
}



//----------------------------------------------------
//
//    Get Rotation
//
//----------------------------------------------------
double 
VertebralSegment::GetRotation( void ) 
{
  return m_Rotation;
}



//----------------------------------------------------
//
//    Set Rotation
//
//----------------------------------------------------
void 
VertebralSegment::SetRotation( double val )
{
  m_Rotation = val;
}



//----------------------------------------------------
//
//    Get Vertebra Body
//
//----------------------------------------------------
VertebralBody::Pointer 
VertebralSegment::GetVertebralBody( void ) 
{
  return m_Body;
}



//----------------------------------------------------
//
//    Get Vertebra Pedicle
//
//----------------------------------------------------
VertebralPedicle::Pointer 
VertebralSegment::GetVertebralPedicle( void ) 
{
  return m_Pedicles;
}





//----------------------------------------------------
//
//    Write to stream
//
//----------------------------------------------------
int 
VertebralSegment::Write( std::ofstream &os, int level ) const 
{

  int stat = 1;

  char *indent = new char[level+1];
  for(int i=0; i<level; i++) indent[i]='\t';
  indent[level]='\0';

  os << indent << "VertebralSegment { "  << std::endl;
  if( m_Name ) {
    os << indent << '\t' << "Name " << m_Name << std::endl;
    }

  os << indent << '\t' << "Shift " << m_Shift << std::endl;
  os << indent << '\t' << "Rotation " << m_Rotation << std::endl;

  m_Body->Write(os,level+1);
  m_Pedicles->Write(os,level+1);

  os << indent << '\t' << "}" << std::endl;

  delete [] indent;

  if( os.fail() ) {
    std::cerr << "Error encountered writing VertebralSegment";
    std::cerr << std::endl;
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
VertebralSegment::Read( Shape3D::IfstreamType &is ) 
{

  char field[200];

  int endOfGroup=0;

  is >> field;
  if( strcmp(field,"{")!=0 ) { 
    std::cerr << "Problem reading VertebralSegment, missing '{'";
    std::cerr << std::endl;
    return 0;
    }

  while( !endOfGroup ) {

    is >> field;

    if( is.eof() ) {
      std::cerr << "Unexpected end of file reading VertebralSegment";
      std::cerr << std::endl;
      return 0;
      }
      
    if( is.fail() ) {
      std::cerr << "Error found reading VertebralSegment";
      std::cerr << std::endl;
      return 0;
      }

    if( strcmp(field,"Name")==0 ) {
      char temp[200];
      is >> temp;
      SetName( temp );
      continue;
      }

    if( strcmp(field,"Shift")==0 ) { is >> m_Shift; continue; }
    if( strcmp(field,"Rotation")==0 ) { is >> m_Rotation; continue; }

    if( strcmp(field,"VertebralBody")==0 ) { m_Body->Read(is); continue; }
    if( strcmp(field,"VertebralPedicles")==0 ) { m_Pedicles->Read(is); continue; }

    if( strcmp(field,"}")==0 ) { endOfGroup=1; }
    }

  return 1;

}





//----------------------------------------------------
//
//    Draw Axis
//
//----------------------------------------------------
void VertebralSegment::DrawAxis(void ) const
{

  glDisable(GL_LIGHTING);

 	// Draw axes
	const GLfloat axisLength = 50;
	glBegin(GL_LINES);
		glColor3f((GLfloat)1.0,(GLfloat)0.0,(GLfloat)0.0);
		glVertex3f((GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0);
		glVertex3f(axisLength,(GLfloat)0.0,(GLfloat)0.0);
		
		glColor3f((GLfloat)0.0,(GLfloat)1.0,(GLfloat)0.0);
		glVertex3f((GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0);
		glVertex3f((GLfloat)0.0,axisLength,(GLfloat)0.0);
		
		glColor3f((GLfloat)0.0,(GLfloat)0.0,(GLfloat)1.0);
		glVertex3f((GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0);
		glVertex3f((GLfloat)0.0,(GLfloat)0.0,axisLength);
	glEnd();
}




//----------------------------------------------------
//
//    Set Axis Visibility
//
//----------------------------------------------------
void
VertebralSegment::SetAxisVisible( bool visible ) 
{
  m_AxisVisible = visible;
  InvokeEvent( li::PatientMovedEvent );
}


//----------------------------------------------------
//
//    Set Samples Visibility
//
//----------------------------------------------------
void
VertebralSegment::SetSamplesVisible( bool visible ) 
{
  m_SamplesVisible = visible;
  InvokeEvent( li::PatientMovedEvent );
}


} // end namespace li



