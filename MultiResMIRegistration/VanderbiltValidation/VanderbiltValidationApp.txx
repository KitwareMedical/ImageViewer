/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltValidationApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltValidationApp_txx
#define _VanderbiltValidationApp_txx

#include "VanderbiltValidationApp.h"

namespace itk
{

template <typename TInputPixel, typename TInputImage, typename TImage>
VanderbiltValidationApp<TInputPixel,TInputImage,TImage>
::VanderbiltValidationApp()
{ 
  m_ImageDirectoryName = "";
  m_PatientNumber = 0;
  m_FromModality = VanderbiltModality::CT;
  m_ToModality = VanderbiltModality::CT;
  m_ParameterFileName = "";
  m_InvertTransform = false;
  m_OutputFileName = "";
  m_AppendOutputFile = false;
  m_InvestigatorString = "";
  m_SiteString = ""; 
  m_MethodString = "";
}


template <typename TInputPixel, typename TInputImage, typename TImage>
void
VanderbiltValidationApp<TInputPixel,TInputImage,TImage>
::InitializeParser()
{
  m_Parser->SetImageDirectoryName( m_ImageDirectoryName.c_str() );
  m_Parser->SetPatientNumber( m_PatientNumber );
  m_Parser->SetFromModality( m_FromModality );
  m_Parser->SetToModality( m_ToModality );
  m_Parser->SetParameterFileName( m_ParameterFileName.c_str() );
}


template <typename TInputPixel, typename TInputImage, typename TImage>
void
VanderbiltValidationApp<TInputPixel,TInputImage,TImage>
::InitializeGenerator()
{
  m_Generator->SetMovingImage( m_Parser->GetMovingImage() );
  m_Generator->SetFixedImage( m_Parser->GetFixedImage() );
  m_Generator->SetTransform( m_Transform );
  m_Generator->SetInvertTransform( m_InvertTransform );

  m_Generator->SetOutputFileName( m_OutputFileName.c_str() );
  m_Generator->SetAppendOutputFile( m_AppendOutputFile );

  m_Generator->SetPatientNumber( m_PatientNumber );
  m_Generator->SetFromModality( m_FromModality );
  m_Generator->SetToModality( m_ToModality );

  m_Generator->SetInvestigatorString1( m_InvestigatorString.c_str() );
  m_Generator->SetSiteString1( m_SiteString.c_str() );
  m_Generator->SetMethodString( m_MethodString.c_str() );

  char buffer[20];
  time_t currentTime;
  struct tm *now;
  ::time( &currentTime );
  now = localtime( &currentTime );
  ::strftime( buffer, 20, "%d %B %Y", now );
  m_Generator->SetDateString( buffer );
}


} // namespace itk

#endif
