#ifndef __SurfaceGeneratorBase_H
#define __SurfaceGeneratorBase_H

#include "itkMesh.h"
#include "itkDefaultDynamicMeshTraits.h"
#include "CriticalPoint.h"
#include "fltkDisplayGlWindowGUI.h"
#include "fltkSlice3DDrawer.h"
#include "fltkClippingPlane3DDrawer.h"
#include "itkObject.h"
#include "itkCommand.h"




class SurfaceGeneratorBase : public itk::Object {

public:
  /**
   * Standard "Self" typedef.
   */
  typedef SurfaceGeneratorBase      Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Object  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(SurfaceGeneratorBase, itk::Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  


  /**
   * Traits for the Mesh class
   */
   typedef itk::DefaultDynamicMeshTraits<
                                    CriticalPoint,
                                    3,
                                    2,
                                    double, 
                                    double,
                                    char
                                      >  MeshTraitType;

  /**
   * Mesh class type
   */
   typedef itk::Mesh< CriticalPoint, 3, MeshTraitType > MeshType;

   typedef MeshType::Pointer                  MeshPointer;
   typedef MeshType::PointType                PointType;

   typedef MeshType::PointsContainer          PointsContainer;
   typedef MeshType::PointDataContainer       PointDataContainer;
   typedef PointDataContainer::Iterator        PointDataIterator;
   typedef PointDataContainer::ConstIterator  PointDataConstIterator;

   typedef CriticalPoint::IdentifierType    IdentifierType;

  /**
   * Command that will draw the object
   */
  typedef itk::SimpleConstMemberCommand< Self >   DrawCommandType;
  typedef DrawCommandType::Pointer                DrawCommandPointer;


protected:

  SurfaceGeneratorBase();
  virtual ~SurfaceGeneratorBase();

  SurfaceGeneratorBase( const SurfaceGeneratorBase & );           // not defined 
  SurfaceGeneratorBase operator=( const SurfaceGeneratorBase & ); // not defined 

public:

  virtual void Load(const char * filename);
  virtual void Save(void) const;
  virtual void ShowDisplay(void);
  virtual void HideDisplay(void);
  virtual void Draw(void) const;
  virtual void GenerateEllipsoid(double a, double b, double c);
  virtual void GeneratePatch(void);

  /**
   * Get the Observer/Command that will redraw the object
   */
  DrawCommandPointer GetDrawCommand(void);


protected:

  MeshPointer                         m_Mesh;

  fltkDisplayGlWindowGUI              m_Display;


private:
  
  DrawCommandPointer                  m_DrawCommand;

  mutable GLuint                      m_DisplayList;

};


  


#endif


