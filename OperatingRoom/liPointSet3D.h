/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liPointSet3D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liPOINTSET3DCLASS
#define liPOINTSET3DCLASS

#include "fltkShape3D.h"
#include "itkPointSet.h"

namespace li {

/** \class PointSet3D derives from Shape3D and contains an itk::PointSet */
class ITK_EXPORT PointSet3D : public fltk::Shape3D 
{
public:
  /** Standard class typedefs. */
  typedef PointSet3D   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( PointSet3D, fltk::Shape3D );
  
  /** PointSet Type Defaults */
  typedef ::itk::DefaultStaticMeshTraits<
                      double,
                      3,3,
                      double,
                      double> PointSetDefaultType;

  /** PointSet Type */
  typedef ::itk::PointSet<double,3,PointSetDefaultType>   PointSetType;

  /*** Points Container Type */
  typedef PointSetType::PointsContainer     PointsContainerType;

  /** Point Data Container Type */
  typedef PointSetType::PointDataContainer     PointDataContainerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Draw the geometry of the shape */
  void DrawGeometry(void) const;

  /** Accepts a mesh. */
  void SetPointSet( PointSetType * );

  /** Return a mesh. */
  PointSetType::ConstPointer GetPointSet( void );

protected:
  /** Constructor */
  PointSet3D();
  
private:
  /** PointSet that contains all the data */
   PointSetType::Pointer                  m_PointSet;
};


} // end namespace li


#endif
