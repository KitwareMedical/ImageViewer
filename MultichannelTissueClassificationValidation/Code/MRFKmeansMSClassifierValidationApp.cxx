/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MRFKmeansMSClassifierValidationApp.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>
#include <fstream>

#include "MRFKmeansMSClassifierValidationApp.h"
#include "itkImage.h"
#include "itkArray.h"
#include "itkExceptionObject.h"


int main(int argc, char *argv[])
{

  if ( argc < 2 )
    {
    std::cout << "Parameter file name missing" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:  KMeansClassifierValidationApp param.file" << std::endl;
    return 1;
    }

  const unsigned int NumberOfChannels = 2;
  const unsigned int ImageDimension   = 3;

  typedef itk::Image< itk::Vector<unsigned short,NumberOfChannels>,
    ImageDimension > VectorInputImageType;

  typedef itk::Image< unsigned char, ImageDimension > 
    BrainMaskImageType;

  typedef itk::MRFKmeansMSClassifierValidationApp< VectorInputImageType,
                                              BrainMaskImageType > AppType;

  AppType::Pointer app = AppType::New();

  //Set the number of labels (set 1 label for background = 0)
  unsigned int numberOfLabels = 5;

  //Set the labels in the truth (brain segmentation data set)
  //each labels should correspond to the array index that is associated
  //with the tisse type provided in the parameter files.
  typedef itk::Array<unsigned int> IntArrayType;

  IntArrayType truthLabels(numberOfLabels);
  truthLabels[0] = 0; truthLabels[1] = 1; 
  truthLabels[2] = 2; truthLabels[3] = 3;
  truthLabels[4] = 4;

  app->SetTruthLabels( truthLabels );

  // Parse the input file and set up the app
  std::ifstream inputStream( argv[1], std::ios::in );

  if ( !inputStream.is_open() )
    {
    std::cout << "Can't open parameter file: " << argv[1] << std::endl;
    return -1;
    }

  const unsigned int maxChar = 512;
  char lineBuffer[maxChar];
  char buffer[maxChar];

  // get the MS directory name
  inputStream.getline( lineBuffer, maxChar, '\n' );
  
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find image directory name." << std::endl;
    return -1;
    }
  app->SetImageDirectoryName( buffer );

  // get the MS filename extensions

  for( unsigned int i = 0; i < NumberOfChannels; i++ )
    {
    inputStream.getline( lineBuffer, maxChar, '\n' );
  
    if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
      {
      std::cout << "Can't find the file extension directory name." << std::endl;
      return -1;     
      }
    
    app->AddFileExtension( buffer );

    }

  // get the output filename
  inputStream.getline( lineBuffer, maxChar, '\n' );
   
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find output file name. " << std::endl;
    return -1;
    }
  app->SetOutputFileName( buffer );

  // get the output filename
  inputStream.getline( lineBuffer, maxChar, '\n' );

  unsigned int numberOfChannels;   
  if ( sscanf( lineBuffer, "%d", &numberOfChannels ) != 1 )
    {
    std::cout << "Can't find number of channels. " << std::endl;
    return -1;
    }

  app->SetNumberOfChannels( numberOfChannels );

  app->SetAppendOutputFile( true );

  app->SetNumberOfClasses( numberOfLabels );

  //Set the MRF parameters
  app->SetMaximumNumberOfIterations(1);

  app->SetErrorTolerance(0.2);

  app->SetNeighborhoodRadius( 1 );

  app->SetSmoothingFactor( 0.5 );


  unsigned int numberOfSlices;
//  signed int startSlice;
//  signed int startSegSlice;
//  unsigned int numberOfSegSlices;
  char patientID[maxChar];

  // run classification cases one at a time
  while ( !inputStream.eof() )
    {

    inputStream.getline( lineBuffer, maxChar, '\n' );
   
    if ( sscanf( lineBuffer, "%s %u %s", 
      patientID, &numberOfSlices, buffer  ) != 3 )
      {
      std::cout << "Problem with subject specification: " << lineBuffer << std::endl;
      std::cout << "Usage: ";
      std::cout << "patientID numberOfSlices parameterFileName";
      std::cout << std::endl;
      continue;
      }
    
    app->SetPatientID( patientID );
    app->SetNumberOfSlices( numberOfSlices );
    app->SetParameterFileName( buffer );

    std::cout << " subject = " << app->GetPatientID();
    std::cout << std::endl;

    try
      {
      app->Execute();
      }
    catch( itk::ExceptionObject& err)
      {
      std::cout << "Caught an ITK exception: " << std::endl;
      std::cout << err << std::endl;
      }
    catch(...)
      {
      std::cout << "Caught an non-ITK exception " << std::endl;
      }

    }

  return 0;

}

