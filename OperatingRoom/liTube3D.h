//--------------------------------------------
//
//     Project Angio Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------


#ifndef liTUBE3DCLASS
#define liTUBE3DCLASS

#include <fstream>
#include <liShape3D.h>
#include <itkVector.h>
#include <liPointSet3D.h>

namespace li {

/**
 * 
 * Tube3D is the basic class to model a tube of variable
 * diameter. It is modeled as a set of points in the medial
 * axis of the tube, along with estimated radius at these
 * positions.
 *
 */ 

class ITK_EXPORT Tube3D : public Shape3D 
{

public:

  /**
   * Standard "Self" typedef.
   */
  typedef Tube3D   Self;

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
  itkTypeMacro( Tube3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  unsigned int GetNumberOfSectors(void)  { return m_NumberOfSectors;  };
  unsigned int GetNumberOfSections(void) { return m_NumberOfSections; };

  void SetNumberOfSectors(unsigned int val)  {   m_NumberOfSectors = val; }
  void SetNumberOfSections(unsigned int val) {  m_NumberOfSections = val; }
 
  /**
   * typedef of the Point type
   */
  typedef   itk::Point<double,3>    PointType;
    
  /**
   * typedef of the Vector type
   */
  typedef   itk::Vector<double,3>    VectorType;

  /**
   * typedef of the PointSet type
   */
  typedef   PointSet3D::PointSetType   PointSetType;
  

	bool Null(void) const;
	void DrawGeometry(void) const;
	int  Read(Shape3D::IfstreamType & is);
	int  Write(const char *filename) const;
	int  GenerateSkin(void);
	void GeneratePointSet(void);
  void GeneratePointSet( const VectorType & sight ); 

  void ScalePoints(double sx,double sy, double sz);

  
	void SetRadius( float radius );

	PointType & GetMedial( unsigned int i ) const { return m_Medial[i]; }
	PointType & GetVertex( unsigned int i ) const { return m_Vertex[i]; }

	float & GetRadius( unsigned int i ) { return m_Radius[i]; }

	void Smooth(void);

  void SubsampleUsingRadius(void);

	void Copy(const Tube3D & sm );

	bool Alloc(unsigned int nsect, unsigned int nsecc);

  PointSetType::ConstPointer GetPointSet( void ) const
            { return m_PointSet.GetPointer(); }

protected:

	Tube3D();
	~Tube3D();
	void Clear(void);
  
private:

	unsigned int	m_NumberOfSectors;	  // Number of radial sectors
	unsigned int	m_NumberOfSections; 	// Number of rectilinear sections
	float        *m_Radius;	            // Array of radius
	PointType    *m_Vertex;	            // Array of vertex coordinates
	PointType    *m_Medial;	            // Array of medial Point3Ds

  PointSetType::Pointer    m_PointSet; // Point Set

};

} // end namespace li

#endif

