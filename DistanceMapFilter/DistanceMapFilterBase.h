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
#include "fltkImageViewer.h"
#include "fltkClippingPlane3DDrawer.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCommand.h"
#include "itkDanielssonDistanceMapImageFilter.h"

class DistanceMapFilterBase : public itk::Object 
{
public:
  /** Standard class typedefs. */
  typedef DistanceMapFilterBase      Self;
  typedef itk::Object  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DistanceMapFilterBase, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Image Dimension */
  enum { ImageDimension = 3 };

  /** Convenient typedefs. */
  typedef itk::Image<unsigned short,ImageDimension>    ImageType;
  typedef ImageType::Pointer                           ImagePointer;
  typedef itk::Image<unsigned short, ImageDimension>   DistanceImageType;
  typedef DistanceImageType::Pointer                   DistanceImagePointer;
  typedef itk::DanielssonDistanceMapImageFilter<ImageType,DistanceImageType> 
          DistanceFilterType;
  typedef DistanceFilterType::Pointer                  DistanceFilterPointer; 
  typedef itk::ImageFileReader< ImageType>             VolumeReaderType;
  typedef VolumeReaderType::Pointer                    VolumeReaderPointer;

  /** Slice Drawer for Input Image : Orthogonal slices in OpenGL */
  typedef fltk::Slice3DDrawer<ImageType>         ImageSliceDrawerType;
  typedef ImageSliceDrawerType::Pointer          ImageSliceDrawerPointer;

  /** Slice Drawer for Distance Image  : Orthogonal slices in OpenGL */
  typedef fltk::Slice3DDrawer<DistanceImageType> DistanceImageSliceDrawerType;
  typedef DistanceImageSliceDrawerType::Pointer DistanceImageSliceDrawerPointer;
  typedef unsigned char      OverlayPixelType;

  /** Image viewer using in-plane slice by slice */ 
  typedef fltk::ImageViewer<ImageType::PixelType,OverlayPixelType>
          ImageViewerType;
  typedef fltk::ImageViewer<DistanceImageType::PixelType,OverlayPixelType>
          DistanceImageViewerType;

public:
  virtual void Load(void);
  virtual void Save(void) const;
  virtual void Load(const char * filename);
  virtual void Save(const char * filename) const;
  virtual void ShowDisplay(void);
  virtual void HideDisplay(void);
  virtual void ComputeDistance(void);

protected:
  DistanceMapFilterBase();
  virtual ~DistanceMapFilterBase();

  VolumeReaderPointer                 m_Reader;
  DistanceImagePointer                m_DistanceImage;
  DistanceFilterPointer               m_DistanceFilter;
  ImageSliceDrawerPointer             m_ImageSliceDrawer;
  DistanceImageSliceDrawerPointer     m_DistanceImageSliceDrawer;
  ImageViewerType                   * m_ImageViewer;
  DistanceImageViewerType           * m_DistanceImageViewer;
  fltkDisplayGlWindowGUI              m_Display;
  bool    ImageHasBeenLoaded( void ) { return m_ImageLoaded; }

private:
  DistanceMapFilterBase( const DistanceMapFilterBase & ); // don't implement
  DistanceMapFilterBase operator=( const DistanceMapFilterBase & );

  bool  m_ImageLoaded;

};


  


#endif


