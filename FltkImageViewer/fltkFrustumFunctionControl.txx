/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkFrustumFunctionControl.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "fltkFrustumFunctionControl.h"

namespace fltk {
  
template <class ImagePixelType>
FrustumFunctionControl<ImagePixelType>
::FrustumFunctionControl()
{

  m_SpatialFunction = SpatialFunctionType::New();
  m_Shape           = ShapeType::New();

  ShapeType::ColorType color;
  color.SetRed(   0.8f );
  color.SetGreen( 1.0f );
  color.SetBlue(  0.8f );
  m_Shape->SetColor( color );

  m_DrawCommand  = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

}


  
template <class ImagePixelType>
FrustumFunctionControl<ImagePixelType>
::~FrustumFunctionControl()
{
}


   


template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetParametersFromGUI( void )
{ 
  this->SetAngleZ( angleZRoller->value() );

  this->SetApertureAngleX( apertureAngleXRoller->value() );
  this->SetApertureAngleY( apertureAngleYRoller->value() );

  this->SetApex( xAdjuster->value(),
                 yAdjuster->value(),
                 zAdjuster->value() );
}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetAngleZ( double angle )
{
 
  this->fltkFrustumFunctionControlGUI::SetAngleZ( angle );

  m_AngleZ = angle;
  m_SpatialFunction->SetAngleZ( angle );

  this->InvokeEvent( fltk::RedrawEvent );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApertureAngleX( double angle )
{
 
  this->fltkFrustumFunctionControlGUI::SetApertureAngleX( angle );

  m_Shape->SetApertureAngleX( angle );
  m_SpatialFunction->SetApertureAngleX( angle );

  this->InvokeEvent( fltk::RedrawEvent );

}



template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApertureAngleY( double angle )
{
 
  this->fltkFrustumFunctionControlGUI::SetApertureAngleY( angle );

  m_Shape->SetApertureAngleY( angle );
  m_SpatialFunction->SetApertureAngleY( angle );

  this->InvokeEvent( fltk::RedrawEvent );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetBottomPlane( double bottomPlane )
{
 
  this->fltkFrustumFunctionControlGUI::SetBottomPlane( bottomPlane );

  m_Shape->SetBottomPlane( bottomPlane );
  m_SpatialFunction->SetBottomPlane( bottomPlane );

  this->InvokeEvent( fltk::RedrawEvent );

}





template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetTopPlane( double topPlane )
{
 
  this->fltkFrustumFunctionControlGUI::SetTopPlane( topPlane );

  m_Shape->SetTopPlane( topPlane );
  m_SpatialFunction->SetTopPlane( topPlane );

  this->InvokeEvent( fltk::RedrawEvent );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApex( double x, double y, double z )
{
    
  SpatialFunctionType::InputType functionApex;
  functionApex[0] = x;
  functionApex[1] = y;
  functionApex[2] = z;

  m_SpatialFunction->SetApex( functionApex );

  this->fltkFrustumFunctionControlGUI::SetApexX( x );
  this->fltkFrustumFunctionControlGUI::SetApexY( y );
  this->fltkFrustumFunctionControlGUI::SetApexZ( z );

  this->InvokeEvent( fltk::RedrawEvent );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApexX( double x )
{
    
  const double y = yAdjuster->value();
  const double z = zAdjuster->value();

  this->SetApex( x, y , z );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApexY( double y )
{
    
  const double x = xAdjuster->value();
  const double z = zAdjuster->value();

  this->SetApex( x, y , z );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApexZ( double z )
{
    
  const double x = xAdjuster->value();
  const double y = yAdjuster->value();

  this->SetApex( x, y , z );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::glDraw( void ) const
{
  glPushMatrix();
  glTranslated( 
		xAdjuster->value(),
		yAdjuster->value(),
		zAdjuster->value() );
  glRotated(     180.0, 0.0,  1.0, 0.0 );
  glRotated(  m_AngleZ, 0.0, -1.0, 0.0 );
  m_Shape->glDraw();
  glPopMatrix();

}



} // end namespace itk
 
