/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liTube3D.h
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
//     Project Angio Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------
#ifndef liTUBE3DCLASS
#define liTUBE3DCLASS

#include "fltkShape3D.h"
#include "itkVector.h"
#include "liPointSet3D.h"

namespace li {

/**Tube3D is the basic class to model a tube of variable
 * diameter. It is modeled as a set of points in the medial
 * axis of the tube, along with estimated radius at these
 * positions.*/ 
class ITK_EXPORT Tube3D : public fltk::Shape3D 
{
public:
  /** Standard class typedefs. */
  typedef Tube3D   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( Tube3D, fltk::Shape3D );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  unsigned int GetNumberOfSectors(void)  { return m_NumberOfSectors;  }
  unsigned int GetNumberOfSections(void) { return m_NumberOfSections; }
  void SetNumberOfSectors(unsigned int val)  {   m_NumberOfSectors = val; }
  void SetNumberOfSections(unsigned int val) {  m_NumberOfSections = val; }
 
  /** Typedef of the Point type. */
  typedef   itk::Point<double,3>    PointType;
    
  /** Typedef of the Vector type. */
  typedef   itk::Vector<double,3>    VectorType;

  /** Typedef of the PointSet type, */
  typedef   PointSet3D::PointSetType   PointSetType;

  bool Null(void) const;
  void DrawGeometry(void) const;
  int  Read(fltk::Shape3D::IfstreamType & is);
  int  Write(const char *filename) const;
  int  GenerateSkin(void);
  void GeneratePointSet(void);
  void GeneratePointSet( const VectorType & sight ); 
  void ScalePoints(double sx,double sy, double sz);
  
  void SetRadius( float radius );

  PointType & GetMedial( unsigned int i ) const { return m_Medial[i]; }
  PointType & GetVertex( unsigned int i ) const { return m_Vertex[i]; }
  float & GetRadius( unsigned int i ) { return m_Radius[i]; }
  void Smooth(void);

  void SubsampleUsingRadius(void);
  void Copy(const Tube3D & sm );
  bool Alloc(unsigned int nsect, unsigned int nsecc);
  PointSetType::ConstPointer GetPointSet( void ) const
    { return m_PointSet.GetPointer(); }

protected:
  Tube3D();
  ~Tube3D();
  void Clear(void);
  
private:
  unsigned int    m_NumberOfSectors;        // Number of radial sectors
  unsigned int    m_NumberOfSections;     // Number of rectilinear sections
  float        *m_Radius;             // Array of radius
  PointType    *m_Vertex;             // Array of vertex coordinates
  PointType    *m_Medial;             // Array of medial Point3Ds
  PointSetType::Pointer    m_PointSet; // Point Set

};

} // end namespace li

#endif

