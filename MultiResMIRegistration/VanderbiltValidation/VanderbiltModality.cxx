/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltModality.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltModality_cxx
#define _VanderbiltModality_cxx

#include "VanderbiltModality.h"

namespace itk
{

const char 
VanderbiltModality::
DirectoryName[NumberOfModalities][20]
= { "ct", "mr_T1", "mr_PD", "mr_T2", "mr_MP-RAGE", "pet",
    "mr_T1_rectified", "mr_PD_rectified", "mr_T2_rectified" };

const char 
VanderbiltModality::
Description[NumberOfModalities][20]
= { "CT", "MR_T1", "MR_PD", "MR_T2", "MR_MP_RAGE", "PET",
    "MR_T1-rectified", "MR_PD-rectified", "MR_T2-rectified" };


} // namespace itk

#endif
