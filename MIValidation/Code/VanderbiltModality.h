/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltModality.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltModality_h
#define _VanderbiltModality_h

namespace itk
{

class VanderbiltModality
{
public:

  /** Enum of the different modalities. */
  enum Modalities { CT = 0, MRT1, MRPD, MRT2, MRMPRAGE, PET,
    MRT1rectified, MRPDrectified, MRT2rectified };

  typedef enum Modalities ModalityType;

  /** Enum of the number of modalities. */
  enum { NumberOfModalities = 9 };

  /** Table of modality directory names. */
  static const char DirectoryName[NumberOfModalities][20];

  /** Table of modality description. */
  static const char Description[NumberOfModalities][20];

};


} // namespace itk

#endif

