//--------------------------------------------
//
//     Project Angio Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------



#include <liVectorTubes3D.h>
#include <itkExceptionObject.h>

namespace li {

//--------------------------------------------------
//
//    Creator
//
//--------------------------------------------------
VectorTubes3D::VectorTubes3D() {

  Shape3D::ColorType color;
  color.SetRed( 0.8 );
  color.SetGreen( 0.8 );
  color.SetBlue( 1.0 );

  m_Boundary = Box3D::New();
  m_Boundary->SetDrawingMode( Shape3D::lines );
  m_Boundary->SetColor( color );

  nx = 0;
  ny = 0;
  nz = 0;

  dx = 1.0;
  dy = 1.0;
  dz = 1.0;
  
  rz = 0.0;    

  tx = 0.0;
  ty = 0.0;
  tz = 0.0;

  ex = 0.0;
  ey = 0.0;
  ez = 0.0;

  m_PointSet3D = PointSet3D::New();

  m_DisplayPointSet = false;

  this->SetAutoSensing( true );

  }


//--------------------------------------------------
//
//    Destructor      
//
//--------------------------------------------------
VectorTubes3D::~VectorTubes3D() 
{
  Clear();
}

  

//--------------------------------------------------
//
//    Free tubes memory      
//
//--------------------------------------------------
void VectorTubes3D::Clear(void) 
{

  m_Tubes.clear();


}




//--------------------------------------------------
//
//       set Rotation around Z axis
//
//--------------------------------------------------
void VectorTubes3D::SetRotation(GLdouble irz) 
{
  rz = irz;
}






//--------------------------------------------------
//
//       get Rotation around Z axis
//
//--------------------------------------------------
GLdouble VectorTubes3D::GetRotation(void) const 
{
  return rz;
}




//--------------------------------------------------
//
//       set Offset
//
//--------------------------------------------------
void VectorTubes3D::SetOffset(const VectorType & newoffset) {
  tx = newoffset[0];
  ty = newoffset[1];
  tz = newoffset[2];
  }




//--------------------------------------------------
//
//       set External Offset
//
//--------------------------------------------------
void VectorTubes3D::SetExternalOffset(const VectorType & newoffset) {
  ex = newoffset[0];
  ey = newoffset[1];
  ez = newoffset[2];
  }





//--------------------------------------------------
//
//       get Offset
//
//--------------------------------------------------
VectorTubes3D::VectorType 
VectorTubes3D::GetOffset(void)  const 
{
  VectorType v;
  v = tx,ty,tz;
  return v;
}





//--------------------------------------------------
//
//       get External Offset
//
//--------------------------------------------------
VectorTubes3D::VectorType 
VectorTubes3D::GetExternalOffset(void)  const 
{
  VectorType v;
  v = ex,ey,ez;
  return v;
}



  
  
//--------------------------------------------------
//
//       Enable drawing m_Boundary
//
//--------------------------------------------------
void VectorTubes3D::EnableBoundary(void) 
{
  m_Boundary->SetDrawingMode(Shape3D::lines);
}


  


//--------------------------------------------------
//
//       Disable drawing m_Boundary
//
//--------------------------------------------------
void VectorTubes3D::DisableBoundary(void) 
{
  m_Boundary->SetDrawingMode(Shape3D::none);
}





//--------------------------------------------------
//
//      Draws tubes in OpenGL
//
//--------------------------------------------------
void VectorTubes3D::DrawGeometry(void) const 
{

  glPushMatrix();

  glTranslated( ex, ey, ez );

  m_Boundary->glDraw();

  glTranslated( -m_Boundary->GetLength() / 2.0,
                -m_Boundary->GetWidth() / 2.0,
                 0.0 );

  glTranslated( tx, ty, tz );

  if( m_Tubes.empty() ) 
  {
    return;
  }

  VectorTubeType::const_iterator tube = m_Tubes.begin();
  while( tube != m_Tubes.end() )
  {
    (*tube)->glDraw();
    tube++;
  } 

  if( m_DisplayPointSet )
  {
    glLoadIdentity();
    m_PointSet3D->glDraw();
  }

  glPopMatrix();

}



//--------------------------------------------------
//
//      Load tubes from a file
//
//--------------------------------------------------
void VectorTubes3D::Load( const char * filename )
{

  Shape3D::IfstreamType ifs;
  ifs.open( filename );
  if( ifs.fail() ) 
  {
    throw itk::ExceptionObject();
  }

  char field[200];
  ifs >> field;

  if( strcmp(field,"VectorTubes3D") != 0 ) 
  {
    std::cerr << "The file " << filename << std::endl;
    std::cerr << "don't seems to contain vessels" << std::endl;
  }
  else
  {
    Read( ifs );
  }

  ifs.close();
  

}



//--------------------------------------------------
//
//      Read tubes from an Ifstream
//
//--------------------------------------------------
void VectorTubes3D::Read(Shape3D::IfstreamType & is)
{

  Clear();

  char field[200];

  int endOfGroup=0;

  is >> field;
  if( strcmp(field,"{") != 0 ) 
  { 
    std::cerr << "Problem reading VectorTubes3D, missing '{'";
    std::cerr << std::endl;
    throw itk::ExceptionObject();
  }

  while( !endOfGroup ) 
  {

    is >> field;

    if( is.eof() ) 
    {
      std::cerr << "Unexpected end of file reading VectorTubes3D";
      std::cerr << std::endl;
      throw itk::ExceptionObject();
    }
      
    if( is.fail() ) 
    {
      std::cerr << "Error found reading VectorTubes3D";
      std::cerr << std::endl;
      throw itk::ExceptionObject();
    }

    if( strcmp(field,"Dimensions"    )==0 ) { is >> nx >> ny >> nz; continue; }
    if( strcmp(field,"VoxelSize"     )==0 ) { is >> dx >> dy >> dz; continue; }
    if( strcmp(field,"Offset"        )==0 ) { is >> tx >> ty >> tz; continue; }
    if( strcmp(field,"ExternalOffset")==0 ) { is >> ex >> ey >> ez; continue; }
    if( strcmp(field,"Angle"         )==0 ) { is >> rz;             continue; }

    
    if( strcmp(field,"Tube3D")==0 ) 
    { 
      Tube3D::Pointer temp = Tube3D::New();
      temp->Read( is );
      temp->ScalePoints( dx, dy, dz );
      AddComponent( temp );
      m_Tubes.push_back( temp );
      continue;
    }

    if( strcmp(field,"}")==0 ) 
    { 
      endOfGroup=1; 
    }

  }

  m_Boundary->SetLength( nx*dx );
  m_Boundary->SetWidth(  ny*dy );
  m_Boundary->SetHeight( nz*dz );

  // the first time needs two redraws
  InvokeEvent( li::PatientMovedEvent );
  InvokeEvent( li::PatientMovedEvent ); 


}




  
//--------------------------------------------------
//
//      Save tubes to file
//
//--------------------------------------------------
int VectorTubes3D::Save(const char * filename) const 
{

  return 1;

}



//--------------------------------------------------
//
//      Generate Point Set
//
//--------------------------------------------------
void VectorTubes3D::GeneratePointSet( const VectorType & sight )
{

  VectorTubeType::iterator tube = m_Tubes.begin();
  while( tube != m_Tubes.end() )
  {
    (*tube)->GeneratePointSet( sight );
    tube++;
  } 

  tube = m_Tubes.begin();
  unsigned long numberOfPoints = 0;
  while( tube != m_Tubes.end() )
  {
    numberOfPoints += (*tube)->GetPointSet()->GetNumberOfPoints();
    tube++;
  } 

  typedef  PointSetType::PointsContainer     PointsContainer;
  typedef  PointSetType::PointDataContainer  PointDataContainer;

  PointsContainer::Pointer     points = PointsContainer::New();
  PointDataContainer::Pointer  values = PointDataContainer::New();

  points->Reserve( numberOfPoints );
  values->Reserve( numberOfPoints );

  PointsContainer::Iterator    ttp = points->Begin();
  PointDataContainer::Iterator ttv = values->Begin();

  tube = m_Tubes.begin();
  while( tube != m_Tubes.end() )
  {

    PointsContainer::ConstPointer tubePoints =
                                 (*tube)->GetPointSet()->GetPoints();

    PointsContainer::ConstIterator tp = tubePoints->Begin();
    while( tp != tubePoints->End() )
    {
      ttp->Value() = tp->Value();
      ++tp;
      ++ttp;
    }
    
    PointDataContainer::ConstPointer tubeValues =
                                 (*tube)->GetPointSet()->GetPointData();

    PointDataContainer::ConstIterator tv = tubeValues->Begin();
    while( tv != tubeValues->End() )
    {
      ttv->Value() = tv->Value();
      ++tv;
      ++ttv;
    }
 
    tube++;

  } 



  
  PointSetType::Pointer pointSet = PointSetType::New();

  pointSet->SetPoints( points );
  pointSet->SetPointData( values );

  m_PointSet3D->SetPointSet( pointSet );

}



} // end namespace li


