/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FuzzyConnectApp.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <fstream>
#include "FuzzyConnectApp.h"

#include "RawVolumeReader.h"
#include "PGMVolumeWriter.h"
#include <string>


FuzzyConnectApp
::FuzzyConnectApp()
{
  this->Initialize();
}


void
FuzzyConnectApp
::Initialize()
{

  m_InputImage = InputImageType::New();
  m_Filter = FilterType::New();
  m_ObjectVariance = 2500.0;
  m_DiffMean = 1.0;
  m_DiffVariance = 1.0;
  m_Weight = 1.0;

  m_DumpPGMFiles = true;

  for( int j = 0; j < ImageDimension; j++ )
    {
    m_InputSpacing[j] = 1.0;
    }

}


void
FuzzyConnectApp
::Execute()
{

  char         currentLine[150];
  char         buffer[150];
  unsigned int uNumbers[3];
  float       fValue;
  char         symbol;

  std::cout << std::endl;

  // Get input file name
  while(1)
    {
    std::cout << "Input file name: ";
    std::cin.getline( currentLine, 150);
    if( sscanf( currentLine, "%s", buffer ) >= 1 ) break;
    std::cout << "Error: filename expected." << std::endl;
    }
  m_InputFileName = buffer;

  // Get big endian flag
  while(1)
    {
    std::cout << "Input image big endian? [y|n]: ";
    std::cin.getline( currentLine, 150 );
    if( sscanf( currentLine, "%c", &symbol ) >= 1  &&
        ( symbol == 'y' || symbol == 'n' ) ) break;
    std::cout << "Error: 'y' or 'n' expected." << std::endl;
    }  
  if( symbol == 'y' )
    {
    m_InputBigEndian = true;
    }
  else
    {
    m_InputBigEndian = false;
    }

  // Get input file size
  while(1)
    {
    std::cout << "Input image size: ";
    std::cin.getline( currentLine, 150 );
    if( sscanf( currentLine, "%d%d%d", uNumbers, uNumbers+1, uNumbers+2 ) >= 3 ) break;
    std::cout << "Error: three unsigned integers expected." << std::endl;
    } 
  for( int j = 0; j < ImageDimension; j++ )
    {
    m_InputSize[j] = uNumbers[j];
    }


  // Read in input image
  if( !this->ReadImage( m_InputFileName.c_str(), m_InputSize, 
    m_InputSpacing, m_InputBigEndian, m_InputImage ) )
   {
    std::cout << "Error while reading in input volume: ";
    std::cout << m_InputFileName.c_str() << std::endl;
    return;
   }

  // Get input file name
  while(1)
    {
    std::cout << "PGM output directory: ";
    std::cin.getline( currentLine, 150);
    if( sscanf( currentLine, "%s", buffer ) >= 1 ) break;
    std::cout << "Error: directory name expected." << std::endl;
    }
  m_PGMDirectory = buffer;

  if( m_DumpPGMFiles )
    {
    //dump out the input image
    std::cout << "Writing PGM files of the input volume." << std::endl;
    if( !this->WritePGMFiles( m_InputImage, m_PGMDirectory.c_str(), "input" ))
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }
    }

  std::cout << std::endl << std::endl;
  // Set the initial seed
  while(1)
    {
    std::cout << "Set initial seed index: ";
    std::cin.getline( currentLine, 150 );
    if( sscanf( currentLine, "%d%d%d", uNumbers, uNumbers+1, uNumbers+2 ) >= 3 ) break;
    std::cout << "Error: three unsigned integers expected." << std::endl;
    } 
  for( int j = 0; j < ImageDimension; j++ )
    {
    m_Seed[j] = uNumbers[j];
    }
  
  // Set the initial threshold
  while(1)
    {
    std::cout << "Set initial threshold value: ";
    std::cin.getline( currentLine, 150 );
    if( sscanf( currentLine, "%f", &fValue ) >= 1 ) break;
    std::cout << "Error: one floating point value expected." << std::endl;
    } 
  m_Threshold = fValue;
 
  // run the filter
  std::cout << "Generating the fuzzy connectedness map." << std::endl;
  this->ComputeMap();
  this->WriteSegmentationImage();

  while(1)
   {
   std::cout << std::endl << "Command [s|t|d|x]: ";
   std::cin.getline( currentLine, 150 );
   if( sscanf( currentLine, "%s", buffer ) >= 1 )
     {
     // parse the command
     switch ( buffer[0] )
      {
      case 's' :
        if( sscanf( currentLine, "%c%d%d%d", &symbol, uNumbers, uNumbers+1, uNumbers+2 ) != 4 )
          {
          std::cout << "Error: three unsigned integers expected" << std::endl;
          continue;
          }
         for( int j = 0; j < ImageDimension; j++ )
          {
          m_Seed[j] = uNumbers[j];
          }
         std::cout << "Re-generating the fuzzy connectedness map." << std::endl;
         this->ComputeMap();
         this->WriteSegmentationImage();
         break;
      case 't' :
        if( sscanf( currentLine, "%c%f", &symbol, &fValue ) != 2 )
          {
          std::cout << "Error: one floating point value expected" << std::endl;
          continue;
          }
         m_Threshold = fValue;;
         std::cout << "Re-thresholding the map." << std::endl;
         this->ComputeSegmentationImage();
         this->WriteSegmentationImage();
         break;
        break;
      case 'd':
        std::cout << "Seed: " << m_Seed << "Threshold: " << m_Threshold << std::endl;
        break;
      case 'x' :
        std::cout << "Goodbye. " << std::endl;
        return;
        break;
      default :
        std::cout << "Not a valid command." << std::endl;
      } //end switch

    }
    
   } //end while

}


void
FuzzyConnectApp
::ComputeMap()
{

  m_Filter->SetInput( m_InputImage );
  m_Filter->SetObjectsSeed( m_Seed );

  m_ObjectMean = double( m_InputImage->GetPixel( m_Seed ) );

  m_Filter->SetParameters( m_ObjectMean, m_ObjectVariance,
    m_DiffMean, m_DiffVariance, m_Weight );

  m_Filter->SetThreshold( m_Threshold );

  m_Filter->Update();
 
}


void
FuzzyConnectApp
::ComputeSegmentationImage()
{

  m_Filter->UpdateThreshold( m_Threshold );

}


void
FuzzyConnectApp
::WriteSegmentationImage()
{

   if( m_DumpPGMFiles )
    {
    //dump out the segmented image
    if( !this->WritePGMFiles( m_Filter->GetOutput(), m_PGMDirectory.c_str(), "seg" ) )
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }
    }

}

bool 
FuzzyConnectApp
::ReadImage(
const char * filename,
const SizeType& size,
const double * spacing,
bool  bigEndian,
InputImageType::Pointer &imgPtr
)
{

  // Read in a raw volume
  typedef itk::RawVolumeReader<InputPixelType,InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
 
  reader->SetFileName( filename );
  reader->SetBigEndian( bigEndian );
  reader->SetSize( size );
  reader->SetSpacing( spacing );
  reader->Execute();

  imgPtr = reader->GetImage();

  return true;

}


bool
FuzzyConnectApp
::WritePGMFiles(
InputImageType * input, 
const char * dirname,
const char * basename )
{

  typedef itk::PGMVolumeWriter<InputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetImage( input );
  writer->SetDirectoryName( dirname );
  writer->SetFilePrefix( basename );
  writer->Execute();

  return true;
}
