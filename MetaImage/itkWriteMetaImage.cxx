/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWriteMetaImage.cxx
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

#include "itkImage.h"
#include "itkPhysicalImage.h"
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

// Now we gotta do the same for physical image variants...
template <>
MET_Type
WriteMetaImage<PhysicalImage<signed char,2> >
::GetTypeCode(void) const
{
  return MET_CHAR;
}

/**
 *   Type Code for char
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<signed char,3> >
::GetTypeCode(void) const
{
  return MET_CHAR;
}

/**
 *   Type Code for unsigned char
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<unsigned char,2> >
::GetTypeCode(void) const
{
  return MET_UCHAR;
}

/**
 *   Type Code for unsigned char
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<unsigned char,3> >
::GetTypeCode(void) const
{
  return MET_UCHAR;
}

/**
 *   Type Code for float
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<float,2> >
::GetTypeCode(void) const
{
  return MET_FLOAT;
}

/**
 *   Type Code for float
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<float,3> >
::GetTypeCode(void) const
{
  return MET_FLOAT;
}

/**
 *   Type Code for float
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<Scalar<float>,3> >
::GetTypeCode(void) const
{
  return MET_FLOAT;
}

/**
 *   Type Code for unsigned short
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<unsigned short,2> >
::GetTypeCode(void) const
{
  return MET_USHORT;
}

/**
 *   Type Code for unsigned short
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<unsigned short,3> >
::GetTypeCode(void) const
{
  return MET_USHORT;
}

/**
 *   Type Code for short
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<short,2> >
::GetTypeCode(void) const
{
  return MET_SHORT;
}

/**
 *   Type Code for short
 */
template <>
MET_Type
WriteMetaImage<PhysicalImage<short,3> >
::GetTypeCode(void) const
{
  return MET_SHORT;
}

} // end namespace itk
