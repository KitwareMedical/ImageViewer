
#ifndef __SurfaceGenerator_H
#define __SurfaceGenerator_H


#include "SurfaceGeneratorGUI.h"


class SurfaceGenerator : public SurfaceGeneratorGUI {


public:
  /**
   * Standard "Self" typedef.
   */
  typedef SurfaceGenerator      Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef SurfaceGeneratorGUI  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(SurfaceGenerator, SurfaceGeneratorGUI);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

  /**
   * Mesh class type
   */
  typedef SurfaceGeneratorBase::MeshType MeshType;
  typedef MeshType::Pointer     MeshPointer;


protected:

  SurfaceGenerator();
  virtual ~SurfaceGenerator();

  void Load();

  SurfaceGenerator( const SurfaceGenerator & );           // not defined 
  SurfaceGenerator operator=( const SurfaceGenerator & ); // not defined
  
  void glDrawEllipsoid();


};


  


#endif


