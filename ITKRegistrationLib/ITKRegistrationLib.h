/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ITKRegistrationLib.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ITK_REGISTRATION_LIB
#define ITK_REGISTRATION_LIB

/**
 * This function wraps the multi-resolution mutual information
 * registration module of ITK.   Via this wrapping that registration
 * module can be called from c.   For details on the parameters, etc
 * of the registration method, see its example in 
 * Insight/Applications/MultiResMIRegistration.
 **/

extern bool ITKRegistrationLib(int * fixedDimSize, float * fixedOrigin, 
                               float * fixedSpacing, 
                               unsigned short * fixedImageData, 
                               int * movingDimSize, float * movingOrigin, 
                               float * movingSpacing, 
                               unsigned short * movingImageData,
                               unsigned short * resultImageData);

#endif
