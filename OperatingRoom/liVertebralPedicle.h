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

#ifndef liVERTEBRALPEDICLECLASS
#define liVERTEBRALPEDICLECLASS


#include <liCylinder3D.h>



namespace li {


/**
 *  This class implements a 3D model of a generic Vertebral Pedicle 
 */
class ITK_EXPORT VertebralPedicle : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef VertebralPedicle   Self;

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
  itkTypeMacro( VertebralPedicle, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Destructor
   */
	~VertebralPedicle();


  /**
   * Set Vertical Position of the pedicle
   */
	void    SetVerticalPosition(double val);


  /**
   * Set Horizontal Position of the pedicle
   */
	void    SetHorizontalPosition(double val);

  /**
   * Set Frontal Position of the pedicle
   */
	void    SetFrontalPosition(double val);


  /**
   * Set Vertical Angle of the pedicle
   */
	void    SetVerticalAngle(double val);


  /**
   * Set Horizontal Angle of the pedicle
   */
	void    SetHorizontalAngle(double val);


  /**
   * Set Vertical Radius of the pedicle
   */
	void    SetVerticalRadius(double val);


  /**
   * Set Horizontal Radius of the pedicle
   */
	void    SetHorizontalRadius(double val);


  /**
   * Set the Length of the pedicle
   */
	void    SetLength(double val);


  /**
   * Set the number of sectors on the cylinder
   */
	void    SetNumberOfSectors(unsigned int num);


  /**
   * Get Vertical Position of the pedicle
   */
	double  GetVerticalPosition(void) const;


  /**
   * Get Horizontal Position of the pedicle
   */
	double  GetHorizontalPosition(void) const;

  /**
   * Get Frontal Position of the pedicle
   */
	double  GetFrontalPosition(void) const;


  /**
   * Get Vertical Angle of the pedicle
   */
	double  GetVerticalAngle(void) const;


  /**
   * Get Horizontal Angle of the pedicle
   */
	double  GetHorizontalAngle(void) const;


  /**
   * Get Vertical Radius of the pedicle
   */
	double  GetVerticalRadius(void) const;


  /**
   * Get Horizontal Radius of the pedicle
   */
	double  GetHorizontalRadius(void) const;


  /**
   * set the Length of the pedicle
   */
	double  GetLength(void) const;


  /**
   * Get the number of sectors on the cylinder
   */
	unsigned int  GetNumberOfSectors(void) const;



  /**
   * Draw the model using OpenGL commands
   */
	void    DrawGeometry(void) const;


  /**
   * Write the serialized object to a stream
   */
  int     Write( Shape3D::OfstreamType &os, int level ) const;


  /**
   * Read the serialized object from a stream
   */
  int     Read( Shape3D::IfstreamType &is);

protected:
	VertebralPedicle();
	VertebralPedicle(const VertebralPedicle & b);
	const VertebralPedicle & operator=(const VertebralPedicle & b);


private:

	double                m_VerticalPosition;
	double                m_HorizontalPosition;
	double                m_FrontalPosition;
	double                m_VerticalAngle;
	double                m_HorizontalAngle;
	double                m_VerticalRadius;
	double                m_HorizontalRadius;
	double                m_Length;
  Cylinder3D::Pointer   m_LeftTube;
  Cylinder3D::Pointer   m_RightTube;
  unsigned int          m_NumberOfSectors;


};



}   // end namespace li


#endif




