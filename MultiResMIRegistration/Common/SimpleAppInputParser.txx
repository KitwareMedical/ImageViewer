/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SimpleAppInputParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SimpleAppInputParser_txx
#define _SimpleAppInputParser_txx

#include "SimpleAppInputParser.h"
#include "RawVolumeReader.h"
#include <stdio.h>

namespace itk
{

template <typename TImage>
SimpleAppInputParser<TImage>
::SimpleAppInputParser()
{
  m_ParameterFileName = "";
  m_FixedImage = NULL;
  m_MovingImage = NULL;

  for ( unsigned int j = 0; j < ImageDimension; j++ )
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

  m_PGMDirectoryName = "";

}


template <typename TImage>
void
SimpleAppInputParser<TImage>
::Execute()
{

  // open up the parameter file
  FILE * paramFile;
  paramFile = fopen( m_ParameterFileName.c_str(), "r" );
  if( !paramFile )
    {
    itkExceptionMacro( << "Could not open parameter file. " );
    }
    
  // parse the file
  char         currentLine[150];
  unsigned int uNumber;
  float        fNumber;


  /************************************
   * Parse fixed image information
   ************************************/
  // get fixed image file name
  if( fscanf( paramFile, "%s", currentLine ) != 1 ) 
    {
    itkExceptionMacro( << "Could not find fixed image filename." );
    }
  std::string fixedImageFileName = currentLine;
  std::cout << "Fixed image filename: " << fixedImageFileName << std::endl;

  // get fixed image big endian flag
  if( fscanf( paramFile, "%d", &uNumber ) != 1)
    {
    itkExceptionMacro( << "Could not find fixed image file big endian flag." );
    }
  bool fixedImageBigEndian = !(!uNumber);
  std::cout << "Big Endian: " << fixedImageBigEndian << std::endl;

  // get fixed image size
  typename ImageType::SizeType fixedImageSize;
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
      {
      itkExceptionMacro( << "Could not find fixed image size." );
      }

    fixedImageSize[j] = uNumber;
    } 
  std::cout << "Image Size: " << fixedImageSize << std::endl;

  // get fixed image spacing
  SpacingType fixedImageSpacing;
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 ) 
      {
      itkExceptionMacro( << "Could not find fixed image spacing." );
      }

    fixedImageSpacing[j] = fNumber;
    }
  std::cout << "Image Spacing: " << fixedImageSpacing;


  /************************************
   * Parse moving image information
   ************************************/
  // get moving image file name
  if( fscanf( paramFile, "%s", currentLine ) != 1 ) 
    {
    itkExceptionMacro( << "Could not find moving image filename." );
    }
  std::string movingImageFileName = currentLine;
  std::cout << "Moving image filename: " << movingImageFileName << std::endl;

  // get moving image big endian flag
  if( fscanf( paramFile, "%d", &uNumber ) != 1)
    {
    itkExceptionMacro( << "Could not find moving image file big endian flag." );
    }
  bool movingImageBigEndian = !(!uNumber);
  std::cout << "Big Endian: " << movingImageBigEndian << std::endl;

  // get moving image size
  typename ImageType::SizeType movingImageSize;
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
      {
      itkExceptionMacro( << "Could not find moving image size." );
      }

    movingImageSize[j] = uNumber;
    } 
  std::cout << "Image Size: " << movingImageSize << std::endl;

  // get moving image spacing
  SpacingType movingImageSpacing;
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 ) 
      {
      itkExceptionMacro( << "Could not find moving image spacing." );
      }
    movingImageSpacing[j] = fNumber;
    }
  std::cout << "Image Spacing: " << movingImageSpacing;


  /************************************
   * Parse permute and flip parameters
   ************************************/
  // get permute order
  for ( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
      {
      itkExceptionMacro( << "Could not find permute order." );
      }

    m_PermuteOrder[j] = uNumber;
    }
  std::cout << "Permute order: " << m_PermuteOrder;


  // get which axes to flip
  for ( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
      {
      itkExceptionMacro( << "Could not find permute order." );
      }

    m_FlipAxes[j] = !(!uNumber);
    }
  std::cout << "Flip axes: " << m_FlipAxes;


  /************************************
   * Parse registration parameters
   ************************************/
  // get number of levels
  std::cout << "Number of levels: ";
  if( fscanf( paramFile, "%d", &uNumber) != 1 )
    {
    itkExceptionMacro( << "Could not find the number of levels." );
    }
  m_NumberOfLevels = uNumber;
  std::cout << uNumber << std::endl;

  // get fixed image shrink factors
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
      {
      itkExceptionMacro( << "Could not find fixed image shrink factors. " );
      }

    m_FixedImageShrinkFactors[j] = uNumber;
    }
  std::cout << "Fixed image shrink factors: " << m_FixedImageShrinkFactors;
 
  // get moving image shrink factors
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 )
      {
      itkExceptionMacro( << "Could not find moving image shrink factors. " );
      }
    m_MovingImageShrinkFactors[j] = uNumber;
    }
  std::cout << "Moving image shrink factors: " << m_MovingImageShrinkFactors;

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
  std::cout << "Number of iterations: " << m_NumberOfIterations;

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
  std::cout << "Learning rates: " << m_LearningRates;

  // get translation scale
  if( fscanf( paramFile, "%f", &fNumber) != 1 )
    {
    itkExceptionMacro( << "Could not find the translation scale." );
    }
  m_TranslationScale = fNumber;
  std::cout << "Translation scale: " << fNumber << std::endl;


  /************************************
   * Parse output related parameters
   ************************************/
  // pgm directory
  if( fscanf( paramFile, "%s", currentLine ) != 1 ) 
  {
  itkExceptionMacro( << "Could not find PGM directory name." );
  }

  m_PGMDirectoryName = currentLine;

  std::cout << "PGM directory: " << m_PGMDirectoryName << std::endl;
  std::cout << std::endl;


  /***********************
   * Read in the images
   ***********************/
   typedef typename ImageType::PixelType PixelType;
   typedef itk::RawVolumeReader<PixelType,ImageType> ReaderType;
   typename ReaderType::Pointer reader = ReaderType::New();
  
   try 
    {
    reader->SetFileName( fixedImageFileName.c_str() );
    reader->SetSize( fixedImageSize );
    reader->SetSpacing( fixedImageSpacing );
    reader->SetBigEndian( fixedImageBigEndian );
    reader->Execute();
    
    m_FixedImage = reader->GetImage();
    }
   catch(...)
    {
    std::cout << "Error while reading in fixed image." << std::endl;
    throw;
    }


   try 
    {
    reader->SetFileName( movingImageFileName.c_str() );
    reader->SetSize( movingImageSize );
    reader->SetSpacing( movingImageSpacing );
    reader->SetBigEndian( movingImageBigEndian );
    reader->Execute();
    
    m_MovingImage = reader->GetImage();
    }
   catch(...)
    {
    std::cout << "Error while reading in moving image." << std::endl;
    throw;
    }

 
}


} // namespace itk

#endif
