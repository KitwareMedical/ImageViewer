/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkReadMetaImage.txx
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
#ifndef _itkReadMetaImage_txx
#define _itkReadMetaImage_txx

#include "itkReadMetaImage.h"
#include "itkObjectFactory.h"
#include "itkSimpleImageRegionIterator.h"
#include <MetaImageLib.h>

namespace itk
{

/**
 *
 */
template <class TOutputImage>
ReadMetaImage<TOutputImage>
::ReadMetaImage()
{
  m_FileName = "";
}

/**
 *
 */
template <class TOutputImage>
void 
ReadMetaImage<TOutputImage>
::GenerateData()
{
  
  MetaImage   metaImage;
  
  metaImage.OpenMetaFile(m_FileName.c_str(), true);

  if( metaImage.Error() ) 
    {
    throw ExceptionObject();
    }

  if( metaImage.NDims() != TOutputImage::ImageDimension )
    {
    throw ExceptionObject();
    }
  
  typename TOutputImage::Pointer m_OutputImage( GetOutput() );

  m_OutputImage->SetSpacing( metaImage.ElemSize() );
  m_OutputImage->SetOrigin( metaImage.Position() );

  Size dimSize;

  for(unsigned int i=0; i<TOutputImage::ImageDimension; i++) 
    {
    dimSize[i] = metaImage.DimSize(i);
    }

  const unsigned long startPosition[] = { 0, 0, 0 };
  typename TOutputImage::IndexType start;
  start.SetIndex( startPosition );

  Region region;
  
  region.SetSize( dimSize );
  region.SetIndex( start );

  m_OutputImage->SetLargestPossibleRegion( region );
  m_OutputImage->SetRequestedRegion( region );
  m_OutputImage->SetBufferedRegion( region );
  m_OutputImage->Allocate();

  typedef typename TOutputImage::PixelType  PixelType;

  typedef itk::SimpleImageRegionIterator< TOutputImage> IteratorType;
  
  IteratorType it(m_OutputImage,
                  m_OutputImage->GetLargestPossibleRegion() );

  if( typeid(TOutputImage::PixelType) == typeid(unsigned char) )
    {
    metaImage.ConvertTo(MET_UCHAR);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(char) )
    {
    metaImage.ConvertTo(MET_CHAR);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(unsigned short))
    {
    metaImage.ConvertTo(MET_USHORT);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(short) )
    {
    metaImage.ConvertTo(MET_SHORT);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(unsigned int) )
    {
    metaImage.ConvertTo(MET_UINT);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(int) )
    {
    metaImage.ConvertTo(MET_INT);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(float) )
    {
    metaImage.ConvertTo(MET_FLOAT);
    }
  else if( typeid(TOutputImage::PixelType) == typeid(double) )
    {
    metaImage.ConvertTo(MET_DOUBLE);
    }
  else
    {
    itkErrorMacro(<<"Couldn't convert pixel type");
    return;
    }

  PixelType * source = (PixelType *)metaImage.Get();
  
  for ( it.Begin(); !it.IsAtEnd(); ++it ) 
    {
    it.Set( *source++ );
    }
}


/**
 *
 */
template <class TOutputImage>
void 
ReadMetaImage<TOutputImage>
::PrintSelf(itk::Ofstream& os, Indent indent)
{
  Superclass::PrintSelf(os,indent);

}

} // end namespace itk

#endif
