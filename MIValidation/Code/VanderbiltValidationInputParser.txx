/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltValidationInputParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltValidationInputParser_txx
#define _VanderbiltValidationInputParser_txx

#include "VanderbiltValidationInputParser.h"
#include "VanderbiltHeaderParser.h"
#include "RawVolumeReader.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk
{

template <typename TImage>
VanderbiltValidationInputParser<TImage>
::VanderbiltValidationInputParser()
{
  m_ImageDirectoryName = "";
  m_ParameterFileName = "";
  m_PatientNumber = 0;
  m_FromModality = VanderbiltModality::CT;
  m_ToModality = VanderbiltModality::CT;

  m_FixedImage = NULL;
  m_MovingImage = NULL;

  for ( int j = 0; j < ImageDimension; j++ )
    {
    m_PermuteOrder[j] = j;
    }
  m_FlipAxes.Fill( false );

  m_NumberOfLevels = 1;
  m_FixedImageShrinkFactors.Fill( 1 );
  m_MovingImageShrinkFactors.Fill( 1 );

  m_NumberOfIterations = IterationsArrayType(1);
  m_NumberOfIterations.Fill( 10 );

  m_LearningRates = RatesArrayType(1);
  m_LearningRates.Fill( 1e-4 );

  m_TranslationScale = 100.0;

  m_DumpPGMFiles = true;
  m_PGMDirectoryName = "";
  
}


template <typename TImage>
void
VanderbiltValidationInputParser<TImage>
::ReadImage(
ModalityType modality,
ImagePointer& image,
PermuteOrderType& axes,
FlipAxesType& direction )
{

  std::string fileName;
  char patientID[12] = "practice";

  if ( m_PatientNumber > 0 )
    {
    sprintf( patientID,"patient_%03d", m_PatientNumber );
    }

  /*******************************
   * Parse the fixed image header
   *******************************/
  fileName = m_ImageDirectoryName;
  fileName += "/";
  fileName += patientID;
  fileName += "/";
  fileName += VanderbiltModality::DirectoryName[modality];
  fileName += "/header.ascii";
 
  typedef VanderbiltHeaderParser<ImageDimension> ParserType;
  typename ParserType::Pointer parser = ParserType::New();
  parser->SetFileName( fileName.c_str() );

  try
    {
    parser->Execute();
    }
  catch(...)
    {
    std::cout << "Error while parsing " << fileName.c_str() << std::endl;
    throw;
    }

  axes      = parser->GetImageAxes();
  direction = parser->GetImageDirection();


  /*******************************
   * Read in the fixed image
   *******************************/
  fileName = m_ImageDirectoryName;
  fileName += "/";
  fileName += patientID;
  fileName += "/";
  fileName += VanderbiltModality::DirectoryName[modality];
  fileName += "/image.bin";


  typedef typename ImageType::PixelType PixelType;
  typedef RawVolumeReader<PixelType,ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( fileName.c_str() );
  reader->SetSize( parser->GetImageSize() );
  reader->SetSpacing( parser->GetImageSpacing() );
  reader->SetBigEndian( parser->GetImageBigEndian() );

  try
    {
    reader->Execute();
    }
  catch(...)
    {
    std::cout << "Error while reading in image " << fileName.c_str() << std::endl;
    throw;
    }

  image = reader->GetImage();


}


template <typename TImage>
void
VanderbiltValidationInputParser<TImage>
::ComputeShrinkFactors(
ImagePointer & image,
double minSpacing,
ShrinkFactorsType & shrinkFactors )
{

  unsigned int maxShrinkFactor = 1 << (m_NumberOfLevels - 1);
  if ( maxShrinkFactor > MaximumShrinkFactor )
    {
    maxShrinkFactor = MaximumShrinkFactor;
    }

  for ( int j = 0; j < ImageDimension; j++ )
    {
    unsigned int factor;

    factor = maxShrinkFactor / static_cast<unsigned long>( 
      vnl_math_rnd( image->GetSpacing()[j] / minSpacing ) );

    unsigned int upper = 
      image->GetLargestPossibleRegion().GetSize()[j] / 
      MinimumImageSize;
    if ( factor > upper )
      {
      factor = upper;
      }
    if ( factor < 1 )
      {
      factor = 1;
      }

    shrinkFactors[j] = factor;

    }

}


template <typename TImage>
void
VanderbiltValidationInputParser<TImage>
::Execute()
{

  int j, k;

  PermuteOrderType fixedAxes, movingAxes;
  FlipAxesType     fixedDirection, movingDirection;


  /*************************
   * Read in the images
   *************************/
  this->ReadImage( m_ToModality, m_FixedImage, fixedAxes, fixedDirection );
  this->ReadImage( m_FromModality, m_MovingImage, movingAxes, movingDirection );


  // Compute the permute and flip parameters
  m_FlipAxes.Fill( false );
  for ( j = 0; j < ImageDimension; j++ )
    {
    // Find the correpsonding movingAxes for each fixedAxes
    for ( k = 0; k < ImageDimension; k++ )
      {
      if ( movingAxes[k] == fixedAxes[j] )
        {
        m_PermuteOrder[j] = k;
        if ( movingDirection[k] != fixedDirection[j] )
          {
          m_FlipAxes[j] = true;
          }
        }
      }
    }


  /*************************
   * Compute translation scale =
   * minimum (spacing[j] * size[j]) over both the fixed and moving image
   *************************/  
  m_TranslationScale = NumericTraits<double>::NonpositiveMin();
  
  for ( j = 0; j < ImageDimension; j++ )
    {
    double value;

    value = static_cast<double>( 
      m_FixedImage->GetLargestPossibleRegion().GetSize()[j] - 1 ) *
      m_FixedImage->GetSpacing()[j];
    if ( value > m_TranslationScale )
      {
      m_TranslationScale = value;
      }

    value = static_cast<double>( 
      m_MovingImage->GetLargestPossibleRegion().GetSize()[j] - 1 ) *
      m_MovingImage->GetSpacing()[j];
    if ( value > m_TranslationScale )
      {
      m_TranslationScale = value;
      }

    }

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
  float        fNumber;

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

  // get learning rates
  {
  itk::Array<double> temp( m_NumberOfLevels );
  temp.Fill( 0 );
  m_LearningRates = temp;
  }
  for( unsigned int j = 0; j < m_NumberOfLevels; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 )
     {
     itkExceptionMacro( << "Could not find learning rates per level. " );
     }
    m_LearningRates[j] = fNumber;
    }


  /*************************
   * Compute the starting shrink factors
   *************************/
  double minSpacing = NumericTraits<double>::max();
  for ( j = 0; j < ImageDimension; j++ )
    {    
    double value;

    value = m_FixedImage->GetSpacing()[j];
    if ( value < minSpacing )
      {
      minSpacing = value;
      }

    value = m_MovingImage->GetSpacing()[j];
    if ( value < minSpacing )
      {
      minSpacing = value;
      }

    }

  this->ComputeShrinkFactors( m_FixedImage, minSpacing, 
    m_FixedImageShrinkFactors );
  this->ComputeShrinkFactors( m_MovingImage, minSpacing,
    m_MovingImageShrinkFactors );


/*** FIXME: remove later ****/
#define PRINTOUT( name ) \
  std::cout << "" #name ": " << m_##name << std::endl;

  PRINTOUT( NumberOfLevels );
  PRINTOUT( NumberOfIterations );
  PRINTOUT( LearningRates );
  PRINTOUT( TranslationScale );
  PRINTOUT( FixedImageShrinkFactors );
  PRINTOUT( MovingImageShrinkFactors );

}


} // namespace itk

#endif
