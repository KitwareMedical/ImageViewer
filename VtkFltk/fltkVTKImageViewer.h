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
#include "itkVTKImageExport.h"
#include "vtkImageImport.h"
#include "vtkRenderWindow.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkImageMapper.h"


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
  typedef itk::VTKImageExport< ImageType >    VTKImageExportType;


  virtual void SetImage(ImageType * img);
  virtual void Show();
  virtual void Hide();
  
protected:
 
  VTKImageViewer();
  virtual ~VTKImageViewer();

private:

  VTKImageExportType::Pointer  m_VTKImageExport;

  vtkRenderWindow           *  m_RenderWindow;

  vtkImageImport            *  m_VTKImageImport;

  vtkImageMapper            *  m_ImageMapper;

  vtkActor                  *  m_Actor;

  vtkRenderer               *  m_Renderer;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkVTKImageViewer.txx>
#endif




#endif
