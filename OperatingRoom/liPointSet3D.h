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


#ifndef liPOINTSET3DCLASS
#define liPOINTSET3DCLASS

#include <itkPointSet.h>
#include <liShape3D.h>

namespace li {


/**
 *  PointSet3D derives from Shape3D and contains an itk::PointSet
 */
class ITK_EXPORT PointSet3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef PointSet3D   Self;

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
  itkTypeMacro( PointSet3D, Shape3D );
  

  /** 
   * PointSet Type Defaults
   */
  typedef ::itk::DefaultStaticMeshTraits<
                      double,
                      3,3,
                      double,
                      double> PointSetDefaultType;


  /** 
   * PointSet Type
   */
   typedef ::itk::PointSet<double,3,PointSetDefaultType>   PointSetType;


  /** 
   * Points Container Type
   */
  typedef PointSetType::PointsContainer     PointsContainerType;

  /** 
   * Point Data Container Type
   */
  typedef PointSetType::PointDataContainer     PointDataContainerType;


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    

  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



  /**
   * Accepts a mesh
   */
  void SetPointSet( PointSetType * );



  /**
   * Return a mesh
   */
  PointSetType::ConstPointer GetPointSet( void );


protected:
  /**
   * Constructor
   */
  PointSet3D();

  
private:

   /**
   * PointSet that contains all the data
   */
   PointSetType::Pointer                  m_PointSet;

};


} // end namespace li


#endif
