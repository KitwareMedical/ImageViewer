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


#ifndef liPATIENTCLASS
#define liPATIENTCLASS


#include <liSpineModel.h>
#include <liVectorTubes3D.h>
#include <liBox3D.h>
#include <itkAffineTransform.h>



namespace li {

/** 
 *  Patinent implementes the support for all the patient's
 *  anatomy models. It holds together the different representations
 *  for organs that could be relevant during a particular type
 *  of intervention
 */
class ITK_EXPORT Patient : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef Patient   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Patient, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  /**
   * Destructor
   */
  ~Patient();


  /**
   * Affine Transform type
   */
   typedef itk::AffineTransform< double, 3 >  TransformType;

  
  /**
   * DrawGeometry implements the drawing of the table.
   * this method is called by liShape3D in its Draw method.
   */
  void DrawGeometry(void) const;


  /**
   *  Get the Extrinsic transformation
   */
  TransformType::ConstPointer GetExtrinsic( void ) const;


  /**
   *  Defines the position of the patient on the Surgery Table
   */
	typedef enum { 
            Ventral, 
            Supine, 
            LateralLeft, 
            LateralRight 
                          } ClinicalPosition;


  /**
   *  Set the position of the patient on the Surgery Table
   */
  void SetClinicalPosition( ClinicalPosition newPos );


  /**
   *  Set differential correction in pose obtained from 
   *  a registration method
   */
  void SetRegistrationOffset( const VectorType & trans, 
                              const VectorType & rot     ); 

  /**
   *  Clear the value of registration offset. 
   *  This method will reset the position of the patient
   *  to its original settings
   */
  void ClearRegistrationOffset(void);


  /**
   *  Set the upper bound in translation correction
   */
  void SetCorrectionTranslationBound( double limit );


  /**
   *  Test whether values are inside the predefined bounds
   */
  bool CorrectionOutOfBounds(void) const;


  /**
   *  Get a Pointer to the Spine Model
   */
  SpineModel::Pointer & GetSpineModel( void );


  /**
   *  Get a Pointer to the Vessels Model
   */
  VectorTubes3D::Pointer & GetVesselsModel( void );



protected:
  /**
   *  Constructor
   */
	Patient();


private:

  Box3D::Pointer                 m_Body;

	mutable TransformType::Pointer m_Extrinsic;

  TransformType::Pointer         m_RegistrationCorrection;

	ClinicalPosition             m_ClinicalPosition;

  double                       m_CorrectionTranslationBound;

  SpineModel::Pointer          m_Spine;
  
  VectorTubes3D::Pointer       m_Vessels;

};


}  // end namespace li

#endif
