
#ifndef __DistanceMapFilter_H
#define __DistanceMapFilter_H


#include "DistanceMapFilterGUI.h"


class DistanceMapFilter : public DistanceMapFilterGUI {


public:
  /**
   * Standard "Self" typedef.
   */
  typedef DistanceMapFilter      Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef DistanceMapFilterGUI  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(DistanceMapFilter, DistanceMapFilterGUI);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

  /**
   * Image types
   */
  typedef DistanceMapFilterBase::ImageType            ImageType;
  typedef DistanceMapFilterBase::DistanceImageType    DistanceImageType;


  virtual void ComputeDistance(void);

protected:

  DistanceMapFilter();
  virtual ~DistanceMapFilter();

  DistanceMapFilter( const DistanceMapFilter & );           // not defined 
  DistanceMapFilter operator=( const DistanceMapFilter & ); // not defined
  

};


  


#endif


