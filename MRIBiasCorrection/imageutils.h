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
#include <MetaImageLib.h>
#include <itkReadMetaImage.h>
#include <itkWriteMetaImage.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "mydefs.h"
#include "myutils.h"


class ImageIOError : public itk::ExceptionObject
{
 public:
  ImageIOError() : itk::ExceptionObject() {}
  
  ImageIOError(const char *file, unsigned int line, std::string fileName, std::string operation) : itk::ExceptionObject(file, line)
  {
    FileName = fileName ;
    Operation = operation ;
  }

  ImageIOError(const std::string &nfile, unsigned int line, std::string fileName, std::string operation) : itk::ExceptionObject(nfile, line)
  {
    FileName = fileName ;
    Operation = operation ;
  }
  
  std::string FileName ;
  std::string Operation ;
} ;

void loadImage(std::string fileName, ImagePointer image)
  throw (ImageIOError)
{
  if (fileName == "")
    {
      throw ImageIOError(__FILE__, __LINE__, "", "Reading meta image: file name not specified") ;
    }

  try
    {
      typedef itk::ReadMetaImage<ImageType> Reader ;
      Reader::Pointer reader = Reader::New() ;
      
      reader->SetOutput(image) ;
      reader->SetFileName(fileName.c_str()) ;
      reader->Update() ;
    }
  catch (itk::ExceptionObject)
    {
      throw ImageIOError(__FILE__, __LINE__, fileName, "Reading meta image: failed to read") ;
    }
}


void loadMask(std::string fileName, MaskPointer mask)
{
  if (fileName == "")
    {
      throw ImageIOError(__FILE__, __LINE__, "", "Reading meta image: file name not specified") ;
    }

  try
    {
      typedef itk::ReadMetaImage<MaskType> Reader ;
      Reader::Pointer reader = Reader::New() ;
      
      reader->SetOutput(mask) ;
      reader->SetFileName(fileName.c_str()) ;
      reader->Update() ;
    }
  catch (itk::ExceptionObject)
    {
      throw ImageIOError(__FILE__, __LINE__, fileName, "Reading meta image: failed to read") ;
    }
}

void writeImage(std::string fileName, ImagePointer image)
  throw (ImageIOError)
{
  if (fileName == "")
    {
      throw ImageIOError(__FILE__, __LINE__, "", "Writing meta image: file name not specified") ;
    }

  try
    {
      typedef itk::WriteMetaImage<ImageType> Writer ;
      Writer::Pointer writer = Writer::New() ;
      
      writer->SetInput(image) ;
      writer->SetFileName(fileName.c_str()) ;
      writer->GenerateData() ;
    }
  catch (itk::ExceptionObject)
    {
      throw ImageIOError(__FILE__, __LINE__, fileName, "Writing meta image: failed to write") ;
    }

}

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
