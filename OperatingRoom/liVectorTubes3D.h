/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVectorTubes3D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project Point Associator
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------
#ifndef liVECTORTUBES3DCLASS
#define liVECTORTUBES3DCLASS

#include "liTube3D.h"
#include "fltkBox3D.h"
#include "liPointSet3D.h"
#include <vector>

namespace li 
{

/** VectorTubes3D represents a set of Tubes3D */
class ITK_EXPORT VectorTubes3D : public fltk::Shape3D 
{
public:
  /** Standard class typedefs. */
  typedef VectorTubes3D   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( VectorTubes3D, fltk::Shape3D );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Typedef for the vector of tubes.  */
  typedef std::vector<Tube3D::Pointer> VectorTubeType;

  /** Typedef for the geometric vector type.  */
  typedef Tube3D::VectorType  VectorType;
     
  /** Typedef for the geometric point type.  */
  typedef Tube3D::PointType  PointType;
     
  /** Typedef for the set of points.  */
  typedef PointSet3D::PointSetType  PointSetType;
     
  /** Remove the tube on the list */
  void Clear(void);    

  /** Draw the OpenGL representation of the tubes */
  void DrawGeometry(void) const;

  /** Read the tube information from an ifstream */
  void Read(fltk::Shape3D::IfstreamType & ifs);

  /** Load the tube information from a file specified by the filename. */
  void Load(const char * filename);

  /** Save the tube information in a file specified by the file name. */
  int Save(const char * filename) const;

  /** Enable the visualization of the boundaries from which tubes have been
   *  extracted */
  void EnableBoundary(void);

  /** Disable the visualization of the boundaries from which tubes 
   *  have been extracted */
  void DisableBoundary(void);

  void SetRotation(GLdouble irz);
  void SetOffset(const VectorType & newoffset);
  void SetExternalOffset(const VectorType & newoffset);
  VectorType GetOffset(void) const;
  VectorType GetExternalOffset(void) const;
  GLdouble GetRotation(void) const;
      
  void GeneratePointSet( const VectorType & sight );

  void SetDisplayPointSet( bool value)
    { m_DisplayPointSet = value; }

  bool GetDisplayPointSet( void ) const
    { return m_DisplayPointSet; }
  
  PointSetType::ConstPointer GetPointSet(void) 
    { return m_PointSet3D->GetPointSet(); }
    
protected:
  VectorTubes3D();
  ~VectorTubes3D();

private:
  GLint nx,ny,nz;      // Dimensions of Volume data
  GLdouble dx,dy,dz;  // Voxel size
  GLdouble rz;        // Volume orientation (z angle)
  GLdouble tx,ty,tz;  // Cube offset
  GLdouble ex,ey,ez;  // Cube external offset (with respect to patient's head)

  fltk::Box3D::Pointer      m_Boundary;      // Boundary of Volume data
  VectorTubeType            m_Tubes;
  PointSet3D::Pointer       m_PointSet3D;
  bool                      m_DisplayPointSet;
};

} // end namespace li

#endif

