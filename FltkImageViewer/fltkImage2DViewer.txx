/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkImage2DViewer.txx
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

#include "fltkImage2DViewer.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkMinimumMaximumImageCalculator.h"

namespace fltk {
  
template <class ImagePixelType>
Image2DViewer<ImagePixelType>
::Image2DViewer()
{
  imageViewer->SetIntensityWindow( intensityWindow );
  imageViewer->SetParentWindow( externalWindow );
  
  m_Command = ObserverCommandType::New();
  m_Command->SetViewer( this );

  m_Tag = 0L;
}


  
template <class ImagePixelType>
Image2DViewer<ImagePixelType>
::~Image2DViewer()
{
}




template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::SetImage(ImageType * image)
{
  
  if( m_Image && m_Tag )
    {
    m_Image->GetSource()->RemoveObserver( m_Tag );
    }
  m_Image = image;
  m_Tag = m_Image->GetSource()->AddObserver( 
      itk::Command::EndEvent, m_Command );
  Update();

}





template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::Update(void) 
{

  if( !m_Image )
    {
    return;
    }

  typename ImageType::RegionType region  = m_Image->GetRequestedRegion();
  typename ImageType::SizeType   size    = region.GetSize();  

  if(    size[0] != static_cast<unsigned int>(imageViewer->GetWidth())
      || size[1] != static_cast<unsigned int>(imageViewer->GetHeight())  )
  {
    this->externalWindow->size(size[0],size[1]);
    imageViewer->Allocate( size[0], size[1] );
  }

  fltk::Image2DViewerWindow::DataType * buffer = imageViewer->GetBuffer();

  typedef itk::MinimumMaximumImageCalculator<ImageType> MinMaxCalculatorType;
  MinMaxCalculatorType::Pointer MinMaxCalculator =  MinMaxCalculatorType::New();
  MinMaxCalculator->SetImage(m_Image);
  MinMaxCalculator->Compute();

  typename ImageType::PixelType max = MinMaxCalculator->GetMaximum();
  typename ImageType::PixelType min = MinMaxCalculator->GetMinimum();


  double factor = 
    255.0/(static_cast<double>(max)-static_cast<double>(min));

 
  itk::ImageLinearConstIteratorWithIndex< ImageType > 
                                        it( m_Image,region );

  it.SetDirection( 0 );

  const unsigned int totalSize = size[0]*size[1]*4;
  const unsigned int totalWidth = size[0]*4;
  unsigned char * dest = buffer + totalSize - totalWidth;


  while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
  {
    while( !it.IsAtEndOfLine() ) 
    {
       double value = ( it.Get() - min ) * factor;
       const unsigned char valuec = static_cast<unsigned char>( value );
      *dest++ = valuec;
      *dest++ = valuec;
      *dest++ = valuec;
      *dest++ = 255;
      ++it;
    }
    it.NextLine();
    dest -= 2 * totalWidth;
  
  }

  imageViewer->redraw();
  Fl::check();

}






} // end namespace itk
