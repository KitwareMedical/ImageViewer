/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltHeaderParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltHeaderParser_h
#define _VanderbiltHeaderParser_h

#include <fstream>

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkSize.h"
#include "itkFixedArray.h"

namespace itk
{

/** class VanderbiltHeaderParser
 *  
 * This componet parses a Vanderbilt image header file.
 *
 * This class is activatived by method Exceute().
 *
 * Inputs:
 *  - name of the header file
 *
 * Outputs:
 *  - name of the image file
 *  - size of the image
 *  - spacing of the image
 *  - image axes { L/R = 0,  P/A = 1, H/F = 2 }
 *  - image direction { L/P/H = false, R/A/F = true }
 *  - bool flag to indicate if image file is big endian
 *
 */
template <unsigned int VImageDimension = 3 >
class ITK_EXPORT VanderbiltHeaderParser : public Object
{

public:

  /** Standard class typedefs. */
  typedef VanderbiltHeaderParser Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VanderbiltHeaderParser, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Typedef support. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);
  typedef Size<VImageDimension> SizeType;
  typedef FixedArray<double,VImageDimension> SpacingType;
  typedef FixedArray<unsigned int, VImageDimension> AxesType;
  typedef FixedArray<bool, VImageDimension> DirectionType;

  /** Set the input file name. */
  itkSetStringMacro( FileName );

  /** Activiate the process. */
  virtual void Execute();

  /** Get the image size. */
  itkGetConstReferenceMacro( ImageSize, SizeType );

  /** Get the image spacing. */
  itkGetConstReferenceMacro( ImageSpacing, SpacingType );

  /** Get the image axes. */
  itkGetConstReferenceMacro( ImageAxes, AxesType );

  /** Get the image axes direction. */
  itkGetConstReferenceMacro( ImageDirection, DirectionType );

  /** Get the big endian boolean flag. */
  itkGetMacro( ImageBigEndian, bool );

protected:

   VanderbiltHeaderParser();
   ~VanderbiltHeaderParser(){};

private:

  VanderbiltHeaderParser( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string       m_FileName;

  SizeType          m_ImageSize;
  SpacingType       m_ImageSpacing;
  AxesType          m_ImageAxes;
  DirectionType     m_ImageDirection;
  bool              m_ImageBigEndian;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "VanderbiltHeaderParser.txx"
#endif

#endif
