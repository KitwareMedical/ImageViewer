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


#ifndef liLIGHTCLASS
#define liLIGHTCLASS

#include "fltkShape3D.h"



namespace li {




/**
 *  Light derives from Shape3D and represents a light source 
 */
class ITK_EXPORT Light : public fltk::Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef Light   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltk::Shape3D  Superclass;

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
  itkTypeMacro( Light, fltk::Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

 
  /**
   * Set Light position
   */
  void SetPosition( const PointType & );

  
  /**
   * Set Diffuse Color
   */
  void SetDiffuseColor( const ColorType & color );

 
  /**
   * Set Specular Color
   */
  void SetSpecularColor( const ColorType & color );


  /**
   * Return light position
   */
  const PointType & GetPosition(void) const;


  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



protected:
  /**
   * Constructor
   */
  Light();


  /**
   * Destructor
   */
  ~Light();


  /**
   * Copy Constructor
   */
  Light( const Light & box );


  /**
   * Assignment Operator
   */
  const Light & operator=( const Light & box );


private:

  /**
   * Diffuse color of the light
   */
   ColorType m_Diffuse;
 

  /**
   * Specular color of the light
   */
   ColorType m_Specular;

 
  /**
   * Position of the light
   */
   PointType m_Position;
 

  /**
   * Numeric ID of this light
   */
   GLenum m_LightNumber;
 

  /**
   * Counter of lights (OpenGL only guarranties 5)
   */
   static unsigned int m_Counter;
 
};


} // end namespace li


#endif
