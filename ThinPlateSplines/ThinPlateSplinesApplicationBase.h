/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThinPlateSplinesApplicationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ThinPlateSplinesApplicationBase_H
#define ThinPlateSplinesApplicationBase_H


#include "itkMacro.h"

#include "vtkRenderWindow.h"
#include "itkPoint.h"
#include "vtkPoints.h"

#include <set>



// This is the base classe for the Application

class ThinPlateSplinesApplicationBase 
{

public:

  enum { PointsDimension              = 3 };

  typedef float       CoordinateRepresentationType;

  typedef itk::Point< CoordinateRepresentationType,
                                  PointsDimension >  PointType;

  typedef std::vector< PointType >            PointArrayType;
      
public:

  ThinPlateSplinesApplicationBase();
  ~ThinPlateSplinesApplicationBase();
  
  virtual void DisplayAxes(void);
  virtual void CreateSpline(void);
  virtual void CreateLandMarks(void);
  virtual void DisplayLandMarks(void);


protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;


  PointArrayType    m_Points;   // Points to pass though the transformation

  PointArrayType    m_SourceLandMarks;   
  PointArrayType    m_TargetLandMarks;   
  
  vtkPoints       * m_VTKSourceLandMarks;
  vtkPoints       * m_VTKTargetLandMarks;


private:

  void TransferLandMarksToVTK(void);

};




#endif
