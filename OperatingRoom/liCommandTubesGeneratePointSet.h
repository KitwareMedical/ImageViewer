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


#ifndef liCOMMANDTUBESGENERATEPOINTSET
#define liCOMMANDTUBESGENERATEPOINTSET

#include <itkCommand.h>
#include <liCommandEvents.h>
#include <liVectorTubes3D.h>
#include <liFluoroscopyCArmMobile.h>



namespace li {

/**
 *  Implementation of the Command Pattern for 
 *  the generation of points in the tube surface 
 *  according to the line of sight defined in 
 *  the fluoroscopy unit
 */
class ITK_EXPORT CommandTubesGeneratePointSet : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandTubesGeneratePointSet   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Command  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will call redraw in the GL Window
   */
  void Execute(itk::LightObject *caller, unsigned long event);
  void Execute(const itk::LightObject *caller, unsigned long event);


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandTubesGeneratePointSet, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  
  /**
   * Type defining the vector of Tubes
   */
  typedef   VectorTubes3D    VectorTubesType;


  /**
   * Type defining the Fluoroscopy Unit type
   */
  typedef   FluoroscopyCArmMobile    FluoroscopyUnitType;


  /**
   * Set the Fluorosocpy unit from which to get
   * the line of sight
   */
  void SetFluoroscopyUnit( FluoroscopyUnitType * unit )
  { m_FluoroscopyUnit = unit; }

  /**
   * Set the vector of Tubes for which points have
   * to be generated
   */
  void SetTubes( VectorTubesType * tubes )
  { m_Tubes = tubes; }




protected:
  /**
   * Constructor
   */
  CommandTubesGeneratePointSet();

private:

  /**
   * The fluoroscopy unit from which 
   * the point of view is extracted
   */
  FluoroscopyUnitType::ConstPointer     m_FluoroscopyUnit;
 
  /**
   * The vector of Tubes3D for which points 
   * have to be generated
   */
  VectorTubesType::Pointer         m_Tubes;
 
  
};


} // end namespace li


#endif
