/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMValidationInputParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMValidationInputParser_txx
#define _FEMValidationInputParser_txx

#include "FEMValidationInputParser.h"
#include "RawSliceVolumeReader.h"
#include "RawVolumeReader.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk
{

template <typename TImage, typename TLabelImage>
FEMValidationInputParser<TImage,TLabelImage>
::FEMValidationInputParser()
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
  
}


template <typename TImage, typename TLabelImage>
void
FEMValidationInputParser<TImage,TLabelImage>
::ReadImage(
const char * patientID,
signed long startSlice,
unsigned long numberOfSlices,
ImagePointer& image )
{

  std::string filePrefix;

  filePrefix = m_ImageDirectoryName;
  filePrefix += "/whead";
  filePrefix += patientID;
  filePrefix += ".img";

  typedef typename ImageType::PixelType PixelType;

  typedef  itk::ImageFileReader< ImageType >      FileSourceType;

  const unsigned int ImageDimension=ImageType::ImageDimension;
  typedef  itk::RawImageIO< PixelType,ImageDimension>   RawReaderType;

  FileSourceType::Pointer reffilter = FileSourceType::New();

  RawReaderType::Pointer  rawReader  = RawReaderType::New();
  rawReader->SetFileDimensionality( ImageDimension );
  
  rawReader->SetDimensions( 0,  m_ImageXSize );
  rawReader->SetDimensions( 1,  m_ImageYSize );
  rawReader->SetDimensions( 2, numberOfSlices );
  
  // Defaults for IBSR data
//  RawReaderType::SpacingType spacing;
  bool                    bigEndian = true;
// HERE ASSUMING THE IMAGES HAVE BEEN RESAMPLED TO BE ISOTROPIC
//  spacing[0] = 1.0; spacing[1] = 1.0; spacing[2] = 1.0; 
//  rawReader->SetSpacing( spacing );
//  rawReader->SetBigEndian( bigEndian );

  reffilter->SetImageIO( rawReader );

  reffilter->SetFileName(filePrefix.c_str() );

  try
    {
    reffilter->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during reference file reading " << std::endl;
    std::cerr << e << std::endl;
    return ;
    }



  image = reffilter->GetOutput();


}


template <typename TImage, typename TLabelImage>
void
FEMValidationInputParser<TImage,TLabelImage>
::ReadLabelImage(
const char * patientID,
unsigned long numberOfSlices,
LabelImagePointer& image )
{

  std::string fileName;

  fileName = m_BrainSegmentationDirectoryName;
  fileName += "/brain";
  fileName += patientID;
  fileName += ".img";
  typedef typename LabelImageType::PixelType PixelType;

  typedef  itk::ImageFileReader< LabelImageType >      FileSourceType;

  const unsigned int ImageDimension=ImageType::ImageDimension;
  typedef  itk::RawImageIO< PixelType,ImageDimension>   RawReaderType;

  FileSourceType::Pointer reffilter = FileSourceType::New();

  RawReaderType::Pointer  rawReader  = RawReaderType::New();
  rawReader->SetFileDimensionality( ImageDimension );
  
  rawReader->SetDimensions( 0, m_ImageXSize );
  rawReader->SetDimensions( 1, m_ImageYSize );
  rawReader->SetDimensions( 2, numberOfSlices );
  
  // Defaults for IBSR data
//  RawReaderType::SpacingType spacing;
  bool                    bigEndian = true;
// HERE ASSUMING THE IMAGES HAVE BEEN RESAMPLED TO BE ISOTROPIC
//  spacing[0] = 1.0; spacing[1] = 1.0; spacing[2] = 1.0; 
//  rawReader->SetSpacing( spacing );
//  rawReader->SetBigEndian( bigEndian );

  reffilter->SetImageIO( rawReader );

  reffilter->SetFileName(fileName.c_str() );

  try
    {
    reffilter->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during reference file reading " << std::endl;
    std::cerr << e << std::endl;
    return ;
    }



  image = reffilter->GetOutput();
 

}


template <typename TImage, typename TLabelImage>
void
FEMValidationInputParser<TImage,TLabelImage>
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
/*  FILE * paramFile;
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

*/

/*** FIXME: remove later ****/
#define PRINTOUT( name ) \
  std::cout << "" #name ": " << m_##name << std::endl;


}


} // namespace itk

#endif
