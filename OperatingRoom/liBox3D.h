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


#ifndef liBOX3DCLASS
#define liBOX3DCLASS

#include <liShape3D.h>



namespace li {




/**
 *  Box3D derives from Shape3D and represents a parallepiped in 3D.
 */
class ITK_EXPORT Box3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef Box3D   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Box3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    

  /**
   * Set Width
   */
  void SetWidth( double width );

  
  /**
   * Set Height
   */
  void SetHeight( double height);

 
  /**
   * Set Length
   */
  void SetLength( double length );


  /**
   * Return Width
   */
  double GetWidth(void) const;


  /**
   * Return Height
   */
  double GetHeight(void) const;

  /**
   * Return Length
   */
  double GetLength(void) const;


  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;

protected:
  /**
   * Constructor
   */
  Box3D();

  /**
   * Constructor with values
   */
  Box3D( double length, double width, double height );


  /**
   * Copy Constructor
   */
  Box3D( const Box3D & box );


  /**
   * Assignment Operator
   */
  const Box3D & operator=( const Box3D & box );


private:

  /**
   * Length of the Box, by default is aligned with X axis
   */
  double m_Length;
 
  /**
   * Width of the Box, by default is aligned with Y axis
   */
  double m_Width;

  /**
   * Width of the Box, by default is aligned with Z axis
   */
  double m_Height;

 
};


} // end namespace li


#endif
