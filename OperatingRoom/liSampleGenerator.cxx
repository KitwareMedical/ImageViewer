/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGenerator.cxx
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


