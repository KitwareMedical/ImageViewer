/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkVTKImageViewer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkVTKImageViewer_h
#define __fltkVTKImageViewer_h

#include "fltkVTKImageViewerGUI.h"

#include "itkImageToVTKImageFilter.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"


namespace fltk {

template <class ImagePixelType >
class VTKImageViewer : public fltkVTKImageViewerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef VTKImageViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef itk::Image< ImagePixelType, 3 >   ImageType;
 

  typedef fltkVTKImageViewerGUI     Superclass;
  
  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(VTKImageViewerBase, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

  /** Filter for connecting and end of the ITK pipeline 
      to the beggining of a VTK pipeline */
  typedef ::itk::ImageToVTKImageFilter< ImageType >   AdaptorFilterType;
  typedef typename AdaptorFilterType::Pointer          AdaptorFilterPointer;


  virtual void SetImage(ImageType * img);
  virtual void Show();
  virtual void Hide();
  
protected:
 
  VTKImageViewer();
  virtual ~VTKImageViewer();

private:

  vtkRenderWindow           *  m_RenderWindow;
  vtkRenderer               *  m_Renderer;

  AdaptorFilterPointer         m_AdaptorFilter;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkVTKImageViewer.txx>
#endif




#endif
