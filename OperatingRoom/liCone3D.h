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


#ifndef liCONE3DCLASS
#define liCONE3DCLASS

#include <liShape3D.h>



namespace li {




/**
 *  Cone3D derives from Shape3D and represents a cone in 3D.
 */
class ITK_EXPORT Cone3D : public Shape3D 
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef Cone3D   Self;

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
  itkTypeMacro( Cone3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Set the Radius
   */
	void SetRadius( double radius );


  /**
   * Set the Height
   */
	void SetHeight( double height );


  /**
   * Set the Number Of Sectors to digitize the base
   */
	void SetNumberOfSectors( unsigned int number );


  /**
   * Get the Radius
   */
	double GetRadius(void) const;


  /**
   * Get the Height
   */
	double GetHeight(void) const;


  /**
   * Get the Number of Sectors
   */
	unsigned int GetNumberOfSectors(void) const;


  /**
   * Draw the Cone using OpenGL commands
   */
	void DrawGeometry(void) const;



protected:
  /**
   * Constructor
   */
	Cone3D();

private:
  /**
   *  Radius of the base
   */
	double          m_Radius;

  /**
   *   Height of the cone
   */
	double          m_Height;

  /**
   *   Number of Sector in which the circle is digitized
   */
	unsigned int    m_NumberOfSectors;


	};


}   // end namespace li

#endif
