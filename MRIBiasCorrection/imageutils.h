/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    imageutils.h
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
  ImageIOError() : ExceptionObject() {}
  
  ImageIOError(const char *file, unsigned int line, std::string fileName, std::string operation) : ExceptionObject(file, line)
  {
    FileName = fileName ;
    Operation = operation ;
  }

  ImageIOError(const std::string &nfile, unsigned int line, std::string fileName, std::string operation) : ExceptionObject(nfile, line)
  {
    FileName = fileName ;
    Operation = operation ;
  }
  
  std::string FileName ;
  std::string Operation ;
} ;

int readMetaImageHeader(std::string fileName, 
                        int& dimension, 
                        itk::Size<3>& size)
  throw (ImageIOError)
{
  MetaImage metaImage ;
  bool ret = metaImage.OpenMetaFile(fileName.c_str(), false);
  if (metaImage.Error()) 
    {
      throw ImageIOError(__FILE__, __LINE__, fileName, "Reading header: can't open the file") ;
    }
  
  dimension = metaImage.NDims() ;
  
  for (int i = 0 ; i < dimension ; i++)
    size[i] = metaImage.DimSize(i) ;
  
  return metaImage.NDims() ;
}


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
  int count = 0 ;
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
