/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liLight.cxx
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
//     Project:   Operating Room
//
//     Author:    Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include <liLight.h>

namespace li {

unsigned int Light::m_Counter = 0;

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Light::Light(void)  
{
  m_Diffuse.SetRed(    1.0f );
  m_Diffuse.SetGreen(  1.0f );
  m_Diffuse.SetBlue(   1.0f );

  m_Specular.SetRed(   0.0f );
  m_Specular.SetGreen( 0.0f );
  m_Specular.SetBlue(  0.0f );

  m_Position =  0.0, 0.0, -1.0;
  
  m_LightNumber = (GLenum)(GL_LIGHT0 + m_Counter);
  m_Counter++;
}



//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
Light::~Light(void)  
{
}





//--------------------------------------------------
//
//    Set Diffuse Color
//
//--------------------------------------------------
void 
Light::SetDiffuseColor( const ColorType & color )
{
  m_Diffuse = color;
}



//--------------------------------------------------
//
//    Set Specular Color
//
//--------------------------------------------------
void 
Light::SetSpecularColor( const ColorType & color )
{
  m_Specular = color;
}



//--------------------------------------------------
//
//    Set Position
//
//--------------------------------------------------
void 
Light::SetPosition( const PointType & point )
{
  m_Position  = point;
}





//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
void 
Light::DrawGeometry(void)  const 
{
        glEnable( m_LightNumber );

        GLfloat specular[4];
  specular[0] = m_Specular.GetRed();
  specular[1] = m_Specular.GetGreen();
  specular[2] = m_Specular.GetBlue();
  specular[3] = 0.0f;

        GLfloat diffuse[4];
  diffuse[0] = m_Diffuse.GetRed();
  diffuse[1] = m_Diffuse.GetGreen();
  diffuse[2] = m_Diffuse.GetBlue();
  diffuse[3] = 0.0f;

  // the last zero implies a directional light
        GLfloat position[]  = { m_Position[0], m_Position[1], m_Position[2], 0.0 };

        glLightfv( m_LightNumber, GL_SPECULAR,  specular );
        glLightfv( m_LightNumber, GL_DIFFUSE,   diffuse  );
        glLightfv( m_LightNumber, GL_POSITION,  position );
        
}




} // end namespace li


