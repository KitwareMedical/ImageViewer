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
#include "vtkCellArray.h"
#include "vtkActor.h"
#include "fltkTimeProbesCollector.h"
#include "itkThinPlateSplineKernelTransform.h"
#include "itkThinPlateR2LogRSplineKernelTransform.h"
#include "itkElasticBodySplineKernelTransform.h"
#include "itkElasticBodyReciprocalSplineKernelTransform.h"
#include "itkVolumeSplineKernelTransform.h"
#include "vtkThinPlateSplineTransform.h"

#include <set>



// This is the base classe for the Application

class ThinPlateSplinesApplicationBase 
{

public:

  enum { PointsDimension              = 3 };

  typedef double       CoordinateRepresentationType;

  typedef itk::Point< CoordinateRepresentationType,
                                  PointsDimension >  PointType;

  typedef std::vector< PointType >                   PointArrayType;
      
  typedef itk::ThinPlateSplineKernelTransform< 
                              CoordinateRepresentationType,
                              PointsDimension >  ThinPlateSplineTransformType;

  typedef itk::ThinPlateR2LogRSplineKernelTransform< 
                              CoordinateRepresentationType,
                              PointsDimension >  ThinPlateR2LogRSplineTransformType;

  typedef ThinPlateSplineTransformType::Pointer  
                                          ThinPlateSplineTransformPointer;

  typedef ThinPlateR2LogRSplineTransformType::Pointer  
                                        ThinPlateR2LogRSplineTransformPointer;

  typedef ThinPlateSplineTransformType::PointSetType      PointSetType;
  typedef PointSetType::Pointer                           PointSetPointer;


  typedef itk::ElasticBodySplineKernelTransform< 
                              CoordinateRepresentationType,
                              PointsDimension >  ElasticBodySplineTransformType;

  typedef itk::ElasticBodyReciprocalSplineKernelTransform< 
                              CoordinateRepresentationType,
                              PointsDimension >  ElasticBodyReciprocalSplineTransformType;

  typedef ElasticBodySplineTransformType::Pointer  ElasticBodySplineTransformPointer;

  typedef ElasticBodyReciprocalSplineTransformType::Pointer  
                                          ElasticBodyReciprocalSplineTransformPointer;

  typedef itk::VolumeSplineKernelTransform< 
                              CoordinateRepresentationType,
                              PointsDimension >  VolumeSplineTransformType;

  typedef VolumeSplineTransformType::Pointer  VolumeSplineTransformPointer;



public:

  ThinPlateSplinesApplicationBase();
  virtual ~ThinPlateSplinesApplicationBase();
  
  virtual void DisplayAxes(void);
  virtual void CreateSpline(void);
  virtual void CreateLandMarks(void);

  virtual void DisplayLandMarks(void);
  virtual void DisplayPoints(void);
  virtual void ShowTimeProbes(void);
  virtual void SelectSourceLandMark(void) = 0;
  virtual void SelectTargetLandMark(void) = 0;
  virtual void UpdateSelectedSourceLandMark(void) = 0;
  virtual void UpdateSelectedTargetLandMark(void) = 0;


protected:
 
  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;


  PointArrayType    m_Points;   // Points to pass though the transformation

  PointSetPointer   m_SourceLandMarks;   
  PointSetPointer   m_TargetLandMarks;   

  PointArrayType    m_PointsToTransform;   

  PointArrayType    m_PointsTransformedByITK;   
  
  vtkPoints       * m_VTKSourceLandMarks;
  vtkPoints       * m_VTKTargetLandMarks;

  vtkPoints       * m_VTKPointsToTransform;
  vtkCellArray    * m_VTKLinesToTransform;

  vtkPoints       * m_VTKPointsTransformedByITK;
  vtkCellArray    * m_VTKLinesTransformedByITK;

  vtkPoints       * m_VTKPointsTransformedByVTK;
  vtkCellArray    * m_VTKLinesTransformedByVTK;

  fltk::TimeProbesCollector  m_TimeCollector;

  virtual void    MapPoints(void);
  virtual void    MapPointsITK(void);
  virtual void    MapPointsElasticBodySplineITK(void);
  virtual void    MapPointsElasticBodyReciprocalSplineITK(void);
  virtual void    MapPointsThinPlateSplineITK(void);
  virtual void    MapPointsThinPlateR2LogRSplineITK(void);
  virtual void    MapPointsVolumeSplineITK(void);
  virtual void    MapPointsVTK(void);
  virtual void    CreateSourcePoints(void);
  virtual void    ConvertITKMappedPointsToVTK(void);
  virtual void    RemoveActors(void);

  ThinPlateSplineTransformPointer     m_ThinPlateSplineTransformITK;

  ThinPlateR2LogRSplineTransformPointer m_ThinPlateR2LogRSplineTransformITK;

  ElasticBodySplineTransformPointer   m_ElasticBodySplineTransformITK;

  ElasticBodyReciprocalSplineTransformPointer   
                                 m_ElasticBodyReciprocalSplineTransformITK;

  VolumeSplineTransformPointer        m_VolumeSplineTransformITK;

  vtkThinPlateSplineTransform       * m_ThinPlateSplineTransformVTK;

private:

  void TransferLandMarksToVTK(void);
  void CreateRenderer(void);


  typedef std::set< vtkActor * >  ActorsArrayType;
  ActorsArrayType                 m_ActorsToDelete;

};




#endif
