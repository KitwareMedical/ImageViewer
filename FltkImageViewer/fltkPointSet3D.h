/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkPointSet3D.h
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


#ifndef fltkPointSet3D_h
#define fltkPointSet3D_h

#include "fltkShape3D.h"
#include "itkPointSet.h"


namespace fltk {


/**
 *  \class PointSet3D derives from Shape3D and 
 *         contains a pointer to an itk::PointSet.
 */
template <class TPointSet>
class ITK_EXPORT PointSet3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef PointSet3D<TPointSet>   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( PointSet3D, Shape3D );
  

  /** 
   * PointSet Type
   */
   typedef           TPointSet                    PointSetType;
   typedef  typename PointSetType::Pointer        PointSetPointer;
   typedef  typename PointSetType::ConstPointer   PointSetConstPointer;
   typedef  typename PointSetType::PointType      PointType;


  /** 
   * Points Container Type
   */
  typedef typename PointSetType::PointsContainer     PointsContainerType;
  typedef typename PointsContainerType::Pointer      PointsContainerPointer;

  /** 
   * Point Data Container Type
   */
  typedef typename PointSetType::PointDataContainer     PointDataContainerType;
  typedef typename PointDataContainerType::Pointer      PointDataContainerPointer;


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    

  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



  /**
   * Accepts a mesh
   */
  void SetPointSet( PointSetType * );



  /**
   * Return a mesh
   */
  PointSetConstPointer GetPointSet( void ) const;


protected:
  /**
   * Constructor
   */
  PointSet3D();

  
private:

   /**
   * PointSet that contains all the data
   */
   PointSetPointer                  m_PointSet;

};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkPointSet3D.txx>
#endif




#endif
