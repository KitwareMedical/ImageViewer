/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkVTKImageViewer.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkVTKImageViewer.h"

#include "vtkAxes.h"
#include "vtkTubeFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"



namespace fltk {
  
template <class ImagePixelType>
VTKImageViewer<ImagePixelType>
::VTKImageViewer()
{
  m_VTKImageExport = VTKImageExportType::New();
  m_VTKImageImport = vtkImageImport::New();

  m_RenderWindow   = vtkRenderWindow::New();
  m_Actor          = vtkActor::New();
  m_ImageMapper    = vtkImageMapper::New();
  m_Renderer       = vtkRenderer::New();

  fltkRenderWindowInteractor->SetRenderWindow( m_RenderWindow );

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_Renderer->SetBackground( 1.0, 1.0, 1.0 ); 
  m_Renderer->GetActiveCamera()->Zoom( 1.0 ); 
  m_Renderer->GetActiveCamera()->SetPosition(0.0, 0.0, 20.0 ); 


  vtkAxes *axes = vtkAxes::New();
    axes->SetOrigin( 0.0, 0.0, 0.0 );
    axes->SetScaleFactor( 1.0 );

  vtkTubeFilter *axesTubes = vtkTubeFilter::New();
    axesTubes->SetInput(axes->GetOutput());
    axesTubes->SetRadius(axes->GetScaleFactor()/100.0);
    axesTubes->SetNumberOfSides(6);

  vtkPolyDataMapper *axesMapper = vtkPolyDataMapper::New();
    axesMapper->SetInput(axesTubes->GetOutput());

  vtkActor *axesActor = vtkActor::New();
    axesActor->SetMapper(axesMapper);

  m_Renderer->AddActor( axesActor );

//  axesMapper->Delete();
//  axesTubes->Delete();
//  axes->Delete();


}


  
template <class ImagePixelType>
VTKImageViewer<ImagePixelType>
::~VTKImageViewer()
{
  // Delete only VTK objects
  if( m_VTKImageImport )
    {
    m_VTKImageImport->Delete();
    }

  if( m_ImageMapper )
    {
    m_ImageMapper->Delete();
    }

  if( m_Actor )
    {
    m_Actor->Delete();
    }

  if( m_RenderWindow )
    {
    m_RenderWindow->Delete();
    }

  if( m_Renderer )
    {
    m_Renderer->Delete();
    }

}



template <class ImagePixelType>
void
VTKImageViewer<ImagePixelType>
::SetImage(ImageType * img)
{
  m_VTKImageExport->SetInput( img );
}




template <class ImagePixelType>
void 
VTKImageViewer<ImagePixelType>
::Show(void)
{
  Superclass::Show(); 
}





template <class ImagePixelType>
void 
VTKImageViewer<ImagePixelType>
::Hide(void)
{
  Superclass::Hide();
}



} // end namespace itk
