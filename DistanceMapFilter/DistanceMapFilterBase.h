/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DistanceMapFilterBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __DistanceMapFilterBase_H
#define __DistanceMapFilterBase_H

#include "fltkDisplayGlWindowGUI.h"
#include "fltkSlice3DDrawer.h"
#include "fltkVectorImage3D.h"
#include "fltkClippingPlane3DDrawer.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCommand.h"
#include "itkDanielssonDistanceMapImageFilter.h"




class DistanceMapFilterBase : public itk::Object {

public:
  /**
   * Standard "Self" typedef.
   */
  typedef DistanceMapFilterBase      Self;

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
  itkTypeMacro(DistanceMapFilterBase, itk::Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

  /**
   * Image Dimension
   */
   enum { ImageDimension = 3 };

  /**
   * Input Image Type
   */
   typedef itk::Image< unsigned short, ImageDimension >  ImageType;
   typedef ImageType::Pointer                            ImagePointer;


  /**
   * Distance Image Type
   */
  typedef itk::Image<unsigned short, ImageDimension>   DistanceImageType;
  typedef DistanceImageType::Pointer                   DistanceImagePointer;
   

  /**
   *  Gradient filter type
   */
  typedef itk::DanielssonDistanceMapImageFilter< 
                                    ImageType,
                                    DistanceImageType >  DistanceFilterType;
   
  typedef DistanceFilterType::Pointer                    DistanceFilterPointer; 


  typedef   itk::ImageFileReader< ImageType >            VolumeReaderType;
  typedef   VolumeReaderType::Pointer                    VolumeReaderPointer;


  /** Slice Drawer   */
  typedef fltk::Slice3DDrawer< ImageType >               ImageSliceDrawerType;
  typedef ImageSliceDrawerType::Pointer                  ImageSliceDrawerPointer;

  

protected:

  DistanceMapFilterBase();
  virtual ~DistanceMapFilterBase();

  DistanceMapFilterBase( const DistanceMapFilterBase & ); // should not be defined 
  DistanceMapFilterBase operator=( const DistanceMapFilterBase & ); // should not be defined 

public:

  virtual void Load(void);
  virtual void Save(void) const;
  virtual void Load(const char * filename);
  virtual void Save(const char * filename) const;
  virtual void ShowDisplay(void);
  virtual void HideDisplay(void);
  virtual void ComputeDistance(void);


protected:

  VolumeReaderPointer                 m_Reader;
  DistanceImagePointer                m_DistanceImage;
  DistanceFilterPointer               m_DistanceFilter;

  ImageSliceDrawerPointer             m_ImageSliceDrawer;

  fltkDisplayGlWindowGUI              m_Display;


private:
  
  bool                                m_ImageLoaded;

};


  


#endif


