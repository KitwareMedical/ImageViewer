/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectApp.cxx
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
#include "ShapeDetectApp.h"
#include "AppUtility.h"

#include "itkByteSwapper.h"
#include "itkImageRegionIterator.h"
#include "itkImageMapper.h"
#include "itkExceptionObject.h"

#include "itkFastMarchingImageFilter.h"

#include "vnl/vnl_math.h"

#include <string>


ShapeDetectApp
::ShapeDetectApp()
{
  this->Initialize();
}


void
ShapeDetectApp
::Initialize()
{

  m_InputImage = InputImageType::New();
  m_EdgePotentialImage = ImageType::New();
  m_SegmentationMask = ImageType::New();
  m_DetectionFilter = DetectionFilterType::New();

  m_Sigma = 1.0;

  m_DumpPGMFiles = true;

}


void
ShapeDetectApp
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
    m_InputBigEndian, m_InputImage ) )
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
    if( !WritePGMFiles<InputImageType>( 
        m_InputImage, m_PGMDirectory.c_str(), "input" ))
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }
    }

  this->ComputeEdgePotentialMap();

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
  
 
  // run the filter
  std::cout << "Generating time crossing map." << std::endl;
  this->ComputeTimeCrossingMap();


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
         std::cout << "Re-generating time crossing map." << std::endl;
         this->ComputeTimeCrossingMap();
         this->ThresholdTimeCrossingMap();
         this->WriteSegmentationImage();
         break;
      case 't' :
        if( sscanf( currentLine, "%c%f", &symbol, &fValue ) != 2 )
          {
          std::cout << "Error: one floating point value expected" << std::endl;
          continue;
          }
         m_Threshold = fValue;;
         std::cout << "Re-thresholding time crossing map." << std::endl;
         this->ThresholdTimeCrossingMap();
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
ShapeDetectApp
::ComputeTimeCrossingMap()
{

  // connect edge potential map
  m_DetectionFilter->SetSpeedImage( m_EdgePotentialImage );
  
  // setup trial points
  typedef DetectionFilterType::NodeType NodeType;
  typedef DetectionFilterType::NodeContainer NodeContainer;

  NodeContainer::Pointer trialPoints = NodeContainer::New();

  NodeType node;
  
  node.SetValue( 0.0 );
  node.SetIndex( m_Seed );
  trialPoints->InsertElement(0, node);
  
  m_DetectionFilter->SetTrialPoints( trialPoints );

  // specify the size of the output image
  m_DetectionFilter->SetOutputSize( m_InputImage->GetBufferedRegion().GetSize() );

  // update the marcher
  m_DetectionFilter->Update();

}


void
ShapeDetectApp
::ThresholdTimeCrossingMap()
{

  // threshold the time crossing map
  m_SegmentationMask->SetLargestPossibleRegion(
    m_InputImage->GetLargestPossibleRegion() );
  m_SegmentationMask->SetBufferedRegion(
    m_InputImage->GetBufferedRegion() );
  m_SegmentationMask->Allocate();

  typedef itk::ImageRegionIterator<ImageType>
   Iterator;
  Iterator inIter( m_DetectionFilter->GetOutput(),
    m_DetectionFilter->GetOutput()->GetBufferedRegion() );

  Iterator outIter( m_SegmentationMask,
    m_SegmentationMask->GetBufferedRegion() );

  while( !inIter.IsAtEnd() )
    {
    if( inIter.Get() <= m_Threshold )
      {
      outIter.Set( 1 );
      }
    else
      {
      outIter.Set( 0 );
      }
    ++inIter;
    ++outIter;
    }

}

void
ShapeDetectApp
::WriteSegmentationImage()
{

   if( m_DumpPGMFiles )
    {
    //dump out the segmented image
    if( !WritePGMFiles<ImageType>( m_SegmentationMask, m_PGMDirectory.c_str(), "seg" ) )
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }
    }

}


void
ShapeDetectApp
::ComputeEdgePotentialMap()
{

  // compute derivative of the input image
  DerivativeFilterPointer deriv = DerivativeFilterType::New();
  deriv->SetInput( m_InputImage );
  deriv->SetSigma( m_Sigma );
  deriv->Update();

  // allocate memory for the map
  m_EdgePotentialImage->SetLargestPossibleRegion(
    m_InputImage->GetLargestPossibleRegion() );
  m_EdgePotentialImage->SetBufferedRegion(
    m_InputImage->GetBufferedRegion() );
  m_EdgePotentialImage->Allocate();

  //****
  //FIXME - use an itk filter once API are consistent
  //****
  // compute the magnitude 
  typedef itk::ImageRegionIterator<DerivativeImageType> 
    DerivativeIterator;
  typedef itk::ImageRegionIterator<ImageType>
    ImageIterator;

  DerivativeIterator derivIter( deriv->GetOutput(),
    deriv->GetOutput()->GetBufferedRegion() );
  ImageIterator mapIter( m_EdgePotentialImage,
    m_EdgePotentialImage->GetBufferedRegion() );

  while( !derivIter.IsAtEnd() )
    {

    float accum = 0;
    VectorType grad = derivIter.Get();

    for( int j = 0; j < ImageDimension; j++ )
      {
      accum += vnl_math_sqr( grad[j] );
      }
    
    accum = vnl_math_sqrt( accum );
//    mapIter.Set( 1.0 / ( 1.0 + accum ) );
    mapIter.Set( exp( -1.0 * accum ) );

    ++derivIter;
    ++mapIter;

    } 


  if( m_DumpPGMFiles )
    {
    //dump out the input image
    std::cout << "Writing PGM files of the edge potential map." << std::endl;
    if( !WritePGMFiles<ImageType>( 
        m_EdgePotentialImage, m_PGMDirectory.c_str(), "map" ))
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }
    }    

}


bool 
ShapeDetectApp
::ReadImage(
const char * filename,
const SizeType& size,
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


