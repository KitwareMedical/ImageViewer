//--------------------------------------------
//
//    Project Registrator
//
//    Author:  Luis Ibanez
//  
//    Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------


#include <liSampleGenerator.h>




//----------------------------------------------
//
//    Constructor
//
//----------------------------------------------
liSampleGenerator::liSampleGenerator() 
{
}


//----------------------------------------------
//
//    Destructor
//
//----------------------------------------------
liSampleGenerator::~liSampleGenerator() 
{
}





//----------------------------------------------
//
//    Show
//
//----------------------------------------------
void 
liSampleGenerator::Show( void ) 
{
  controlWindow->show();
}




//----------------------------------------------
//
//    Hide
//
//----------------------------------------------
void 
liSampleGenerator::Hide( void ) 
{
  controlWindow->hide();
}






//----------------------------------------------
//
//    Set Name
//
//----------------------------------------------
void 
liSampleGenerator::SetName( const char * name )
{
  controlWindow->label( name );
}




//----------------------------------------------
//
//    Get Name
//
//----------------------------------------------
const char *
liSampleGenerator::GetName( void ) const
{
  return controlWindow->label();
}




//----------------------------------------------
//
//    Set Position
//
//----------------------------------------------
void
liSampleGenerator::SetPosition( const PointType & point )
{
  xPositionInput->value( point[0] );
  yPositionInput->value( point[1] );
  zPositionInput->value( point[2] );
}


