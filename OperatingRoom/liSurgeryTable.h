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

#ifndef liSURGERYTABLECLASS
#define liSURGERYTABLECLASS

#include "fltkBox3D.h"


namespace li {


/**
 *  SurgeryTable implements a representation of the table in
 *  an operating room. The table can be adjusted in height,
 *  and can be displaced laterally and in cranio-caudal 
 *  direction. All dimensions are assumed to be in millimeters.
 *
 */
class ITK_EXPORT SurgeryTable: public fltk::Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef SurgeryTable   Self;

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
  itkTypeMacro( SurgeryTable, fltk::Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  /**
   * Destructor
   */
  ~SurgeryTable();

  /**
   * DrawGeometry implements the drawing of the table.
   * this method is called by liShape3D in its Draw method.
   */
  void DrawGeometry(void) const;

  /**
   * Set the Axial position of the bed (Cranio-Caudal) direction.
   */
  void SetAxial(double newaxial);

  /**
   * Set the Vertical position of the bed.
   */
  void SetVertical(double newvertical);

  /**
   * Set the Lateral displacement of the bed.
   */
  void SetLateral(double newlateral);

  /**
   * Get the Axial position of the bed (Cranio-Caudal) direction.
   */
  double GetAxial( void ) const;

  /**
   * Get the Vertical position of the bed.
   */
  double GetVertical( void ) const;

  /**
   * Get the Lateral displacement of the bed.
   */
  double GetLateral( void ) const;


protected:

  /**
   * Constructor
   */
  SurgeryTable();


private:
  
  /**
   * Lateral displacement of the bed. Dimensions in millimeters
   */
  double     m_Lateral;

  /**
   * Axial displacement of the bed, Cranio-Caudal direction. 
   * Dimensions in millimeters
   */
  double     m_Axial;

  /**
   * Vertical displacement of the bed. Dimensions in millimeters
   */
  double     m_Vertical;
  
  /**
   * Block in contact with the floor
   */
  fltk::Box3D::Pointer    m_Base;

  /**
   * Block on top of the base, in which the hydraulic 
   * system is supposed to be held 
   */
  fltk::Box3D::Pointer    m_FixedPile;

  /**
   * Sliding Block that defines the height of the bed.
   * it is attached to the mobile part of the hydraulic
   * system.
   */
  fltk::Box3D::Pointer    m_Pile;

  /**
   * Flat surface that supports the bed.
   * It is attached to the Pile. Lateral displacements
   * affect this piece.
   */
  fltk::Box3D::Pointer    m_BedSupport;

  /**
   * Flat surface in which the patient lies.
   * Axial displacements move the Bed along the BedSupport.
   */
  fltk::Box3D::Pointer    m_Bed;


};

} // end namespace li

#endif




