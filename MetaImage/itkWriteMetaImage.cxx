/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWriteMetaImage.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#include "itkImage.h"
#include <itkScalar.h>

#define ITK_MANUAL_INSTANTIATION
#include "itkWriteMetaImage.h"

namespace itk
{


/**
 *   Specialized version of GetTypeCode()
 */


/**
 *   Type Code for char
 */
template <>
MET_Type 
WriteMetaImage<Image<signed char,2> >
::GetTypeCode(void) const
{
  return MET_CHAR;
}

/**
 *   Type Code for char
 */
template <>
MET_Type 
WriteMetaImage<Image<signed char,3> >
::GetTypeCode(void) const
{
  return MET_CHAR;
}

/**
 *   Type Code for unsigned char
 */
template <>
MET_Type 
WriteMetaImage<Image<unsigned char,2> >
::GetTypeCode(void) const
{
  return MET_UCHAR;
}

/**
 *   Type Code for unsigned char
 */
template <>
MET_Type 
WriteMetaImage<Image<unsigned char,3> >
::GetTypeCode(void) const
{
  return MET_UCHAR;
}

/**
 *   Type Code for float
 */
template <>
MET_Type 
WriteMetaImage<Image<float,2> >
::GetTypeCode(void) const
{
  return MET_FLOAT;
}

/**
 *   Type Code for float
 */
template <>
MET_Type 
WriteMetaImage<Image<float,3> >
::GetTypeCode(void) const
{
  return MET_FLOAT;
}

/**
 *   Type Code for float
 */
template <>
MET_Type 
WriteMetaImage<Image<Scalar<float>,3> >
::GetTypeCode(void) const
{
  return MET_FLOAT;
}

/**
 *   Type Code for unsigned short
 */
template <>
MET_Type 
WriteMetaImage<Image<unsigned short,2> >
::GetTypeCode(void) const
{
  return MET_USHORT;
}

/**
 *   Type Code for unsigned short
 */
template <>
MET_Type 
WriteMetaImage<Image<unsigned short,3> >
::GetTypeCode(void) const
{
  return MET_USHORT;
}

/**
 *   Type Code for short
 */
template <>
MET_Type 
WriteMetaImage<Image<short,2> >
::GetTypeCode(void) const
{
  return MET_SHORT;
}

/**
 *   Type Code for short
 */
template <>
MET_Type 
WriteMetaImage<Image<short,3> >
::GetTypeCode(void) const
{
  return MET_SHORT;
}

} // end namespace itk
