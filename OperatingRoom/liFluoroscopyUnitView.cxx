//--------------------------------------------
//
//     Project:  Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------

#include <liFluoroscopyUnitView.h>
#include <liBox3D.h>
#include <liCommandEvents.h>
#include <itkPoint.h>
#include <itkImageSliceConstIterator.h>
#include <itkSimpleImageRegionConstIterator.h>
#include <GL/glu.h>
#include <cmath>


namespace li {


//--------------------------------------------
//
//    Creator
//
//--------------------------------------------
FluoroscopyUnitView
::FluoroscopyUnitView(int x,int y,int w,int h, const char * label)
:fltk::GlWindow(x,y,w,h,label) 
{
  m_Zoom = 1.0;
  m_Notifier = itk::LightObject::New();
  m_ImageBackground = 0;
}



//--------------------------------------------
//
//    Set position of the X-ray source
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetSourcePosition( const PointType & position )
{
  m_SourcePosition = position;
}



//--------------------------------------------
//
//    Get position of the X-ray source
//
//--------------------------------------------
const FluoroscopyUnitView::PointType &
FluoroscopyUnitView
::GetSourcePosition( void ) const
{
  return m_SourcePosition;
}



//--------------------------------------------
//
//    Set radius of the Image Detector
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetDetectorRadius( double radius )
{
  m_DetectorRadius = radius;
}




//--------------------------------------------
//
//    Set position of the Image Detector
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetDetectorPosition( const PointType & position )
{
  m_DetectorPosition = position;
}



//--------------------------------------------
//
//    Get position of the Image Detector
//
//--------------------------------------------
const FluoroscopyUnitView::PointType &
FluoroscopyUnitView
::GetDetectorPosition( void ) const
{
  return m_DetectorPosition;
}



//--------------------------------------------
//
//    Set vertical direction of the Image Detector
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetDetectorVerticalDirection( const VectorType & direction )
{
  m_DetectorVerticalDirection = direction;
}



//--------------------------------------------
//
//    Get vertical direction of the image detector
//
//--------------------------------------------
const FluoroscopyUnitView::VectorType &
FluoroscopyUnitView
::GetDetectorVerticalDirection( void ) const
{
  return m_DetectorVerticalDirection;
}





//--------------------------------------------
//
//    Set the zoom factor
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetZoom( double zoom )
{
  m_Zoom = zoom;
}



//--------------------------------------------
//
//    Get position of the Image Detector
//
//--------------------------------------------
double
FluoroscopyUnitView
::GetZoom( void ) const
{
  return m_Zoom;
}





//--------------------------------------------
//
//    Get the Notifier
//
//--------------------------------------------
itk::LightObject::Pointer
FluoroscopyUnitView::GetNotifier(void) 
{
  return m_Notifier;
}



//--------------------------------------------
//
//    Set fluoroscopy image
//
//--------------------------------------------
void
FluoroscopyUnitView::SetFluoroscopyImage( const ImageType * image )
{
  if( !image ) 
  {
    return;
  }

  m_Image = image;

  if( m_ImageBackground )
  {
    delete [] m_ImageBackground;
  }
  
  ImageType::SizeType size = m_Image->GetBufferedRegion().GetSize();
  
  m_ImageBackgroundWidth  = size[0];
  m_ImageBackgroundHeight = size[1];

  const unsigned long totalSize = 
              m_ImageBackgroundHeight * m_ImageBackgroundWidth ;

  if( !totalSize ) 
  {
    return;
  }

  itk::SimpleImageRegionConstIterator< ImageType > 
                                        at( m_Image,
                                            m_Image->GetBufferedRegion() );

  at.Begin();
  
  ImageType::PixelType max = at.Get();
  ImageType::PixelType min = at.Get();

  while( !at.IsAtEnd() )
  {
    ImageType::PixelType value = at.Get();
    if( value > max )
    {
      max = value;
    }
    else if( value < min )
    {
      min = value;
    }
    ++at;
  }
  

  m_ImageBackground = new unsigned char [ totalSize ];
  
  itk::ImageSliceConstIterator< ImageType > 
                                        it( m_Image,
                                            m_Image->GetBufferedRegion() );

  it.SetFirstDirection( 0 );
  it.SetSecondDirection( 1 );

  unsigned char * dest = m_ImageBackground + totalSize - m_ImageBackgroundWidth;

  const float factor = 255.0 / (max - min );

  it.Begin();
  while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
  {
    while( !it.IsAtEndOfSlice() )
    {
      while( !it.IsAtEndOfLine() ) 
      {
        *dest = (unsigned char)( ( it.Get() - min ) * factor );
        ++it;
        ++dest;
      }
      it.NextLine();
      dest -= 2 * m_ImageBackgroundWidth;
    }
    it.NextSlice();
  }

}






//--------------------------------------------
//
//    Draw the model
//
//--------------------------------------------
void FluoroscopyUnitView::draw(void) 
{
  
  glViewport(0,0,w(),h());

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_NORMALIZE );
  glEnable( GL_DEPTH_TEST );
  glShadeModel( GL_SMOOTH );
  
  glClearColor((GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0,(GLfloat)1.0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if( m_Image ) 
  {
    ImageType::SizeType size = m_Image->GetBufferedRegion().GetSize();

    const unsigned long imageNx = size[0];
    const unsigned long imageNy = size[1];

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0,w(),0,h(),-1,1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    const GLfloat imgZoom = m_Zoom * ((GLfloat)h()) / imageNy;

    const int imageXpos = (int)( ( w() - imageNx * imgZoom ) / 2.0 );
    const int imageYpos = (int)( ( h() - imageNy * imgZoom ) / 2.0 );

    const int imageIsLSB    = 0;
    const int imageByteSwap = 0;

    glRasterPos2i(0,0);
    // this allows negative raster positions
    glBitmap(0,0,0,0,imageXpos,imageYpos,NULL);

    glPixelStorei( GL_UNPACK_LSB_FIRST, imageIsLSB );
    glPixelStorei( GL_UNPACK_SWAP_BYTES, imageByteSwap );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, imageNx );

    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glDisable( GL_DEPTH_TEST );

    glPixelZoom( imgZoom, imgZoom );

    glDrawPixels( m_ImageBackgroundWidth,
                  m_ImageBackgroundHeight,
                  GL_LUMINANCE,
                  GL_UNSIGNED_BYTE,
                  m_ImageBackground );

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable( GL_DEPTH_TEST );
    glClear(GL_DEPTH_BUFFER_BIT);
  }



  const GLdouble rad2degree = 45.0f / atan( 1.0f );

  const GLdouble distance = 
            m_DetectorPosition.EuclideanDistanceTo( m_SourcePosition );

  const GLdouble fieldOfView = static_cast<GLdouble>( 
                rad2degree * 2.0 * atan2( m_DetectorRadius, distance ) );

  const GLdouble aspect     = 
    static_cast<GLdouble>( w() ) / static_cast<GLdouble>( h() );

  const GLdouble nearPlane  = 10.0f;
  const GLdouble farPlane   = distance;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective( fieldOfView, aspect, nearPlane, farPlane );

  gluLookAt( m_SourcePosition[0],
             m_SourcePosition[1],
             m_SourcePosition[2],
             m_DetectorPosition[0],
             m_DetectorPosition[1],
             m_DetectorPosition[2],
             m_DetectorVerticalDirection[0],
             m_DetectorVerticalDirection[1],
             m_DetectorVerticalDirection[2]  );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  VectorType lineOfSight = m_SourcePosition - m_DetectorPosition;
  
  GLfloat lightPosition[] = {   lineOfSight[0],
                                lineOfSight[1],
                                lineOfSight[2], 
                                0.0  };
 
  GLfloat specular[]      = {   0.0, 0.0, 0.0, 0.0 };
  GLfloat diffuse[]       = {   0.9, 0.9, 0.9, 0.0 };

  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
  glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHTING );


  m_Notifier->InvokeEvent( GLDrawEvent  );

}


}  // end namespace li


