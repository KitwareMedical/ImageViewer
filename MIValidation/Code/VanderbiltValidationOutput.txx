/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltValidationOutput.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltValidationOutput_txx
#define _VanderbiltValidationOutput_txx

#include "VanderbiltValidationOutput.h"

#include "itkExceptionObject.h"

namespace itk
{

template <typename TImage>
VanderbiltValidationOutput<TImage>
::VanderbiltValidationOutput()
{
  m_MovingImage = NULL;
  m_FixedImage = NULL;
  m_Transform = NULL;
  m_InvertTransform = false;

  m_OutputFileName = "";
  m_AppendOutputFile = false;

  m_InvestigatorString1 = "";
  m_InvestigatorString2 = "";
  m_SiteString1 = "";
  m_SiteString2 = "";
  m_MethodString = "";
  m_DateString = "";

  m_PatientNumber = 0;
  m_FromModality = VanderbiltModality::CT;
  m_ToModality = VanderbiltModality::CT;


}


template <typename TImage>
void
VanderbiltValidationOutput<TImage>
::Execute()
{

  if ( !m_MovingImage || !m_FixedImage || 
       !m_Transform || m_OutputFileName == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  FILE * file;
  
  if ( m_AppendOutputFile )
    {
    file = fopen( m_OutputFileName.c_str(), "a" );
    }
  else
    {
    file = fopen( m_OutputFileName.c_str(), "w" );
    }

  if ( !file )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Can't open output file." );
    throw err;
    }

   // write out the header

    char patientID[12] = "practice";
    if ( m_PatientNumber > 0 )
    {
    sprintf( patientID,"pt_%03d", m_PatientNumber );
    }

    fprintf(file, "-------------------------------------------------------------------------\n");
    fprintf(file, "Transformation Parameters\n\n");
    fprintf(file, "Investigator(s): %s\n",m_InvestigatorString1.c_str());
    fprintf(file, "                 %s\n",m_InvestigatorString2.c_str());
    fprintf(file, "Site: %s\n",m_SiteString1.c_str());
    fprintf(file, "      %s\n",m_SiteString2.c_str());
    fprintf(file, "Method: %s\n",m_MethodString.c_str());
    fprintf(file, "Date: %s\n",m_DateString.c_str());
    fprintf(file, "Patient number: %s\n", patientID);

    if ( !m_InvertTransform )
      {
      fprintf(file, "From: %s\n",VanderbiltModality::Description[m_FromModality]);
      fprintf(file, "To: %s\n\n",VanderbiltModality::Description[m_ToModality]);
      }
    else
      {
      fprintf(file, "From: %s\n",VanderbiltModality::Description[m_ToModality]);
      fprintf(file, "To: %s\n\n",VanderbiltModality::Description[m_FromModality]);
      }

    fprintf(file, "Point      x          y          z        new_x       new_y       new_z\n\n");


  AffineTransformPointer invTransform;
  if ( !m_InvertTransform )
    {
      // need to invert the input transform
      invTransform = m_Transform->Inverse();
    }
  else
    { 
      invTransform = m_Transform;
    }

  // write out the transform table
  typedef typename  AffineTransformType::InputPointType PointType;
  unsigned int numberOfCorners = 1 << ImageDimension;

  typedef typename ImageType::IndexType IndexType;
  typedef typename IndexType::IndexValueType IndexValueType;
  typedef typename ImageType::SizeType SizeType;

  typename ImageType::Pointer image;
  if ( !m_InvertTransform )
    {
    image = m_MovingImage;
    }
  else
    { 
    image = m_FixedImage;
    }

  IndexType startIndex = image->GetLargestPossibleRegion().GetIndex();
  SizeType  size = image->GetLargestPossibleRegion().GetSize();

  for ( unsigned int k = 0; k < numberOfCorners; k++ )
    {
    unsigned int upper = k;
    unsigned int j;

    ContinuousIndex<double,ImageDimension> contIndex;
    for( j = 0; j < ImageDimension; j++ )
      {
      if ( upper & 1 )
        {
        contIndex[j] = startIndex[j] + 
          static_cast<IndexValueType>( size[j] ) - 1;
        }
      else
        {
        contIndex[j] = startIndex[j];
        }
      upper >>= 1;
      }

    PointType point;
    image->TransformContinuousIndexToPhysicalPoint(
      contIndex, point );

    PointType newPoint = invTransform->TransformPoint( point );

    fprintf( file, "%3d   ", k + 1 );
    for ( j = 0; j < ImageDimension; j++ )
      {
      fprintf( file, "%10.4f ", point[j] );
      }
    for ( j = 0; j < ImageDimension; j++ )
      {
      fprintf( file, "%10.4f ", newPoint[j] );
      }
    fprintf( file, "\n" );

    }

    fprintf(file, "\n");
    fprintf(file, "(All distances are in millimeters.)\n");
    fprintf(file, "-------------------------------------------------------------------------\n");
    fprintf(file, "\n");

  fclose( file );

}


} // namespace itk

#endif
