/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVTKImageToImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkVTKImageToImageFilter_txx
#define _itkVTKImageToImageFilter_txx

#include "itkVTKImageToImageFilter.h"

namespace itk
{



/**
 * Constructor
 */
template <class TInputImage>
VTKImageToImageFilter<TInputImage>
::VTKImageToImageFilter()
{

  m_Exporter = vtkImageExport::New();

  m_Importer = ImporterFilterType::New();

  m_Importer->SetUpdateInformationCallback( m_Exporter->GetUpdateInformationCallback());
  m_Importer->SetPipelineModifiedCallback( m_Exporter->GetPipelineModifiedCallback());
  m_Importer->SetWholeExtentCallback( m_Exporter->GetWholeExtentCallback());
  m_Importer->SetSpacingCallback( m_Exporter->GetSpacingCallback());
  m_Importer->SetOriginCallback( m_Exporter->GetOriginCallback());
  m_Importer->SetScalarTypeCallback( m_Exporter->GetScalarTypeCallback());
  m_Importer->SetNumberOfComponentsCallback( m_Exporter->GetNumberOfComponentsCallback());
  m_Importer->SetPropagateUpdateExtentCallback( m_Exporter->GetPropagateUpdateExtentCallback());
  m_Importer->SetUpdateDataCallback( m_Exporter->GetUpdateDataCallback());
  m_Importer->SetDataExtentCallback( m_Exporter->GetDataExtentCallback());
  m_Importer->SetBufferPointerCallback( m_Exporter->GetBufferPointerCallback());
  m_Importer->SetCallbackUserData( m_Exporter->GetCallbackUserData());

}




/**
 * Destructor
 */
template <class TInputImage>
VTKImageToImageFilter<TInputImage>
::~VTKImageToImageFilter()
{
}



/**
 * Set an itk::Image as input 
 */
template <class TInputImage>
void
VTKImageToImageFilter<TInputImage>
::SetInput( const InputImageType * inputImage )
{
  m_Exporter->SetInput( inputImage );
}



/**
 * Get a vtkImage as output
 */
template <class TInputImage>
vtkImageData *
VTKImageToImageFilter<TInputImage>
::GetOutput() const
{
  return m_Importer->GetOutput();
}




/**
 * Get the importer filter
 */
template <class TInputImage>
vtkImageImport *
VTKImageToImageFilter<TInputImage>
::GetImporter() const
{
  return m_Importer;
}




} // end namespace itk

#endif

