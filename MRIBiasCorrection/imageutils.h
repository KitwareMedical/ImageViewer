/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    imageutils.h
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) 2002 Insight Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __IMAGEUTILS_H
#define __IMAGEUTILS_H

#include <itkExceptionObject.h>
#include "itkMetaImageIOFactory.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkImageRegionIterator.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "myutils.h"

typedef itk::Image<float, 3> ImageType ;
typedef itk::Image<short, 3> MaskType ;
typedef ImageType::Pointer ImagePointer ;
typedef MaskType::Pointer MaskPointer ;
typedef itk::ImageFileReader< ImageType > ImageReaderType ;
typedef itk::ImageFileReader< MaskType > MaskReaderType ;
typedef itk::ImageFileWriter< ImageType > ImageWriterType ;

void copyImage(ImagePointer source , ImagePointer target)
{
  typedef itk::ImageRegionIteratorWithIndex<ImageType> ImageIterator ;
  ImageType::RegionType s_region = source->GetLargestPossibleRegion() ;
  ImageIterator s_iter(source, s_region ) ;

  
  if (target->GetLargestPossibleRegion().GetSize() != s_region.GetSize())
    {
      target->SetLargestPossibleRegion(s_region) ;
      target->SetBufferedRegion(s_region) ;
      target->Allocate() ;
    }
      
  ImageIterator t_iter(target, s_region) ;

  s_iter.GoToBegin() ;
  t_iter.GoToBegin() ;
  while (!s_iter.IsAtEnd())
    {
      t_iter.Set(s_iter.Get()) ;
      ++s_iter ;
      ++t_iter ;
    }
}


void logImage(ImagePointer source, ImagePointer target) 
{
  print_line("Applying logarithm to the image values...") ;
  ImageType::RegionType region ;
  region = source->GetLargestPossibleRegion() ;

  if (region.GetSize() != target->GetLargestPossibleRegion().GetSize())
    {
      target->SetLargestPossibleRegion(region) ;
      target->SetBufferedRegion(region) ;
      target->Allocate() ;
    }

  itk::ImageRegionIteratorWithIndex<ImageType> s_iter(source, region) ;
  itk::ImageRegionIteratorWithIndex<ImageType> t_iter(target, region) ;
  
  s_iter.GoToBegin() ;
  t_iter.GoToBegin() ;

  ImageType::PixelType pixel ;
  float log_pixel ;
  while (!s_iter.IsAtEnd())
    {
      pixel = s_iter.Get() ;
      
      log_pixel =  log( pixel + 1 ) ;
      if (pixel < 0)
        t_iter.Set( 0.0 ) ;
      else
        t_iter.Set( log_pixel ) ;

      ++s_iter ;
      ++t_iter ;
    }
  print_line("Conversion done.") ;
}

#endif
