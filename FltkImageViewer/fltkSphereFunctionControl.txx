/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSphereFunctionControl.txx
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

#include "fltkSphereFunctionControl.h"

namespace fltk {
  
template <class ImagePixelType>
SphereFunctionControl<ImagePixelType>
::SphereFunctionControl()
{

  m_SpatialFunction = SpatialFunctionType::New();
  m_Shape           = ShapeType::New();

  ShapeType::ColorType color;
  color.SetRed(   0.8f );
  color.SetGreen( 1.0f );
  color.SetBlue(  0.8f );
  m_Shape->SetColor( color );
  m_Shape->SetNumberOfSlices( 60 );
  m_Shape->SetNumberOfStacks( 30 );

  m_DrawCommand  = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

}


  
template <class ImagePixelType>
SphereFunctionControl<ImagePixelType>
::~SphereFunctionControl()
{
}


   


template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetParametersFromGUI( void )
{ 
  this->SetRadius( radiusAdjuster->value() );
  this->SetCenter( xAdjuster->value(),
                   yAdjuster->value(),
                   zAdjuster->value() );
}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetRadius( double radius )
{
 
  this->fltkSphereFunctionControlGUI::SetRadius( radius );

  m_Shape->SetRadius( radius );
  m_SpatialFunction->SetRadius( radius );

  this->InvokeEvent( fltk::RedrawEvent() );

}





template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenter( double x, double y, double z )
{
    
  typename SpatialFunctionType::InputType functionCenter;
  functionCenter[0] = x;
  functionCenter[1] = y;
  functionCenter[2] = z;

  m_SpatialFunction->SetCenter( functionCenter );

  this->fltkSphereFunctionControlGUI::SetCenterX( x );
  this->fltkSphereFunctionControlGUI::SetCenterY( y );
  this->fltkSphereFunctionControlGUI::SetCenterZ( z );

  this->InvokeEvent( fltk::RedrawEvent() );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenterX( double x )
{
    
  const double y = yAdjuster->value();
  const double z = zAdjuster->value();

  this->SetCenter( x, y , z );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenterY( double y )
{
    
  const double x = xAdjuster->value();
  const double z = zAdjuster->value();

  this->SetCenter( x, y , z );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenterZ( double z )
{
    
  const double x = xAdjuster->value();
  const double y = yAdjuster->value();

  this->SetCenter( x, y , z );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::glDraw( void ) const
{
  glPushMatrix();
  glTranslated( 
		xAdjuster->value(),
		yAdjuster->value(),
		zAdjuster->value() );

  m_Shape->glDraw();
  glPopMatrix();

}



} // end namespace itk
