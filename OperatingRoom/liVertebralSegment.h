/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralSegment.h
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
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------
#ifndef liVERTEBRALSEGMENTMODELCLASS
#define liVERTEBRALSEGMENTMODELCLASS

#include "liPointSet3D.h"
#include "liVertebralBody.h"
#include "liVertebralPedicle.h"

namespace li {

/** This class implements a 3D model of a generic Vertebral Segment  */
class ITK_EXPORT VertebralSegment : public fltk::Shape3D
{
public:
  /** Standard class typedefs. */
  typedef VertebralSegment   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** PointSet type */
  typedef PointSet3D::PointSetType  PointSetType;

  /** Run-time type information (and related methods). */
  itkTypeMacro( VertebralSegment, fltk::Shape3D );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /**  Destructor */
  ~VertebralSegment();

  /**  Draw the Model using OpenGL commands */
  void DrawGeometry(void) const;
  
  /**  Set the name of this model */
  virtual void SetName(const char *newName);
  
  /**  Get the name of this model */
  const char * GetName(void);
  
  /**  Set the vertical shift with respect to the previous Segment */
  void SetShift(double val);
   
  /**  Get the vertical shift with respect to the previous Segment */
  double GetShift(void);
  
  /**  Set the rotation with respect to the previous Segment */
  void SetRotation(double val);
  
  /**  Get the rotation with respect to the previous Segment */
  double GetRotation(void);

  /**  Get the Vertebral body */
  VertebralBody::Pointer        GetVertebralBody(void);
        
  /**  Get the Vertebral Pedicles */
  VertebralPedicle::Pointer     GetVertebralPedicle(void);
  
  /**  Write the serialized model to an ostream */
  int Write( fltk::Shape3D::OfstreamType &os, int level ) const;
  
  /**  Read the serialized model of an ostream */
  virtual int Read( fltk::Shape3D::IfstreamType &is );

  virtual void SetAxisVisible( bool );

  virtual void SetSamplesVisible( bool );

  /** Draw the axis of the coordinate system * attached to a particular
   *  vertebra this will be removed from here... later */
  virtual void DrawAxis( void ) const;

  PointSet3D::Pointer & GetLeftPediclePointSet(void )  
    { return m_LeftPediclePointSet;  }
  PointSet3D::Pointer & GetRightPediclePointSet(void ) 
    { return m_RightPediclePointSet; }
  PointSet3D::Pointer & GetTopBodyPointSet(void )      
    { return m_TopBodyPointSet;      }
  PointSet3D::Pointer & GetBottomBodyPointSet(void )   
    { return m_BottomBodyPointSet;   }

protected:
  /**  Constructor */
  VertebralSegment();

private:
  /**  Name assigned to this model */
  char *      m_Name;

  /**  Separation from the previous Vertebral Segment */
  double      m_Shift;

  /**  Rotation with respect to the previous Segment */
  double      m_Rotation;
  
  /**  Model of the Vertebral Body */
  VertebralBody::Pointer        m_Body;

  /**  Model of the Pedicles (both left and right) */
  VertebralPedicle::Pointer     m_Pedicles;

  /**  Defines whether the axis are visible or not */
  bool                          m_AxisVisible;

  /**  Defines whether the samples are visible or not */
  bool                          m_SamplesVisible;

  /**  Receptors for synthetic samples. */
  PointSet3D::Pointer               m_LeftPediclePointSet;
  PointSet3D::Pointer               m_RightPediclePointSet;
  PointSet3D::Pointer               m_TopBodyPointSet;
  PointSet3D::Pointer               m_BottomBodyPointSet;

};

} // end namespace li

#endif




