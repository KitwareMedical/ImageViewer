/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ValidationInputParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ValidationInputParser_txx
#define _ValidationInputParser_txx

#include "ValidationInputParser.h"
#include "RawSliceVolumeReader.h"
#include "RawVolumeReader.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk
{

template <typename TImage, typename TLabelImage>
ValidationInputParser<TImage,TLabelImage>
::ValidationInputParser()
{
  m_ImageDirectoryName = "";
  m_BrainSegmentationDirectoryName = "";

  m_AtlasPatientID = "";
  m_SubjectPatientID = "" ;
  
  m_AtlasStartSliceNumber = 1;
  m_SubjectStartSliceNumber = 1;
  m_AtlasNumberOfSlices = 1;
  m_SubjectNumberOfSlices = 1;

  m_ParameterFileName = "";

  m_AtlasImage = NULL;
  m_SubjectImage = NULL;
  m_AtlasLabelImage = NULL;
  m_SubjectLabelImage = NULL;

  m_NumberOfHistogramLevels = 1024;
  m_NumberOfMatchPoints = 7;

  m_NumberOfLevels = 1;
  m_AtlasImageShrinkFactors.Fill( 1 );
  m_SubjectImageShrinkFactors.Fill( 1 );

  m_NumberOfIterations = IterationsArrayType(1);
  m_NumberOfIterations.Fill( 10 );
  
}


template <typename TImage, typename TLabelImage>
void
ValidationInputParser<TImage,TLabelImage>
::ReadImage(
const char * patientID,
signed long startSlice,
unsigned long numberOfSlices,
ImagePointer& image )
{

  std::string filePrefix;

  filePrefix = m_ImageDirectoryName;
  filePrefix += "/";
  filePrefix += patientID;
  filePrefix += "/";
  filePrefix += patientID;
  filePrefix += "_";
 

  typedef typename ImageType::PixelType PixelType;
  typedef RawSliceVolumeReader<PixelType,ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();


  // Defaults for IBSR data
  ReaderType::SpacingType spacing;
  ReaderType::SizeType    size;
  bool                    bigEndian = true;

  spacing[0] = 1.0; spacing[1] = 1.0; spacing[2] = 3.0;
  size[0] = 256; size[1] = 256; size[2] = numberOfSlices;

  reader->SetFilePrefix( filePrefix.c_str() );
  reader->SetFilePattern( "%d.img" );
  reader->SetStartSliceNumber( startSlice );
  reader->SetSize( size );
  reader->SetSpacing( spacing );
  reader->SetBigEndian( bigEndian );

  try
    {
    reader->Execute();
    }
  catch(...)
    {
    std::cout << "Error while reading in image for patient " << patientID << std::endl;
    throw;
    }

  image = reader->GetImage();


}


template <typename TImage, typename TLabelImage>
void
ValidationInputParser<TImage,TLabelImage>
::ReadLabelImage(
const char * patientID,
unsigned long numberOfSlices,
LabelImagePointer& image )
{

  std::string fileName;

  fileName = m_BrainSegmentationDirectoryName;
  fileName += "/";
  fileName += patientID;
  fileName += ".buchar";
 
  typedef typename LabelImageType::PixelType PixelType;
  typedef RawVolumeReader<PixelType,LabelImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();


  // Defaults for IBSR data
  ReaderType::SpacingType spacing;
  ReaderType::SizeType    size;
  bool                    bigEndian = true;

  spacing[0] = 1.0; spacing[1] = 1.0; spacing[2] = 3.0;
  size[0] = 256; size[1] = 256; size[2] = numberOfSlices;

  reader->SetFileName( fileName.c_str() );
  reader->SetSize( size );
  reader->SetSpacing( spacing );
  reader->SetBigEndian( bigEndian );

  try
    {
    reader->Execute();
    }
  catch(...)
    {
    std::cout << "Error while reading in image" << fileName.c_str() << std::endl;
    throw;
    }

  image = reader->GetImage();

}


template <typename TImage, typename TLabelImage>
void
ValidationInputParser<TImage,TLabelImage>
::Execute()
{

  /*************************
   * Read in the images
   *************************/
  this->ReadImage( m_AtlasPatientID.c_str(), m_AtlasStartSliceNumber, 
    m_AtlasNumberOfSlices, m_AtlasImage );
  this->ReadImage( m_SubjectPatientID.c_str(), m_SubjectStartSliceNumber, 
    m_SubjectNumberOfSlices, m_SubjectImage );

  this->ReadLabelImage( m_AtlasPatientID.c_str(), m_AtlasNumberOfSlices, 
    m_AtlasLabelImage );
  this->ReadLabelImage( m_SubjectPatientID.c_str(), m_SubjectNumberOfSlices, 
    m_SubjectLabelImage );


  /*************************
   * Read in algorithm parameters from file
   *************************/
  FILE * paramFile;
  paramFile = fopen( m_ParameterFileName.c_str(), "r" );
  if( !paramFile )
    {
    itkExceptionMacro( << "Could not open parameter file. " );
    }
    
  // parse the file
  unsigned int uNumber;

  // get number of histogram levels
  if( fscanf( paramFile, "%d", &uNumber) != 1 )
    {
    itkExceptionMacro( << "Could not find the number of histogram levels." );
    }
  m_NumberOfHistogramLevels = uNumber;


  // get number of match points
  if( fscanf( paramFile, "%d", &uNumber) != 1 )
    {
    itkExceptionMacro( << "Could not find the number of match points." );
    }
  m_NumberOfMatchPoints = uNumber;


  // get number of levels
  if( fscanf( paramFile, "%d", &uNumber) != 1 )
    {
    itkExceptionMacro( << "Could not find the number of levels." );
    }
  m_NumberOfLevels = uNumber;

  // get number of iterations
  {
  itk::Array<unsigned int> temp( m_NumberOfLevels );
  temp.Fill( 0 );
  m_NumberOfIterations = temp;
  }
  for( unsigned int j = 0; j < m_NumberOfLevels; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
     {
     itkExceptionMacro( << "Could not find number of iterations per level. " );
     }
    m_NumberOfIterations[j] = uNumber;
    }

  // get altas shrink factors
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
     {
     itkExceptionMacro( << "Could not find atlas starting shrink factor. " );
     }
    m_AtlasImageShrinkFactors[j] = uNumber;
    }

  // get subject shrink factors
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
     {
     itkExceptionMacro( << "Could not find subject starting shrink factor. " );
     }
    m_SubjectImageShrinkFactors[j] = uNumber;
    }


/*** FIXME: remove later ****/
#define PRINTOUT( name ) \
  std::cout << "" #name ": " << m_##name << std::endl;

  PRINTOUT( NumberOfHistogramLevels );
  PRINTOUT( NumberOfMatchPoints );
  PRINTOUT( NumberOfLevels );
  PRINTOUT( NumberOfIterations );
  PRINTOUT( AtlasImageShrinkFactors );
  PRINTOUT( SubjectImageShrinkFactors );

}


} // namespace itk

#endif
