/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FuzzyConnectApp.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "FuzzyConnectApp.h"

#include "itkByteSwapper.h"
#include "itkImageRegionIterator.h"
#include "itkImageMapper.h"
#include "itkRawImageWriter.h"
#include "itkExceptionObject.h"

#include "vnl/vnl_math.h"

#include <fstream>
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
  m_Filter->SetSeed( m_Seed );

  m_ObjectMean = double( m_InputImage->GetPixel( m_Seed ) );

	m_Filter->SetParameters( m_ObjectMean, m_ObjectVariance,
    m_DiffMean, m_DiffVariance, m_Weight );

	m_Filter->SetThreshold( m_Threshold );

  m_Filter->ExcuteSegment();
 
}


void
FuzzyConnectApp
::ComputeSegmentationImage()
{

  m_Filter->SetThreshold( m_Threshold );
  m_Filter->MakeSegmentObject();

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
InputImageType * imgPtr
)
{
  //*****
  //FIXME - use itkRawImageIO instead once its stable
  //*****

  // allocate memory in the image
  InputImageType::RegionType region;
  region.SetSize( size );

  imgPtr->SetLargestPossibleRegion( region );
  imgPtr->SetBufferedRegion( region );
  imgPtr->Allocate();

  double origin[ImageDimension];
  for( int j = 0; j < ImageDimension; j++ )
    {
    origin[j] = -0.5 * ( double(size[j]) - 1.0 ) * spacing[j];
    }

  imgPtr->SetSpacing( spacing );
  imgPtr->SetOrigin( origin ); 

  unsigned int numPixels = region.GetNumberOfPixels(); 


  // open up the file
  std::ifstream imgStream( filename, std::ios::binary | std::ios::in );
  
  if( !imgStream.is_open() )
    {
    return false;
    }

  // read the file
  InputPixelType * buffer = imgPtr->GetBufferPointer();
  imgStream.read( (char *) buffer, numPixels * sizeof(InputPixelType) );

  // clost the file
  imgStream.close();


  // swap bytes if neccessary
  if( bigEndian )
    {
    itk::ByteSwapper<InputPixelType>::SwapRangeBE( buffer, numPixels );
    }
  else
    {
    itk::ByteSwapper<InputPixelType>::SwapRangeLE( buffer, numPixels );
    }

  return true;

}

bool
FuzzyConnectApp
::WritePGMFiles(
InputImageType * input, 
const char * dirname,
const char * basename )
{

  // go through the image and compute the offset and scale
  // to make it normalized to between 0 and 255
  typedef itk::ImageRegionIterator<InputImageType>
   InputIterator;

  InputIterator inIter( input, input->GetBufferedRegion() );

  InputPixelType minValue = inIter.Get();
  InputPixelType maxValue = minValue;
  while( !inIter.IsAtEnd() )
    {
    InputPixelType value = inIter.Get();
    if( value < minValue ) minValue = value;
    if( value > maxValue ) maxValue = value;
    ++inIter;
    }

  double scale = double( maxValue - minValue );
  if( !scale ) scale = 1.0;
  double offset = double(minValue);
  
  // write image out to pgm files
  char filename[256];
  char buffer[50];
  unsigned long ncol = input->GetBufferedRegion().GetSize()[0];
  unsigned long nrow = input->GetBufferedRegion().GetSize()[1];
  unsigned long nslice = input->GetBufferedRegion().GetSize()[2];

  sprintf(buffer,"P5 %ld %ld 255\n", ncol, nrow );
  unsigned int nchar = strlen(buffer);
  unsigned long npixels = nrow * ncol;

  inIter.GoToBegin();

  for( unsigned int k = 0; k < nslice; k++ )
    {
    if( k < 10 )
      {
      sprintf(filename,"%s/%s00%d.pgm", dirname, basename, k );
      }
    else if( k < 100 )
      {
      sprintf(filename, "%s/%s0%d.pgm", dirname, basename, k );
      }
    else
      {
      sprintf(filename, "%s/%s%d.pgm", dirname, basename, k );
      }

    // open up the stream  
    std::ofstream imgStream( filename, std::ios::out | std::ios::binary );
  
    if( !imgStream.is_open() )
      {
      return false;
      }

    // writer the header
    imgStream.write( buffer, nchar );

    // write the bytes
    for( unsigned long i = 0; i < npixels; i++ )
      {
      double value = (double(inIter.Get()) - offset) / scale * 255;
      char num = vnl_math_rnd( value );
      imgStream.put( num );
      ++inIter;
      }

    // close this file
    imgStream.close();
    
    }

  return true;
}
