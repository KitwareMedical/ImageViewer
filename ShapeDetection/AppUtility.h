/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    AppUtility.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __AppUtility_h
#define __AppUtility_h

#include "PGMVolumeWriter.h"

template <class TImage>
bool
WritePGMFiles(
typename TImage::Pointer input, 
const char * dirname,
const char * basename )
{

  typedef itk::PGMVolumeWriter<TImage> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetImage( input );
  writer->SetDirectoryName( dirname );
  writer->SetFilePrefix( basename );
  writer->Execute();

  return true;
}

#endif
