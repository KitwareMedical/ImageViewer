/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ClassifierValidationInputParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ClassifierValidationInputParser_txx
#define _ClassifierValidationInputParser_txx

#include "ClassifierValidationInputParser.h"
#include "RawSliceVolumeReader.h"
#include "RawVolumeReader.h"
#include "itkMaskImageFilter.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk
{

template <typename TVectorInputImage, typename TMaskImage>
ClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::ClassifierValidationInputParser()
{
  m_ImageDirectoryName = "";
  m_BrainMaskDirectoryName = "";
  m_BrainSegmentationDirectoryName = "";

  m_PatientID = "";
  m_StartSliceNumber = 1;
  m_NumberOfSlices = 1;
  m_NumberOfChannels = 1;

  m_ParameterFileName = "NoParamFile.txt";

  m_VectorInputImage = NULL;
  m_MaskImage = NULL;
  m_TruthImage = NULL;
  m_tempImage = NULL;

  m_NumberOfClasses = 1;

  //-------------------------------------------------------------------
  // Initialize the containers for means/covariance/number of samples 
  //-------------------------------------------------------------------

  m_ClassMeans.resize( m_NumberOfClasses, m_NumberOfChannels );
  m_ClassMeans.fill( 0 );

  m_ClassCovariances.resize( m_NumberOfClasses );

  for(unsigned int i = 0; i < m_NumberOfClasses; i++ )
    {
    m_ClassCovariances[i].resize( m_NumberOfChannels, m_NumberOfChannels );
    m_ClassCovariances[i].set_identity( );
    }

  m_ClassNumberOfSamples.resize( m_NumberOfClasses, m_NumberOfChannels );
  m_ClassNumberOfSamples.fill( 0 );
  
}


template <typename TVectorInputImage, typename TMaskImage>
void
ClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::ReadInputImage(
const char * patientID,
signed long startSlice,
unsigned long numberOfSlices,
TempImagePointer& image )
{

  std::string filePrefix;

  filePrefix = m_ImageDirectoryName;
  filePrefix += "/";
  filePrefix += patientID;
  filePrefix += "/";
  filePrefix += patientID;
  filePrefix += "_";
 
  typedef RawSliceVolumeReader<TempImagePixelType,TempImageType> ReaderType;
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


template <typename TVectorInputImage, typename TMaskImage>
void
ClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::ReadMaskImage(
const char * patientID,
unsigned long numberOfSlices,
MaskImagePointer& image )
{

  std::string fileName;

  fileName = m_BrainMaskDirectoryName;
  fileName += "/";
  fileName += patientID;
  fileName += ".buchar";
 
  typedef typename MaskImageType::PixelType PixelType;
  typedef RawVolumeReader<PixelType,MaskImageType> ReaderType;
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

template <typename TVectorInputImage, typename TMaskImage>
void
ClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::ReadTruthImage(
const char * patientID,
unsigned long numberOfSegSlices,
TruthImagePointer& image )
{

  std::string fileName;

  fileName = m_BrainSegmentationDirectoryName;
  fileName += "/";
  fileName += patientID;
  fileName += ".buchar";
 
  typedef typename MaskImageType::PixelType PixelType;
  typedef RawVolumeReader<PixelType,MaskImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();


  // Defaults for IBSR data
  ReaderType::SpacingType spacing;
  ReaderType::SizeType    size;
  bool                    bigEndian = true;

  spacing[0] = 1.0; spacing[1] = 1.0; spacing[2] = 3.0;
  size[0] = 256; size[1] = 256; size[2] = numberOfSegSlices;

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

template <typename TVectorInputImage, typename TMaskImage>
void
ClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::Execute()
{

  /*************************
   * Read in the mask image
   *************************/  

  this->ReadMaskImage( m_PatientID.c_str(), m_NumberOfSlices, 
    m_MaskImage );

  /*************************
   * Read in the input images
   *************************/
  this->ReadInputImage( m_PatientID.c_str(), m_StartSliceNumber, 
    m_NumberOfSlices, m_tempImage );


  //------------------------------------------------------
  //Mask the input images with the mask
  //------------------------------------------------------
  // Declare the type for the MaskInput filter

  typedef MaskImageFilter< TempImageType,
                           MaskImageType,
                           TempImageType  >   MaskFilterType;

  typedef TempImageType::Pointer   MaskedOutputImagePointer;
  typedef MaskFilterType::Pointer  MaskFilterTypePointer;

  // Create an ADD Filter                                
  MaskFilterTypePointer maskfilter = MaskFilterType::New();

  // Connect the input images
  maskfilter->SetInput1( m_tempImage ); 
  maskfilter->SetInput2( m_MaskImage );

  // Execute the filter
  maskfilter->Update();

  // Get the Smart Pointer to the Filter Output 
  MaskedOutputImagePointer maskedImage = maskfilter->GetOutput(); 

  //Pack the input image in a vector image

  
  //------------------------------------------------------
  // Allocate the vector image
  //------------------------------------------------------

  m_VectorInputImage = VectorInputImageType::New();

  typedef typename VectorInputImageType::SizeType  VectorInputImageSizeType;

  VectorInputImageSizeType vectorImageSize = 
    m_tempImage->GetBufferedRegion().GetSize();

  typedef typename VectorInputImageType::IndexType VectorInputImageIndexType;

  VectorInputImageIndexType index = 
    VectorInputImageIndexType::ZeroIndex;

  typename VectorInputImageType::RegionType region;

  region.SetSize( vectorImageSize );
  region.SetIndex( index );

  m_VectorInputImage->SetLargestPossibleRegion( region );
  m_VectorInputImage->SetBufferedRegion( region );
  m_VectorInputImage->Allocate();

  //------------------------------------------------------
  // setup the iterators for the temporary image
  //------------------------------------------------------
  typedef
    ImageRegionIterator< TempImageType > TempImageIterator;

  TempImageIterator tempImageIt( maskedImage, maskedImage->GetBufferedRegion() );

  //------------------------------------------------------
  // setup the iterators for the vector image
  //------------------------------------------------------

  typedef ImageRegionIterator< VectorInputImageType > VectorImageIterator;

  VectorImageIterator vectorImageIt( 
    m_VectorInputImage, m_VectorInputImage->GetBufferedRegion() );

  //------------------------------------------------------
  //Pack the input image to a vector image
  //------------------------------------------------------
  for ( tempImageIt.GoToBegin(); ! tempImageIt.IsAtEnd(); 
    ++tempImageIt, ++vectorImageIt ) 
    {
    vectorImageIt.Set( tempImageIt.Get() );
    }

  /*************************
   * Read in the ground truth image
   *************************/  
  this->ReadTruthImage(m_PatientID.c_str(), m_NumberOfSegSlices, 
    m_TruthImage );  

  /*************************
   * Read in algorithm parameters from file
   *************************/

  FILE * paramFile;
  
  if( m_ParameterFileName != "NoParamFile.txt" )
    {
    paramFile = fopen( m_ParameterFileName.c_str(), "r" );
    if( !paramFile )
      {
      itkExceptionMacro( << "Could not open parameter file. " );
      }
    
    // parse the file
    unsigned int uNumber;
    unsigned int dNumber;

    // get number of classes levels
    if( fscanf( paramFile, "%d", &uNumber) != 1 )
      {
      itkExceptionMacro( << "Could not find the number of histogram levels." );
      }
    m_NumberOfClasses = uNumber;

    //-------------------------------------------------------------------
    // Initialize the containers for means/covariance/number of samples 
    //-------------------------------------------------------------------
    // set the means container
    {
    m_ClassMeans.resize( m_NumberOfClasses, m_NumberOfChannels );
    m_ClassMeans.fill( 0 );
    }

    // set class covariance matrices container
    {
    m_ClassCovariances.resize( m_NumberOfClasses );
    for(unsigned int i = 0; i < m_NumberOfClasses; i++ )
      {
      m_ClassCovariances[i].resize( m_NumberOfChannels, m_NumberOfChannels );
      m_ClassCovariances[i].fill( 0 );
      }

    }

    // set the class number of samples container
    {
    m_ClassNumberOfSamples.resize( m_NumberOfClasses, m_NumberOfChannels );
    m_ClassNumberOfSamples.fill( 0 );    
    }

    //-------------------------------------------------------------------
    // Read the data from the paramfile to the storage containers
    //-------------------------------------------------------------------

    for( unsigned int j = 0; j < m_NumberOfClasses; j++ )
      {
      for(unsigned int i = 0; i < m_NumberOfChannels; i++ )
        {
        if( fscanf( paramFile, "%d", &dNumber ) != 1 )
          {
          itkExceptionMacro( << "Could not find number of class means. " );
          }
        m_ClassMeans[j][i] = dNumber;
        }
      }

    for( unsigned int k = 0; k < m_NumberOfClasses; k++ )
      {
      for(unsigned int j = 0; j < m_NumberOfChannels; j++ )
        {
        for(unsigned int i = 0; i < m_NumberOfChannels; i++ )
          {
          if( fscanf( paramFile, "%d", &dNumber ) != 1 )
            {
            itkExceptionMacro( << "Could not find number of covariance. " );
            }

          (m_ClassCovariances[k])[j][i] = dNumber;
          }
        }
      }

    for( unsigned int j = 0; j < m_NumberOfClasses; j++ )
      {
      for(unsigned int i = 0; i < m_NumberOfChannels; i++ )
        {
        if( fscanf( paramFile, "%d", &dNumber ) != 1 )
          {
          itkExceptionMacro( << "Could not find number of samples. " );
          }
        m_ClassNumberOfSamples[j][i] = dNumber;
        }
      }
    } //end if (file name != NoParamFile.txt
  else
    {
    std::cout << "Use unsupervised classification." << std::endl;
    }
      
/*** FIXME: remove later ****/

#define PRINTOUT( name ) \
  std::cout << "" #name ": " << m_##name << std::endl;

//  PRINTOUT( NumberOfClasses );
//  PRINTOUT( NumberOfChannels );
//  PRINTOUT( ClassMeans );
//  PRINTOUT( ClassNumberOfSamples );
// for( unsigned int i=0; i< m_NumberOfClasses; i++ )
//    PRINTOUT( ClassCovariances[i] );
//  PRINTOUT( SubjectImageShrinkFactors );
}


} // namespace itk

#endif
