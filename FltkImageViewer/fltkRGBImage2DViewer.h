/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkRGBImage2DViewer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkRGBImage2DViewer_h
#define __fltkRGBImage2DViewer_h

#include "itkObject.h"
#include "itkImage.h"
#include "fltkRGBImage2DViewerGUI.h"
#include "itkCommand.h"

namespace fltk {

template <class ImagePixelType>
class RGBImage2DViewer : public itk::Object, public fltkRGBImage2DViewerGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef RGBImage2DViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(RGBImage2DViewer, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Image Type
   */
  typedef itk::Image< ImagePixelType, 2 >   ImageType;
  
  /**
   * Chanel Image Type
   */
  typedef unsigned char ChannelPixelType ;
  typedef itk::Image< ChannelPixelType > ChannelImageType ;
  
  /**
   * Observer Command
   */
  typedef itk::SimpleMemberCommand< Self >      ObserverCommandType;


  /**
   * Observer Command
   */
  virtual void SetImage(ImageType * image);

  virtual void SetRedChannel(ChannelImageType* image) ;
  virtual void SetGreenChannel(ChannelImageType* image) ;
  virtual void SetBlueChannel(ChannelImageType* image) ;

  virtual void FillChannel(int channel, ChannelImageType* image) ;
  virtual void FillChannel(int channel, ChannelPixelType value, 
                           int sizeX, int sizeY) ;
  /**
   * Update the image rendering
   */
  virtual void Update(void);

  /**
   * Return the notifier of the Window, in order
   * to register additional drawers
   */
  itk::Object * GetNotifier(void);

  /**
   * Return the Command responsible for redrawing
   * the window. This command is intended to be registered
   * as observer of the objects whose changes will modify 
   * the display of the image.
   */
  itkGetObjectMacro( RedrawCommand, ObserverCommandType );

  itkSetMacro(FlipY, bool) ;

protected:
  
  RGBImage2DViewer();
  virtual ~RGBImage2DViewer();


private:
  typename ImageType::Pointer              m_Image;
  typename ObserverCommandType::Pointer    m_RedrawCommand; 
  unsigned long                            m_Tag;

  ImageType::RegionType m_ImageRegion ;
  ImageType::SizeType m_ImageSize ;
  
  ChannelImageType::Pointer m_RedImage ;
  ChannelImageType::Pointer m_GreenImage ;
  ChannelImageType::Pointer m_BlueImage ;

  bool m_FlipY ;
};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkRGBImage2DViewer.txx>
#endif




#endif
