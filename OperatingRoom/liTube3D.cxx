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



#include <liTube3D.h>
#include <FL/fl_ask.H>
#include <fstream>
#include <cstring>

namespace li {


//--------------------------------------------------
//
//    Constructor      
//
//--------------------------------------------------
Tube3D::Tube3D()
{
  m_NumberOfSectors  = 6;
  m_NumberOfSections = 0;
  m_Radius           = 0;
  m_Vertex           = 0;
  m_Medial           = 0;
  m_PointSet         = PointSetType::New();
}



//--------------------------------------------------
//
//    Memory allocation
//
//--------------------------------------------------
bool Tube3D::Alloc( unsigned int numberOfSectors,
                    unsigned int numberOfSections ) 
{

  Clear();

  m_NumberOfSectors  = numberOfSectors;
  m_NumberOfSections = numberOfSections;

  m_Radius = 0;
  m_Vertex = 0;
  m_Medial = 0;

  m_Radius = new GLfloat[m_NumberOfSections];
  if(!m_Radius) 
  {
    Clear();
    return false;
  }
  
  for(unsigned int i=0; i<m_NumberOfSections; i++) 
  {
    m_Radius[i] = 1.0;
  }

  double interleave = 2.0/(m_NumberOfSections-1);

  m_Medial = new PointType[m_NumberOfSections];
  if(!m_Medial) 
  {
    Clear();
    return false;
  }

  PointType pp;
  pp = 0.0, 0.0,-interleave*((double)m_NumberOfSections)/2.0;

  for(unsigned int j=0; j<m_NumberOfSections; j++) 
  {
    m_Medial[j] = pp;
    pp[2] += interleave;
  }

  GenerateSkin();

  return true;
}





//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
Tube3D::~Tube3D() 
{
  Clear();
}





//--------------------------------------------------
//
//    Free memory  
//
//--------------------------------------------------
void Tube3D::Clear(void) 
{

  if( m_Medial  ) { delete [] m_Medial;  m_Medial  = 0; }
  if( m_Radius  ) { delete [] m_Radius;  m_Radius  = 0; }
  if( m_Vertex  ) { delete [] m_Vertex;  m_Vertex  = 0; }

  m_NumberOfSectors  = 0;
  m_NumberOfSections  = 0;

}




//--------------------------------------------------
//
//    Return whether memory is allocated or not
//
//--------------------------------------------------
bool Tube3D::Null(void) const 
{
  return (m_Medial == 0 || m_Vertex == 0);
}







//--------------------------------------------------
//
//    Copy tube content
//
//--------------------------------------------------
void Tube3D::Copy(const Tube3D & sm ) 
{

  Alloc(sm.m_NumberOfSectors, sm.m_NumberOfSections);

  if( Null() ) 
  {
    return;
  }

  for(unsigned int i=0; i<m_NumberOfSections; i++) 
  {
    m_Radius[i] = sm.m_Radius[i];
    m_Medial[i] = sm.m_Medial[i];
  }

  GenerateSkin();

}





   
//--------------------------------------------------
//
//    Set a constant m_Radius along the tube  
//
//--------------------------------------------------
void Tube3D::SetRadius(GLfloat rad) {

  if( m_Radius ) 
    {
    for(unsigned int i=0; i<m_NumberOfSections; i++) 
      {
      m_Radius[i] = rad;
      }
    }

  GenerateSkin();

  }




//--------------------------------------------------
//
//    Draw the tube in OpenGL  
//
//--------------------------------------------------
void Tube3D::DrawGeometry(void) const 
{


  if( Null() ) 
  {
    return;
  }

  if( GetDrawingMode() == none ) 
  {
    return;
  }


  if( GetDrawingMode() == points ) 
  {
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    for(unsigned int i=0; i<m_NumberOfSections; i++) 
    {
      PointType pp = m_Medial[i];
      glVertex3d( pp[0], pp[1], pp[2] );
    }
    glEnd();
    glEnable(GL_LIGHTING);
    return;
  } 

  if( GetDrawingMode() == surfacepoints ) 
  {

    unsigned int c=0;

    glDisable(GL_LIGHTING);
    for(unsigned int i=0; i<m_NumberOfSections; i++) 
    {

      glBegin(GL_POINTS);
      for(unsigned int s=0; s<m_NumberOfSectors; s++,c++) 
      {
        glVertex3d( m_Vertex[c][0], m_Vertex[c][1], m_Vertex[c][2] );
      }
    glEnd();
    }
    glEnable(GL_LIGHTING);

    return;
  }



  if( GetDrawingMode() == lines ) 
  {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);
    for(unsigned int i=0; i<m_NumberOfSections; i++) 
    {
      PointType pp = m_Medial[i];
      glVertex3d( pp[0], pp[1], pp[2] );
    }
    glEnd();
    glEnable(GL_LIGHTING);
    return;
  }


  if( GetDrawingMode() == triangles ) 
  {

    int aux1 = 0;
    int aux2 = aux1 + m_NumberOfSectors;

    for(unsigned int i=0; i<m_NumberOfSections-1; i++) 
    {
      PointType med1 = m_Medial[i];
      PointType med2 = m_Medial[i+1];

      glBegin(GL_QUAD_STRIP);
        int last1 = aux1+m_NumberOfSectors-1;
        int last2 = last1+m_NumberOfSectors;
        VectorType n1 = m_Vertex[last1]-med1;
        VectorType n2 = m_Vertex[last2]-med2;
        glNormal3d( n1[0], n1[1], n1[2] );
        glVertex3d( m_Vertex[last1][0], m_Vertex[last1][1], m_Vertex[last1][2] );
        glNormal3d( n2[0], n2[1], n2[2] );
        glVertex3d( m_Vertex[last2][0], m_Vertex[last2][1], m_Vertex[last2][2] );
        for(unsigned int s=0; s<m_NumberOfSectors; s++) 
        {
          n1 = m_Vertex[aux1]-med1;
          n2 = m_Vertex[aux2]-med2;
          glNormal3d( n1[0], n1[1], n1[2] );
          glVertex3d( m_Vertex[aux1][0], m_Vertex[aux1][1], m_Vertex[aux1][2] );
          glNormal3d( n2[0], n2[1], n2[2] );
          glVertex3d( m_Vertex[aux2][0], m_Vertex[aux2][1], m_Vertex[aux2][2] );
          aux1++;
          aux2++;
        }
    glEnd();
    }
    return;
  }

}






//--------------------------------------------------
//
//    Read from a file
//
//--------------------------------------------------
int Tube3D::Read( ifstream & is )
{

  Clear();

  m_NumberOfSectors = 6; // default number of sectors

  Shape3D::ColorType color;
  float opacity;

  char field[200];

  int endOfGroup=0;

  is >> field;
  if( strcmp(field,"{") != 0 ) 
  { 
    std::cerr << "Problem reading Tube3D, missing '{'";
    std::cerr << std::endl;
    return 0;
  }

  while( !endOfGroup ) 
  {

    is >> field;

    if( is.eof() ) 
    {
      std::cerr << "Unexpected end of file reading Tube3D";
      std::cerr << std::endl;
      return 0;
    }
      
    if( is.fail() ) 
    {
      std::cerr << "Error found reading Tube3D";
      std::cerr << std::endl;
      return 0;
    }

    if( strcmp(field,"NumberOfSectors"      )==0 ) { is >> m_NumberOfSectors; continue; }
    if( strcmp(field,"NumberOfSections"     )==0 ) { is >> m_NumberOfSections; continue; }
    if( strcmp(field,"NumberOfPoints"       )==0 ) { is >> m_NumberOfSections; continue; }
    if( strcmp(field,"Color"                )==0 ) { is >> color; SetColor( color ); 
                                                     is >> opacity; 
                                                     SetTransparency(1.0-opacity); continue; }
    
    if( strcmp(field,"Points")==0 ) 
    { 
      if( m_NumberOfSectors && m_NumberOfSections ) 
      {
        Alloc(m_NumberOfSectors,m_NumberOfSections);
      }
      is >> field;
      if( strcmp(field,"{") != 0 ) 
      { 
        std::cerr << "Problem reading Tube3D, missing '{' after Points keyword";
        std::cerr << std::endl;
        return 0;
      }

      for(unsigned int i=0; i<m_NumberOfSections; i++) 
      {
        is >> m_Medial[i] >> m_Radius[i];
      }

      is >> field;
      if( strcmp(field,"}") != 0 ) 
      { 
        std::cerr << "Problem reading Tube3D, missing '}' after Points list";
        std::cerr << std::endl;
        return 0;
      }

      continue;
    }


    if( strcmp(field,"}")==0 ) 
    { 
      endOfGroup=1; 
    }

  }

  SubsampleUsingRadius();
  GenerateSkin();

  return 1;

}





//--------------------------------------------------
//
//    Apply the values of volume resolution 
//    to scale the coordinates and the radius 
//    of the tube
//
//--------------------------------------------------
void Tube3D::ScalePoints(double sx, double sy, double sz) 
{

  for(unsigned int i=0; i<m_NumberOfSections; i++) 
  {
    m_Medial[i][0] *= sx; 
    m_Medial[i][1] *= sy; 
    m_Medial[i][2] *= sz; 
    m_Radius[i]    *= sx;
  }

  GenerateSkin();

}




//--------------------------------------------------
//
//    Resample the points along the tube
//    the medial axis is subsamples in such a way
//    that the distance between points is similar 
//    to the tube radius at this points.
//
//--------------------------------------------------
void Tube3D::SubsampleUsingRadius(void) 
{
  
  PointType * newMedial = new PointType[ m_NumberOfSections ];
  float     * newRadius = new float[ m_NumberOfSections ];
  

  float     currentRadius   = m_Radius[0];
  PointType currentPoint    = m_Medial[0];

  newMedial[ 0 ]            = currentPoint;
  newRadius[ 0 ]            = currentRadius;

  unsigned int count = 1;

  for(unsigned int i=1; i<m_NumberOfSections; i++)
  {
    PointType p = m_Medial[i];
    double distance = p.EuclideanDistanceTo( currentPoint );
    if( distance < currentRadius ) 
    {
      continue;
    }
    currentPoint   = p;
    currentRadius  = m_Radius[i];
    
    newMedial[ count ] = currentPoint;
    newRadius[ count ] = currentRadius;

    count++;

  }
  
  delete [] m_Medial;
  delete [] m_Radius;
  m_Medial = newMedial;
  m_Radius = newRadius;
  m_NumberOfSections = count;
  
}


//--------------------------------------------------
//
//    Smooth the points along the tube
//
//--------------------------------------------------
void Tube3D::Smooth(void) 
{
  const int lenght = 5;
  const unsigned int range  = 2 * lenght + 1;


  if( m_NumberOfSections < range ) 
  {
    return;
  }

  PointType *smoothed;
  smoothed = new PointType[ m_NumberOfSections ];
  
  for(int j=0; j<lenght; j++) 
  {
    smoothed[j] = m_Medial[j];
  }
  
  double mx = 0;
  double my = 0;
  double mz = 0;

  for(unsigned int jj=0; jj<range; jj++) 
  {
    mx += m_Medial[ jj ][0];
    my += m_Medial[ jj ][1];
    mz += m_Medial[ jj ][2];
  }

  for(unsigned int k=m_NumberOfSections-lenght; k<m_NumberOfSections; k++) 
  {
    smoothed[k] = m_Medial[k];
  }
  
  for(unsigned int i=lenght; i< m_NumberOfSections-lenght; i++)
  {
    smoothed[i][0] = mx/range;
    smoothed[i][1] = my/range;
    smoothed[i][2] = mz/range;
    mx += m_Medial[i+lenght][0] - m_Medial[i-lenght][0];
    my += m_Medial[i+lenght][1] - m_Medial[i-lenght][1];
    mz += m_Medial[i+lenght][2] - m_Medial[i-lenght][2];
  }

  for(unsigned int kk=0; kk<m_NumberOfSections; kk++) 
  {
    m_Medial[kk] = smoothed[kk];
  }

  delete [] smoothed;
}




//--------------------------------------------------
//
//    Calculate point in the periphery
//
//--------------------------------------------------
int Tube3D::GenerateSkin(void) 
{

  if( m_Medial==0 || 
      m_Radius==0 || 
      m_NumberOfSectors<3 || 
      m_NumberOfSections<3   )
  {
    return 0;
  }

  if( m_Vertex  ) 
  { 
    delete [] m_Vertex;  
    m_Vertex  = 0; 
  }

  m_Vertex = new PointType[ m_NumberOfSectors * m_NumberOfSections ];

  if( !m_Vertex ) 
  {
    return 0;
  }

  //-----------------------------------
  //  Following VRML extrude object 
  //  reference system
  //-----------------------------------


  //----------------------
  //   First Section
  //----------------------
  VectorType v1 = m_Medial[1] - m_Medial[0];
  VectorType v2 = m_Medial[2] - m_Medial[1];
  VectorType y  = m_Medial[1] - m_Medial[0];
  VectorType z  = v2^v1;
  y.Normalize();
  double znorm = z.GetNorm();
  if( znorm < 1e-16 ) {
    if( fabs(y[0]) < fabs(y[1]) ) {
      if( fabs(y[0]) < fabs(y[2]) ) {
        z = 1.0,0.0,0.0;
        }
      else {
        z = 0.0,0.0,1.0;
        }
      }
    else {
      if( fabs(y[1]) < fabs(y[2]) ) {
        z = 0.0,1.0,0.0;
        }
      else {
        z = 0.0,0.0,1.0;
        }
      }
    }
  else {
    z /= znorm;
    }
  VectorType x = y ^ z;
  x.Normalize();

  double dangle = atan(1.0)*8.0/m_NumberOfSectors;
  double angle = 0.0;
  for(unsigned int i=0; i<m_NumberOfSectors; i++) {
    m_Vertex[i]  = m_Medial[0];
    m_Vertex[i] += x*(m_Radius[0]*cos(angle));
    m_Vertex[i] += z*(m_Radius[0]*sin(angle));
    angle += dangle;
    }

  //--------------------------
  //   Intermediary Section
  //--------------------------
  for(unsigned int s=1; s<m_NumberOfSections-1; s++) {
    v1 = m_Medial[ s ] - m_Medial[s-1];
    v2 = m_Medial[s+1] - m_Medial[ s ];
    y  = m_Medial[s+1] - m_Medial[s-1]; 
    z  = v2^v1;
    y.Normalize();
    znorm = z.GetNorm();
    if( znorm < 1e-16 ) {
      if( fabs(y[0]) < fabs(y[1]) ) {
        if( fabs(y[0]) < fabs(y[2]) ) {
          z = 1.0,0.0,0.0;
          }
        else {
          z = 0.0,0.0,1.0;
          }
        }
      else {
        if( fabs(y[1]) < fabs(y[2]) ) {
          z = 0.0,1.0,0.0;
          }
        else {
          z = 0.0,0.0,1.0;
          }
        }
      }
    else {
      z /= znorm;
      }
    x = y ^ z;
    x.Normalize();

    VectorType segment = v1;
    segment.Normalize();
    double cosx = x*(segment^z);
    for(unsigned int i=0; i<m_NumberOfSectors; i++) {
      VectorType pr = m_Vertex[i+(s-1)*m_NumberOfSectors]-m_Medial[s];
      pr -= segment*(pr*segment);  // take orthogonal subspace
      int pi = i+s*m_NumberOfSectors;
      m_Vertex[pi]  = m_Medial[s];
      double cz = (pr*z);
      double cx = (pr*x)/cosx;
      double fac = m_Radius[s]/sqrt( cx*cx + cz*cz );
      m_Vertex[pi] += z*fac*cz;
      m_Vertex[pi] += x*fac*cx;
      }
    }

  //----------------------
  //   Last Section
  //----------------------
  v1 = m_Medial[m_NumberOfSections-2] - m_Medial[m_NumberOfSections-3];
  v2 = m_Medial[m_NumberOfSections-1] - m_Medial[m_NumberOfSections-2];
  y  = m_Medial[m_NumberOfSections-1] - m_Medial[m_NumberOfSections-2];
  z  = v2^v1;
  y.Normalize();
  znorm = z.GetNorm();
  if( znorm < 1e-16 ) {
    if( fabs(y[0]) < fabs(y[1]) ) {
      if( fabs(y[0]) < fabs(y[2]) ) {
        z = 1.0,0.0,0.0;
        }
      else {
        z = 0.0,0.0,1.0;
        }
      }
    else {
      if( fabs(y[1]) < fabs(y[2]) ) {
        z = 0.0,1.0,0.0;
        }
      else {
        z = 0.0,0.0,1.0;
        }
      }
    }
  else {
    z /= znorm;
    }
  x = y ^ z;
  x.Normalize();

  int auxs = ((m_NumberOfSections-2)*m_NumberOfSectors);
  double fac = m_Radius[m_NumberOfSections-1]/m_Radius[m_NumberOfSections-2];
  for(unsigned int k=0; k<m_NumberOfSectors; k++) {
    VectorType pr = m_Vertex[k+auxs]-m_Medial[m_NumberOfSections-1];
    int pi = k+auxs+m_NumberOfSectors; 
    m_Vertex[pi]  = m_Medial[m_NumberOfSections-1];
    m_Vertex[pi] += z*fac*(pr*z);
    m_Vertex[pi] += x*fac*(pr*x);
    }

  return 1;
}





//--------------------------------------------------
//
//    Generate a PointSet on the surface normal to
//    the line of sight
//
//--------------------------------------------------
void Tube3D::GeneratePointSet( const VectorType & sight )  
{

  const unsigned int numberOfRadialSamples = 7;

  const double       radialWidthRatio      = 0.5;
  
  PointSetType::PointsContainer::Pointer                       
                  points = PointSetType::PointsContainer::New();

  PointSetType::PointDataContainer::Pointer  
                  values = PointSetType::PointDataContainer::New();

  unsigned long numberOfSamples = 2 
                                * m_NumberOfSections 
                                * numberOfRadialSamples;

  points->Reserve( numberOfSamples );
  values->Reserve( numberOfSamples );
  
  PointSetType::PointsContainer::Iterator      pi = points->Begin();
  PointSetType::PointDataContainer::Iterator   vi = values->Begin();

  PointType  medialA = ComputeCurrentPosition( m_Medial[1] );
  PointType  medialB = ComputeCurrentPosition( m_Medial[0] );

  VectorType tangent = medialA - medialB;
  VectorType normal  = sight;

  tangent.Normalize();
  normal.Normalize();


  const double AngleThreshold = 1e-4;

  unsigned int count = 0;
  for(unsigned int i=0; i<m_NumberOfSections; i++ )
  {

    if( i > 0 )
    {
      medialA = ComputeCurrentPosition( m_Medial[ i ] );
      medialB = ComputeCurrentPosition( m_Medial[i-1] );
      tangent = medialA - medialB;
    }


    PointType    medial  = ComputeCurrentPosition( m_Medial[i] );
    VectorType   radial  = tangent ^ normal;
    
    if( radial.GetNorm() < AngleThreshold ) 
    {
      // tangent and normal are almost parallel...
      // ignore this particular segment
      continue;
    }
    
    radial.Normalize();
    
    double factor = 1.0 - radialWidthRatio/2.0;
    for(unsigned int k=0; k<numberOfRadialSamples; k++)
    {
      const double x   = factor - 1.0;
      const double v   = x * exp( - x*x / radialWidthRatio*radialWidthRatio );
      vi.Value() = v;
      const VectorType offset = radial * factor * m_Radius[i];
      pi.Value() = medial + offset;
      ++pi;
      ++vi;
      vi.Value() = v;
      pi.Value() = medial - offset;
      ++pi;
      ++vi;
      factor += radialWidthRatio / numberOfRadialSamples;
    }
    count++;
  }

  m_PointSet->SetPoints( points );
  m_PointSet->SetPointData( values );

}




//--------------------------------------------------
//
//    Generate a PointSet on the surface
//
//--------------------------------------------------
void Tube3D::GeneratePointSet(void)  
{

  const unsigned int numberOfRadialSamples = 7;

  const double       radialWidthRatio      = 0.5;
  
  PointSetType::PointsContainer::Pointer                       
                  points = PointSetType::PointsContainer::New();

  PointSetType::PointDataContainer::Pointer  
                  values = PointSetType::PointDataContainer::New();

  unsigned long numberOfSamples = 
    m_NumberOfSections * m_NumberOfSectors * numberOfRadialSamples;

  points->Reserve( numberOfSamples );
  values->Reserve( numberOfSamples );
  
  PointSetType::PointsContainer::Iterator      pi = points->Begin();
  PointSetType::PointDataContainer::Iterator   vi = values->Begin();

  unsigned int count = 0;
  for(unsigned int i=0; i<m_NumberOfSections; i++ )
  {
    PointType medial = m_Medial[i];
    for(unsigned int j=0; j<m_NumberOfSectors; j++ )
    {
      VectorType radial = m_Vertex[ count ] - medial;
      double factor = 1.0 - radialWidthRatio/2.0;
      for(unsigned int k=0; k<numberOfRadialSamples; k++)
      {
        pi.Value() = medial + radial * factor;
        double x   =  factor-1.0;
        vi.Value() = x * exp( - x*x / radialWidthRatio*radialWidthRatio );
        ++pi;
        ++vi;
        factor += radialWidthRatio / numberOfRadialSamples;
      }
      count++;
    }
  }

  m_PointSet->SetPoints( points );
  m_PointSet->SetPointData( values );



}





} // end namespace li



