/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkReadMetaImage.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
    throw ExceptionObject(__FILE__, __LINE__);
    }

  if( metaImage.NDims() != TOutputImage::ImageDimension )
    {
    throw ExceptionObject(__FILE__, __LINE__);
    }
  
  typename TOutputImage::Pointer m_OutputImage( GetOutput() );

  m_OutputImage->SetSpacing( metaImage.ElemSize() );
  m_OutputImage->SetOrigin( metaImage.Position() );

  Size dimSize;

  for(unsigned int i=0; i<TOutputImage::ImageDimension; i++) 
    {
    dimSize[i] = metaImage.DimSize(i);
    }

  typedef typename TOutputImage::IndexType IndexType;

  IndexType start = IndexType::ZeroIndex;

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
  
  for ( ; !it.IsAtEnd(); ++it ) 
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
::PrintSelf(std::ofstream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "ReadMetaImage class" << std::endl;
  os << indent << "  file name " << m_FileName << std::endl;

}

} // end namespace itk

#endif
