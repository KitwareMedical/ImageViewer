/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWriteMetaImage.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkWriteMetaImage_txx
#define _itkWriteMetaImage_txx

#include <string>

#include "itkWriteMetaImage.h"
#include "itkObjectFactory.h"
#include <MetaImageLib.h>
#include "itkImageRegionIterator.h"

namespace itk
{

/**
 *
 */
template <class TInputImage>
WriteMetaImage<TInputImage>
::WriteMetaImage()
{
  m_FileName = "";
}





/**
 *
 */
template <class TInputImage>
void 
WriteMetaImage<TInputImage>
::GenerateData(void)
{
  this->WriteData();  
}





/**
 *
 */
template <class TInputImage>
void 
WriteMetaImage<TInputImage>
::WriteData(void)
{
  
  const unsigned int BitsPerPixel = 
                          8*sizeof( PixelType );

  const unsigned int dimension = TInputImage::ImageDimension;

  typename TInputImage::Pointer m_InputImage( GetInput() );

  int dimSize[ dimension ];

  const typename TInputImage::SizeType & size = 
        m_InputImage->GetBufferedRegion().GetSize();
        
  const double         *spacing = m_InputImage->GetSpacing();                
  float fspacing[dimension];

  for(unsigned int i=0; i<dimension; i++) 
  {
    dimSize[i]  = size[i];
    fspacing[i] = spacing[i];
  }

  typedef typename TInputImage::PixelType PixelType;
  PixelType *yetAnotherBuffer = 0 ;

  try 
    {
      yetAnotherBuffer = 
        new PixelType[ m_InputImage->GetOffsetTable()[dimension] ];
    }
  catch(std::bad_alloc)
    {
      throw ExceptionObject(__FILE__, __LINE__) ;
    }

  typedef ImageRegionIterator< TInputImage > IteratorType;

  IteratorType it(      m_InputImage, 
                    m_InputImage->GetBufferedRegion() );


  
  PixelType * destination = yetAnotherBuffer;


  while( !it.IsAtEnd() ) 
  {
    *destination++ = it.Get(); 
    ++it;
  }

  MetaImage saver(  dimension,
                    dimSize,
                    GetTypeCode(),
                    fspacing,
                    BitsPerPixel,
                    MET_SYSTEM_BYTE_ORDER_MSB,
                    yetAnotherBuffer  );


  std::string fileExtension ;
  fileExtension= m_FileName.substr(m_FileName.rfind('.') + 1, 
                                   m_FileName.size() - m_FileName.rfind('.')) ;

  // if file extension is mhd, then save the file with header file (.mhd)
  // and data file (.raw)
  if (fileExtension == "mhd")
    {
      std::string baseFileName = 
        m_FileName.substr(0, m_FileName.rfind('.')) ;
      std::string rawFileName = baseFileName + ".raw" ;
      saver.Save( this->m_FileName.c_str(), rawFileName.c_str(), 1);
    }
  else
      saver.Save( this->m_FileName.c_str(), 0, 1);
    

  delete [] yetAnotherBuffer;

  if(saver.Error())  {
    std::cerr << "Unable to open file:" << m_FileName << std::endl;
    return;
  }

}






/**
 *
 */
template <class TInputImage>
void 
WriteMetaImage<TInputImage>
::PrintSelf(std::ofstream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Meta Image file format writer" << std::endl;
}







} // end namespace itk

#endif
