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


#include <liSpineModel.h>
#include <fstream>




namespace li {



//-------------------------------------------
//
//    Creator
//
//-------------------------------------------
SpineModel::SpineModel() 
{
  m_Position.Fill( 0.0 );

	m_RaoLaoAngle       =  0.0;
  m_CranioCaudalAngle = 25.0;
	m_AxialAngle        =  0.0;

}



//-------------------------------------------
//
//    Destructor
//
//-------------------------------------------
SpineModel::~SpineModel() 
{

}



//--------------------------------------------------
//
//    Get the Rao/Lao angle
//
//--------------------------------------------------
double 
SpineModel::GetRaoLaoAngle( void ) const
{
  return m_RaoLaoAngle;
}




//--------------------------------------------------
//
//    Get the Cranio/Caudal angle
//
//--------------------------------------------------
double 
SpineModel::GetCranioCaudalAngle( void ) const
{
  return m_CranioCaudalAngle;
}


//--------------------------------------------------
//
//    Get the Axial angle
//
//--------------------------------------------------
double
SpineModel::GetAxialAngle( void ) const
{
  return m_AxialAngle;
}





//--------------------------------------------------
//
//    Get Position
//
//--------------------------------------------------
const Shape3D::PointType & 
SpineModel::GetPosition( void ) const
{
  return m_Position;
}



//--------------------------------------------------
//
//    Get Begin iterator to Vertebral Segments
//
//--------------------------------------------------
SpineModel::VertebraContainer::iterator 
SpineModel::GetVertebralSegmentsBegin( void )
{
  return m_Segments.begin();
}




//--------------------------------------------------
//
//    Get End iterator to Vertebral Segments
//
//--------------------------------------------------
SpineModel::VertebraContainer::iterator 
SpineModel::GetVertebralSegmentsEnd( void )
{
  return m_Segments.end();
}







//--------------------------------------------------
//
//    Set the Rao/Lao angle
//
//--------------------------------------------------
void 
SpineModel::SetRaoLaoAngle( double angle )
{
  m_RaoLaoAngle = angle;
}




//--------------------------------------------------
//
//    Set the Cranio/Caudal angle
//
//--------------------------------------------------
void 
SpineModel::SetCranioCaudalAngle( double angle )
{
  m_CranioCaudalAngle = angle;
}


//--------------------------------------------------
//
//    Set the Axial angle
//
//--------------------------------------------------
void 
SpineModel::SetAxialAngle( double angle )
{
  m_AxialAngle = angle;
}





//--------------------------------------------------
//
//    Set Position
//
//--------------------------------------------------
void 
SpineModel::SetPosition( const PointType & pnt )
{
  m_Position = pnt;
}




//-------------------------------------------
//
//  Draw Spine Representation with OpenGL
//
//-------------------------------------------
void 
SpineModel::DrawGeometry(void) const
{

  if( m_Segments.empty() ) 
  {
    return;
  }

  glTranslated( 0.0, 0.0, -600.0 );  
  glTranslated( m_Position[0], m_Position[1], m_Position[2] );
  glRotated( m_RaoLaoAngle, 0.0, 1.0, 0.0 );
  glRotated( m_CranioCaudalAngle, 1.0, 0.0, 0.0 );
  glRotated( m_AxialAngle, 0.0, 0.0, 1.0 );

  VertebraContainer::const_iterator it = m_Segments.begin();
  while( it != m_Segments.end() )
  {
    (*it)->glDraw();
    ++it;
  }

}




   
//-------------------------------------------
//
//    Save Spine data to file
//
//-------------------------------------------
void 
SpineModel::Save(const char *filename) 
{

  if( !filename ) 
  {
    return;
  }


  std::ofstream out2;
  Shape3D::OfstreamType outFile;
  outFile.open(filename);
  
  
  if( outFile.fail() ) 
  {
    std::cerr << "SpineModel: Problem opening file ";
    std::cerr << filename << std::endl;
    outFile.close();
    return;
  }

  Write( outFile, 0 );

  outFile.close();
  

}




//-------------------------------------------
//
//    Load Spine data from file
//
//-------------------------------------------
void 
SpineModel::Load( const char *filename ) 
{

  m_Segments.clear();

  if( !filename ) 
  {
    return;
  }

  
  Shape3D::IfstreamType inpFile;
  
  inpFile.open( filename );

  if( inpFile.fail() ) 
  {
    std::cerr << "Problem opening file ";
    std::cerr <<  filename << std::endl;
    return;
  }
  char field[200];
  inpFile >> field;
  if( strcmp(field,"SpineModel") != 0 ) 
  {
    std::cerr <<  "SpineModel keyword don't found in file ";
    std::cerr << filename << std::endl;
    inpFile.close();
    return;
    }

  Read( inpFile );

  inpFile.close();
  

}




//-------------------------------------------
//
//    Verify if data is loaded
//
//-------------------------------------------
bool 
SpineModel::Null(void) const 
{
  return m_Segments.empty();
}





//----------------------------------------------------
//
//    Write to stream
//
//----------------------------------------------------

int 
SpineModel::Write( Shape3D::OfstreamType &os, int level ) const 
{

  int stat = 1;

  char *indent = new char[level+1];
  for(int i=0; i<level; i++) indent[i]='\t';
  indent[level]='\0';

  os << indent << "SpineModel { "  << std::endl;

  os << indent << '\t' << "CranioCaudalAngle " << m_CranioCaudalAngle << std::endl;
  os << indent << '\t' << "RaoLaoAngle       " << m_RaoLaoAngle       << std::endl;
  os << indent << '\t' << "AxialAngle        " << m_AxialAngle        << std::endl;
  os << indent << '\t' << "Position          " << m_Position          << std::endl;

  VertebraContainer::const_iterator it = m_Segments.begin();
  while( it != m_Segments.end() )
  {
    (*it)->Write( os, level+1 );
    ++it;
  }

  os << indent << '\t' << "}" << std::endl;

  delete [] indent;

  if( os.fail() ) 
  {
    std::cerr << "Error encountered writing SpineModel";
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
SpineModel::Read( Shape3D::IfstreamType &is )
{

  char field[200];

  int endOfGroup=0;

  is >> field;
  if( strcmp(field,"{") != 0 ) 
  { 
    std::cerr << "Problem reading SpineModel, missing '{'";
    std::cerr << std::endl;
    return 0;
  }

  while( !endOfGroup ) 
  {

    is >> field;

    if( is.eof() ) 
    {
      std::cerr << "Unexpected end of file reading SpineModel";
      std::cerr << std::endl;
      return 0;
    }
      
    if( is.fail() ) 
    {
      std::cerr << "Error found reading SpineModel";
      std::cerr << std::endl;
      return 0;
    }

    if( strcmp(field,"RaoLaoAngle"      )==0 ) { is >> m_RaoLaoAngle; continue; }
    if( strcmp(field,"CranioCaudalAngle")==0 ) { is >> m_CranioCaudalAngle; continue; }
    if( strcmp(field,"AxialAngle"       )==0 ) { is >> m_AxialAngle; continue; }
    if( strcmp(field,"Position"         )==0 ) { is >> m_Position; continue; }
    
    if( strcmp(field,"VertebralSegment")==0 ) 
    { 
      VertebralSegment::Pointer temp = VertebralSegment::New();
      temp->Read( is );
      AddComponent( temp );
      m_Segments.push_back( temp );
      continue;
    }

    if( strcmp(field,"}")==0 ) 
    { 
      endOfGroup=1; 
    }

  }

  InvokeEvent( li::PatientMovedEvent );
  // the first time needs two redraws
  InvokeEvent( li::PatientMovedEvent ); 
  InvokeEvent( li::SpineChangedEvent );

  return 1;
}




}    // end namespace li



