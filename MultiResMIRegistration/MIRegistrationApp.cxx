/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIRegistrationApp.cxx
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
#include "MIRegistrationApp.h"

#include "itkByteSwapper.h"
#include "itkImageRegionIterator.h"
#include "itkImageMapper.h"
#include "itkRawImageWriter.h"
#include "itkExceptionObject.h"

#include "vnl/vnl_math.h"
#include "vnl/vnl_quaternion.h"
#include "vnl/vnl_diag_matrix.h"

#include <fstream>
#include <string>


MIRegistrationApp
::MIRegistrationApp()
{
  this->Initialize();
}


MIRegistrationApp
::MIRegistrationApp(
const char * filename )
{
 
  if ( !this->ParseInputFile( filename ) )
   {
    itk::ExceptionObject err;
    err.SetLocation( "MIRegistrationApp" );
    err.SetDescription( "Error while parsing input parameter file" );
    throw err;
   }

  this->Initialize();
  
}

void
MIRegistrationApp
::Initialize()
{

  m_Target = InputImageType::New();
  m_Reference = InputImageType::New();
  m_NormalizedTarget = ImageType::New();
  m_NormalizedReference = ImageType::New();
  m_Registrator = RegistratorType::New();

}


void
MIRegistrationApp
::Execute()
{

  //Read in target image
  std::cout << "Reading in target." << std::endl;
  if( !this->ReadImage( m_TargetFileName.c_str(), m_TargetSize, 
    m_TargetSpacing, m_TargetBigEndian, m_Target ) )
   {
    std::cout << "Error while reading in target." << std::endl;
    return;
   }

  //Read in reference image
  std::cout << "Reading in reference." << std::endl;
  if( !this->ReadImage( m_ReferenceFileName.c_str(), m_ReferenceSize, 
    m_ReferenceSpacing, m_ReferenceBigEndian, m_Reference ) )
   {
    std::cout << "Error while reading in reference." << std::endl;
    return;
   }

  //Normalize the target image
  std::cout << "Normalizing the target." << std::endl;
  this->NormalizeImage( m_Target, m_NormalizedTarget );

  //Normalize the reference image
  std::cout << "Normalizing the reference." << std::endl;
  this->NormalizeImage( m_Reference, m_NormalizedReference );

  //Set up the registrator
  std::cout << "Setting up the registrator." << std::endl;
  this->SetUpRegistrator();

  // Start the registrator
  std::cout << "Start the registration." << std::endl;

  try
    {
    m_Registrator->StartRegistration();
    }
  catch(...)
    {
    std::cout << "Error occured during registration" << std::endl;
    throw;
    }

  // Get the results
  m_Parameters = m_Registrator->GetInternalRegistrationMethod()->GetParameters();
  std::cout << "Final parameters: " << m_Parameters << std::endl;

  // Transform the reference
  std::cout << "Transforming the reference." << std::endl;
  this->TransformReference();

  // Write out the registered image
  std::cout << "Writing registered image to " << m_RegisteredImageFileName;
  std::cout << "." << std::endl;
  this->WriteImage( m_RegisteredImage, m_RegisteredImageFileName.c_str(), 
    m_RegisteredImageBigEndian );

  if( m_DumpPGMFiles )
    {
    //dump out the target
    std::cout << "Writing PGM files of the target." << std::endl;
    if( !this->WritePGMFiles( m_Target, m_PGMDirectory.c_str(), "target" ) )
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }

    //dump out the reference
    std::cout << "Writing PGM files of the reference." << std::endl;
    if( !this->WritePGMFiles( m_Reference, m_PGMDirectory.c_str(), "refer" ) )
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }

    //dump out the registered image
    std::cout << "Writing PGM files of the registered image." << std::endl;
    if( !this->WritePGMFiles( m_RegisteredImage, 
      m_PGMDirectory.c_str(), "register" ) )
      { 
      std::cout << "Error while writing PGM files.";
      std::cout << "Please make sure the path is valid." << std::endl; 
      return; 
      }

    }

}


void
MIRegistrationApp
::SetUpRegistrator()
{

  // connect the images
  m_Registrator->SetTarget( m_NormalizedTarget );
  m_Registrator->SetReference( m_NormalizedReference );

  InternalRegistratorType::Pointer internalRegistrator =
    m_Registrator->GetInternalRegistrationMethod();

  // set the parzen window width
  internalRegistrator->GetMetric()->SetTargetStandardDeviation( 0.4 );
  internalRegistrator->GetMetric()->SetReferenceStandardDeviation( 0.4 );

  // set the spatial samples to be used
  internalRegistrator->GetMetric()->SetNumberOfSpatialSamples( 50 );

  // set multiresolution related parameters
  m_Registrator->SetNumberOfLevels( m_NumberOfLevels );

  m_Registrator->GetTargetPyramid()->
    SetStartingShrinkFactors( m_TargetShrinkFactors );
  m_Registrator->GetReferencePyramid()->
    SetStartingShrinkFactors( m_ReferenceShrinkFactors );

  m_Registrator->SetNumberOfIterations( m_NumberOfIterations );
  m_Registrator->SetLearningRates( m_LearningRates );

  // set the translation scale
  itk::Point<double,7> scales;
  scales.Fill(1.0);
  for( int j = 4; j < 7; j++ )
    {
    scales[j] = 1.0 / vnl_math_sqr( m_TranslationScales[0] );
    }
  internalRegistrator->GetOptimizer()->GetTransform()->SetScale( scales );
  
}



bool 
MIRegistrationApp
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


void
MIRegistrationApp
::WriteImage(
InputImageType * input,
const char * filename,
bool bigEndian )
{

  typedef itk::RawImageWriter<InputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  if( bigEndian )
    {
    writer->SetByteOrder( WriterType::BigEndian );
    }
  else
    {
    writer->SetByteOrder( WriterType::LittleEndian );
    }

  writer->SetFileTypeToBinary();
  writer->SetFileName( filename );
  writer->SetInput( input );
  writer->Write();

}



void 
MIRegistrationApp
::NormalizeImage(
InputImageType * input,
ImageType * output )
{

  InputImageType::RegionType region = input->GetBufferedRegion();

  // make output the same size as the input
  output->SetLargestPossibleRegion(
    input->GetLargestPossibleRegion() );

  output->SetBufferedRegion( region );
  output->Allocate();

  // make output have the same spacing/origin;
  output->SetSpacing( input->GetSpacing() );
  output->SetOrigin( input->GetOrigin() );

  typedef itk::ImageRegionIterator<InputImageType>
    InputIterator;
  typedef itk::ImageRegionIterator<ImageType>
    OutputIterator;

  // walk the input to collect sums
  double sum = 0.0;
  double sumsq = 0.0;

  InputIterator inIter( input, region );

  while( !inIter.IsAtEnd() )
    {
    sum += inIter.Get();
    sumsq += vnl_math_sqr( inIter.Get() );
    ++inIter;
    }

  // compute mean and standard deviation
  double numPixels = (double) region.GetNumberOfPixels();
  double mean =  sum / numPixels;
  double stddev  = vnl_math_sqrt( 
    (sumsq - numPixels * vnl_math_sqr( mean )) / (numPixels - 1 ) );

  std::cout << "Mean: " << mean << " StdDev: " << stddev << std::endl;

  // walk both the input and output
  OutputIterator outIter( output, region );

  inIter.GoToBegin();
  outIter.GoToBegin();

  while( !inIter.IsAtEnd() )
    {
    double value = double( inIter.Get() );
    value -= mean;
    value /= stddev;
    
    outIter.Set( value );
    
    ++inIter;
    ++outIter;
    }

}


void
MIRegistrationApp
::TransformReference()
{

  //*****
  //FIXME - use itkResampleImageFilter instead once its stable
  //*****
  // set up the mapper
  typedef InternalRegistratorType::TransformationType   TransformationType;
  typedef itk::ImageMapper<InputImageType,TransformationType> MapperType;
  typedef MapperType::Pointer                   MapperPointer;
  MapperPointer mapper = MapperType::New();

  mapper->GetTransform()->SetParameters( m_Parameters );
  mapper->SetDomain( m_Reference );

  // allocate memory in the output image]
  m_RegisteredImage = InputImageType::New();

  m_RegisteredImage->SetLargestPossibleRegion( 
    m_Target->GetLargestPossibleRegion() );
  m_RegisteredImage->SetBufferedRegion(
    m_Target->GetBufferedRegion() );
  m_RegisteredImage->Allocate();

  const double * spacing = m_Target->GetSpacing();
  const double * origin = m_Target->GetOrigin();
  m_RegisteredImage->SetSpacing( spacing );
  m_RegisteredImage->SetOrigin( origin );

  // set up the iterators
  typedef itk::ImageRegionIterator<InputImageType> Iterator;
  Iterator inIter( m_RegisteredImage, 
    m_RegisteredImage->GetBufferedRegion() );
  
  while( !inIter.IsAtEnd() )
    {
    typedef InputImageType::IndexType IndexType;
    typedef MapperType::InputPointType PointType;

    IndexType index = inIter.GetIndex();
    PointType point;

    for( int j = 0; j < ImageDimension; j++ )
      {
      point[j] = double(index[j]) * spacing[j] + origin[j];
      }
    if( mapper->IsInside( point ) )
      {
      inIter.Set( mapper->Evaluate() );
      }
    else
      {
      inIter.Set( 0 );
      }
    
    ++inIter;
    }
 
}


bool
MIRegistrationApp
::ParseInputFile(
const char * filename )
{

  // open up the parameter file
  FILE * paramFile;
  paramFile = fopen( filename, "r" );
  if( !paramFile ) return false;

  // parse the file
  char         currentLine[150];
  unsigned int uNumber;
  float        fNumber;

  // get target file name
  if( fscanf( paramFile, "%s", currentLine ) != 1 ) return false;
  m_TargetFileName = currentLine;
  std::cout << "Target filename: " << m_TargetFileName << std::endl;

  // get target big endian
  if( fscanf( paramFile, "%d", &uNumber ) != 1) return false;
  m_TargetBigEndian = !(!uNumber);
  std::cout << "Big Endian: " << m_TargetBigEndian << std::endl;

  // get targe size
  for( int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 ) return false;
    m_TargetSize[j] = uNumber;
    } 
  std::cout << "Image Size: " << m_TargetSize; 

  // get target spacing
  std::cout << "Image Spacing: ";
  for( int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 ) return false;
    std::cout << fNumber << " "; 
    m_TargetSpacing[j] = fNumber;
    }
  std::cout << std::endl;
  std::cout << std::endl;

  // get reference file name
  if( fscanf( paramFile, "%s", currentLine ) != 1 ) return false;
  m_ReferenceFileName = currentLine;
  std::cout << "Reference filename: " << m_ReferenceFileName << std::endl;

  // get reference big endian
  if( fscanf( paramFile, "%d", &uNumber ) != 1) return false;
  m_ReferenceBigEndian = !(!uNumber);
  std::cout << "Big Endian: " << m_ReferenceBigEndian << std::endl;

  // get reference size
  for( int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 ) return false;
    m_ReferenceSize[j] = uNumber;
    } 
  std::cout << "Image Size: " << m_ReferenceSize; 

  // get reference spacing
  std::cout << "Image Spacing: ";
  for( int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 ) return false;
    std::cout << fNumber << " "; 
    m_ReferenceSpacing[j] = fNumber;
    }
  std::cout << std::endl;
  std::cout << std::endl;

  // get number of levels
  std::cout << "Number of levels: ";
  if( fscanf( paramFile, "%d", &uNumber) != 1 ) return false;
  m_NumberOfLevels = uNumber;
  std::cout << uNumber << std::endl;
  if( m_NumberOfLevels > MaxNumberOfLevels ) return false;

  // get target shrink factors
  std::cout << "Target shrink factors: ";
  for( int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 ) return false;
    std::cout << uNumber << " "; 
    m_TargetShrinkFactors[j] = uNumber;
    }
  std::cout << std::endl;
 
  // get reference shrink factors
  std::cout << "Reference shrink factors: ";
  for( int j = 0; j < ImageDimension; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 ) return false;
    std::cout << uNumber << " "; 
    m_ReferenceShrinkFactors[j] = uNumber;
    }
  std::cout << std::endl;

  // get number of iterations
  std::cout << "Number of iterations: ";
  for( unsigned int j = 0; j < m_NumberOfLevels; j++ )
    {
    if( fscanf( paramFile, "%d", &uNumber ) != 1 ) return false;
    std::cout << uNumber << " "; 
    m_NumberOfIterations[j] = uNumber;
    }
  std::cout << std::endl;

  // get learning rates
  std::cout << "Learning rates: ";
  for( unsigned int j = 0; j < m_NumberOfLevels; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 ) return false;
    std::cout << fNumber << " "; 
    m_LearningRates[j] = fNumber;
    }
  std::cout << std::endl;

  // get translation scales
  std::cout << "Translation scales: ";
  for( unsigned int j = 0; j < m_NumberOfLevels; j++ )
    {
    if( fscanf( paramFile, "%f", &fNumber ) != 1 ) return false;
    std::cout << fNumber << " "; 
    m_TranslationScales[j] = fNumber;
    }
  std::cout << std::endl;
  std::cout << std::endl;

  // get registered image output file name
  if( fscanf( paramFile, "%s", currentLine ) != 1 ) return false;
  m_RegisteredImageFileName = currentLine;
  std::cout << "Registered image filename: " << m_RegisteredImageFileName << std::endl;

  // get registered image big endian
  if( fscanf( paramFile, "%d", &uNumber ) != 1) return false;
  m_RegisteredImageBigEndian = !(!uNumber);
  std::cout << "Big Endian: " << m_RegisteredImageBigEndian << std::endl;
  std::cout << std::endl;

  // get dump pgm flag
  if( fscanf( paramFile, "%d", &uNumber ) != 1) return false;
  m_DumpPGMFiles = !(!uNumber);
  std::cout << "Dump PGM files: " << m_DumpPGMFiles << std::endl;
 
  if( m_DumpPGMFiles )
    {
    // get target directory
    if( fscanf( paramFile, "%s", currentLine ) != 1 ) return false;
    m_PGMDirectory = currentLine;

    std::cout << "PGM directory: " << m_PGMDirectory << std::endl;
    std::cout << std::endl;
    }
 
  return true;

}


bool
MIRegistrationApp
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

    // generate filename for this slice
    // NOTE: "%03d" generates a 3-digit number with leading zeros
    sprintf(filename, "%s/%s%03d.pgm", dirname, basename, k );

    // open the stream  
    std::ofstream imgStream( filename, std::ios::out | std::ios::binary );
  
    if( !imgStream.is_open() )
      {
      return false;
      }

    // write the header
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
