/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    mydefs.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MYDEFS_H
#define __MYDEFS_H

#include <itkImage.h>

typedef itk::Image<float, 3> ImageType ;
typedef itk::Image<unsigned char, 3> MaskType ;
typedef ImageType::Pointer ImagePointer ;
typedef MaskType::Pointer MaskPointer ;

#endif
