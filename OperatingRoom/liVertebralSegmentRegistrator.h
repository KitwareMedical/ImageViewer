/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralSegmentRegistrator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liVERTEBRALSEGMENTREGISTRATORCLASS
#define liVERTEBRALSEGMENTREGISTRATORCLASS

#include "liVertebralSegment.h"
#include "itkImage.h"
#include "itkPointSetToImageRigid3DPatternIntensityRegularStepGradientDescentRegistration.h"


namespace li {


/**  This class implements a 3D model of a generic Vertebral Segment.  */
class ITK_EXPORT VertebralSegmentRegistrator : public itk::Object 
{

public:
  /** Standard class typedefs. */
  typedef VertebralSegmentRegistrator   Self;
  typedef itk::Object  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** PointSet type */
  typedef PointSet3D::PointSetType  PointSetType;

  /** Fluoroscopic image type  */
  typedef itk::Image<unsigned char, 2> FluoroscopicImageType;
  //typedef itk::Image<unsigned short, 2> FluoroscopicImageType;

  /** Volume image type. */
  typedef itk::Image<unsigned char, 3> VolumeImageType;

  /*** PointSet / Volume Registration Method. */
  typedef
    itk::PointSetToImageRigid3DPatternIntensityRegularStepGradientDescentRegistration<VolumeImageType,PointSetType>  VolumeRegistrationMethod;

  /** Volume Registration Optimizer. */
   typedef VolumeRegistrationMethod::OptimizerType VolumeRegistrationOptimizer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( VertebralSegmentRegistrator, Object );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
        
  /**  Get the Vertebral Pedicles */
  VertebralSegment::Pointer     GetVertebralSegment(void);
  
  /**  Get the Volume Registrator */
  VolumeRegistrationMethod::Pointer GetVolumeRegistrator(void);

protected:
  /**  Constructor */
  VertebralSegmentRegistrator();

  /**  Destructor */
  ~VertebralSegmentRegistrator();

private:
  /**  Model of the Vertebral Segment */
  VertebralSegment::Pointer        m_VertebralSegment;

  /**  Registration method for PointSet / Volume */
  VolumeRegistrationMethod::Pointer   m_VolumeRegistrator;

};

} // end namespace li

#endif




