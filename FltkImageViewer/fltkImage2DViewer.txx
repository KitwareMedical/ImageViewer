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


namespace fltk {
  
template <class ImagePixelType>
Image2DViewer<ImagePixelType>
::Image2DViewer()
{

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
  
  m_Image = image;

  Update();

}





template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::Update(void) 
{
  if( m_Image )
    {
    return;
    }

  ImageType::RegionType region  = m_Image->GetRequestedRegion();
  ImageType::SizeType   size    = region.GetSize();  

  if(    size[0] != static_cast<unsigned int>(imageViewer->w())
      || size[1] != static_cast<unsigned int>(imageViewer->h())  )
  {
    imageViewer->Allocate( size[0], size[1] );
  }

  fltk::Image2DViewerWindow::DataType * buffer = imageViewer->GetBuffer();

  ImageType::PixelType max;
  ImageType::PixelType min;

  typedef itk::ImageLinearConstIteratorWithIndex<ImageType> IteratorType;
    
  IteratorType  it( m_Image, region );

  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
      const ImageType::PixelType value = it.Get();
      if( max < value )
        {
        max = value;
        }
      if( min > value )
        {
        min = value;
        }

      ++it;
    }

  double factor = 
    255.0/(static_cast<double>(max)-static_cast<double>(min));
    
  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
      double value = ( it.Get() - min ) * factor;
      *buffer++ = static_cast<unsigned char>( value );
      ++it;
    }

}






} // end namespace itk
