/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MSClassifierValidationInputParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MSClassifierValidationInputParser_txx
#define _MSClassifierValidationInputParser_txx

#include "MSClassifierValidationInputParser.h"
#include "RawVolumeReader.h"


#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk
{

template <typename TVectorInputImage, typename TMaskImage>
MSClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::MSClassifierValidationInputParser()
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
  m_tempImagePointerVector.resize(0);

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
MSClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::ReadInputImage(
const char * patientID,
unsigned long vectorNumber,
unsigned long numberOfSlices,
TempImagePointer& image )
{

  std::string fileName;

  fileName = m_ImageDirectoryName;
  fileName += "/";
  fileName += patientID;
  fileName += "/";
  fileName += patientID;
  fileName += m_FileExtensions[vectorNumber];
 
  typedef RawVolumeReader<TempImagePixelType,TempImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();


  // Defaults for IBSR data
  typename ReaderType::SpacingType spacing;
  typename ReaderType::SizeType    size;
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
    std::cout << "Error while reading in image for patient " << patientID << std::endl;
    throw;
    }

  image = reader->GetImage();

}

template <typename TVectorInputImage, typename TMaskImage>
void
MSClassifierValidationInputParser<TVectorInputImage,TMaskImage>
::Execute()
{

  m_tempImagePointerVector.resize(m_NumberOfChannels); 
  /*************************
   * Read in the input images
   *************************/
  for( unsigned int channelNumber = 0; 
                    channelNumber < m_NumberOfChannels;
                    channelNumber++ )
    {
    
    this->ReadInputImage( m_PatientID.c_str(), channelNumber, m_NumberOfSlices, 
      m_tempImagePointerVector[channelNumber] );
    }

  //*************************
  //Pack the input image in a vector image
  //*************************
  
  //------------------------------------------------------
  // Allocate the vector image
  //------------------------------------------------------

  m_VectorInputImage = VectorInputImageType::New();

  typedef typename VectorInputImageType::SizeType  VectorInputImageSizeType;

  VectorInputImageSizeType vectorImageSize = 
    (m_tempImagePointerVector[0])->GetBufferedRegion().GetSize();

  typedef typename VectorInputImageType::IndexType VectorInputImageIndexType;

  VectorInputImageIndexType index;
  index.Fill(0);

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
  
  std::vector< TempImageIterator > tempImageItVector;
  tempImageItVector.resize( m_NumberOfChannels );

  for( unsigned int i=0; i< m_NumberOfChannels; i++ )
    {
    TempImageIterator tempImageIt( m_tempImagePointerVector[i], 
      (m_tempImagePointerVector[i])->GetBufferedRegion() );
    tempImageItVector[i] = tempImageIt;
    }

  //------------------------------------------------------
  // setup the iterators for the vector image
  //------------------------------------------------------

  typedef ImageRegionIterator< VectorInputImageType > VectorImageIterator;

  VectorImageIterator vectorImageIt( 
    m_VectorInputImage, m_VectorInputImage->GetBufferedRegion() );

  //------------------------------------------------------
  //Pack the input image to a vector image
  //------------------------------------------------------
  // setup the iterators
  typedef typename VectorInputImageType::PixelType VectorInputImagePixelType;
  VectorInputImagePixelType vectorSample;

  for ( vectorImageIt.GoToBegin(); ! vectorImageIt.IsAtEnd(); ++vectorImageIt ) 
    {
    for( unsigned int i=0; i<m_NumberOfChannels; ++i)
      {
      vectorSample[i] = (tempImageItVector[i]).Get();
       ++(tempImageItVector[i]); 
      }
    vectorImageIt.Set( vectorSample );
    }

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
    int dNumber;

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
    m_ClassNumberOfSamples.resize( m_NumberOfClasses, 1 );
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
        if( fscanf( paramFile, "%d", &dNumber ) != 1 )
          {
          itkExceptionMacro( << "Could not find number of samples. " );
          }
        m_ClassNumberOfSamples[j][0] = dNumber;
      }

    } //end if (file name != NoParamFile.txt
  else
    {
    std::cout << "Use unsupervised classification." << std::endl;
    }
      
/*** FIXME: remove later ****/


#define PRINTOUT( name ) \
  std::cout << "" #name ": \n" << m_##name << std::endl;

  PRINTOUT( NumberOfClasses );
  PRINTOUT( NumberOfChannels );
  PRINTOUT( ClassMeans );
  PRINTOUT( ClassNumberOfSamples );
  for( unsigned int i=0; i< m_NumberOfClasses; i++ )
    PRINTOUT( ClassCovariances[i] );
  //PRINTOUT( SubjectImageShrinkFactors );
}

} // namespace itk

#endif
