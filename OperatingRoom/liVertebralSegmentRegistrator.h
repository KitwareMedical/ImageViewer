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

#ifndef liVERTEBRALSEGMENTREGISTRATORCLASS
#define liVERTEBRALSEGMENTREGISTRATORCLASS

#include "liVertebralSegment.h"
#include "itkImage.h"
#include "itkPointSetToImageRigid3DPatternIntensityRegularStepGradientDescentRegistration.h"


namespace li {


/**
 *  This class implements a 3D model of a generic Vertebral Segment 
 */
class ITK_EXPORT VertebralSegmentRegistrator : public itk::Object 
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef VertebralSegmentRegistrator   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Object  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * PointSet type
   */
  typedef PointSet3D::PointSetType  PointSetType;

  
  /** 
   * Fluoroscopic image type 
   */
  // Azucar
  // typedef itk::Image<unsigned char, 2> FluoroscopicImageType;
  typedef itk::Image<unsigned short, 2> FluoroscopicImageType;


  /** 
   * Volume image type 
   */
  typedef itk::Image<unsigned char, 3> VolumeImageType;


  /** 
   * PointSet / Volume Registration Method
   */
  typedef
    itk::PointSetToImageRigid3DPatternIntensityRegularStepGradientDescentRegistration<
                                                      VolumeImageType,
                                                      PointSetType
                                                          >  VolumeRegistrationMethod;

  /**
   * Volume Registration Optimizer
   */
   typedef  VolumeRegistrationMethod::OptimizerType  
                                              VolumeRegistrationOptimizer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( VertebralSegmentRegistrator, Object );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

	
  /**
   *  Get the Vertebral Pedicles
   */
  VertebralSegment::Pointer     GetVertebralSegment(void);
	
  
  /**
   *  Get the Volume Registrator
   */
  VolumeRegistrationMethod::Pointer GetVolumeRegistrator(void);
	

protected:

  /**
   *  Constructor
   */
	VertebralSegmentRegistrator();


  /**
   *  Destructor
   */
	~VertebralSegmentRegistrator();


private:

  /**
   *  Model of the Vertebral Segment
   */
  VertebralSegment::Pointer        m_VertebralSegment;


  /**
   *  Registration method for PointSet / Volume
   */
  VolumeRegistrationMethod::Pointer   m_VolumeRegistrator;


	};


} // end namespace li

#endif




