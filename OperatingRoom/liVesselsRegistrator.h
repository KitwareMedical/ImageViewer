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

#ifndef liVESSELSREGISTRATORCLASS
#define liVESSELSREGISTRATORCLASS

#include <liVectorTubes3D.h>
#include <itkImage.h>
#include <itkPointSetToImageRigid3DPerspectivePatternIntensityRegularStepGradientDescentRegistration.h>


namespace li {


/**
 *  This class implements a Registration method 
 */
class ITK_EXPORT VesselsRegistrator : public itk::Object 
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef VesselsRegistrator   Self;

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
   * PointSet / Fluoroscopic Image Registration Method
   */
  typedef
    itk::PointSetToImageRigid3DPerspectivePatternIntensityRegularStepGradientDescentRegistration<
                                                      FluoroscopicImageType,
                                                      PointSetType
                                                          >  RegistrationMethod;

  /**
   * Registration Optimizer
   */
   typedef  RegistrationMethod::OptimizerType  RegistrationOptimizer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( VesselsRegistrator, Object );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

	
  /**
   *  Get the Vessels
   */
  VectorTubes3D::Pointer GetVessels( void );
	

  /**
   *  Set the Vessels
   */
  void SetVessels( VectorTubes3D  * vessels );
	
  
  /**
   *  Set the Fluoroscopic image
   */
  void SetImage( FluoroscopicImageType  * image );
	 

  /**
   *  Get the  Registrator
   */
  RegistrationMethod::Pointer GetRegistrator(void);
	

protected:

  /**
   *  Constructor
   */
	VesselsRegistrator();


  /**
   *  Destructor
   */
	~VesselsRegistrator();


private:

  /**
   *  Model of the Vertebral Segment
   */
  VectorTubes3D::Pointer        m_Vessels;


  /**
   *  Registration method for PointSet / Fluoroscopic Image
   */
  RegistrationMethod::Pointer   m_Registrator;


  /**
   *  Fluoroscopic Image 
   */
  FluoroscopicImageType::Pointer  m_Image;

	};


} // end namespace li

#endif




