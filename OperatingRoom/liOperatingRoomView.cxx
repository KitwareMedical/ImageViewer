/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liOperatingRoomView.cxx
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

#include <liCommandEvents.h>
#include <liOperatingRoomView.h>
#include <GL/glu.h>




namespace li {


//--------------------------------------------
//
//              Creator
//
//--------------------------------------------
OperatingRoomView
::OperatingRoomView(int x,int y,int w,int h, const char * label)
:fltk::GlWindowInteractive(x,y,w,h,label) 
{
  SetSensitivity( 800.0f );
  SetBackground( 0.9f, 0.9f, 0.9f );
  SetAzimuth( 150.0f );
  SetAltitude( 30.0f );
  m_Notifier = itk::Object::New();
}



//--------------------------------------------
//
//              Get the Notifier
//
//--------------------------------------------
itk::Object::Pointer
OperatingRoomView::GetNotifier(void) 
{
  return m_Notifier;
}



//--------------------------------------------
//
//              Draw the model
//
//--------------------------------------------
void OperatingRoomView::draw(void) 
{

        glViewport(0,0,w(),h());
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        const GLdouble width = 1800.0;
        const GLdouble height = width*h()/w();

        glOrtho(-width,width,-height,height,-20000,10000);

  // Put Z axis up
  glRotatef( -90.0f, 0.1f, 0.0f, 0.0f );

  // User selected view
  SetViewPoint();


  glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_NORMALIZE);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        
  glClearColor((GLfloat)0.9,(GLfloat)0.9,(GLfloat)0.9,(GLfloat)1.0);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

  glEnable( GL_LIGHTING );

  m_Notifier->InvokeEvent( GLDrawEvent() );


}


}  // end namespace li


