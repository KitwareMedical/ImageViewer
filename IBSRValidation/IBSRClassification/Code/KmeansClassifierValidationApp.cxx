/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    KmeansClassifierValidationApp.cxx
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
#include <stdio.h>

#include "KmeansClassifierValidationApp.h"
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

  const unsigned int NumberOfChannels = 1;
  const unsigned int ImageDimension   = 3;

  typedef itk::Image< itk::Vector<unsigned short,NumberOfChannels>,
    ImageDimension > VectorInputImageType;

  typedef itk::Image< unsigned char, ImageDimension > 
    BrainMaskImageType;

  typedef itk::KmeansClassifierValidationApp< VectorInputImageType,
                                              BrainMaskImageType > AppType;

  AppType::Pointer app = AppType::New();

  //Set the number of labels (set 1 label for background = 0)
  unsigned int numberOfLabels = 4;

  //Set the labels in the truth (brain segmentation data set)
  //each labels should correspond to the array index that is associated
  //with the tisse type provided in the parameter files.
  typedef itk::Array<unsigned int> IntArrayType;

  IntArrayType truthLabels(numberOfLabels);
  truthLabels[0] = 0; truthLabels[1] = 128; 
  truthLabels[2] = 192; truthLabels[3] = 254;

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

  // get the IBSR "20Normal_T1" directory name
  inputStream.getline( lineBuffer, maxChar, '\n' );
  
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find image directory name." << std::endl;
    return -1;
    }
  app->SetImageDirectoryName( buffer );

  // get the IBSR "20Normal_T1_brain" directory name
  inputStream.getline( lineBuffer, maxChar, '\n' );
  
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find brain mask directory name." << std::endl;
    return -1;
    }
  app->SetBrainMaskDirectoryName( buffer );

  // get the IBSR "20Normal_T1_seg" directory name
  inputStream.getline( lineBuffer, maxChar, '\n' );
  
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find brain segmentation directory name." << std::endl;
    return -1;
    }
  app->SetBrainSegmentationDirectoryName( buffer );

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

  signed int startSlice;
  unsigned int numberOfSlices;
  signed int startSegSlice;
  unsigned int numberOfSegSlices;
  char patientID[maxChar];

  // run classification cases one at a time
  while ( !inputStream.eof() )
    {

    inputStream.getline( lineBuffer, maxChar, '\n' );
   
    if ( sscanf( lineBuffer, "%s %d %u %d %u %s", 
      patientID, &startSlice, &numberOfSlices, &startSegSlice, &numberOfSegSlices, buffer  ) != 6 )
      {
      std::cout << "Problem with subject specification: " << lineBuffer << std::endl;
      std::cout << "Usage: ";
      std::cout << "patientID startSlice numberOfSlices startSegSlices numberOfSegSlices parameterFileName";
      std::cout << std::endl;
      continue;
      }
    
    app->SetPatientID( patientID );
    app->SetStartSliceNumber( startSlice );
    app->SetNumberOfSlices( numberOfSlices );
    app->SetStartSegSliceNumber( startSegSlice );
    app->SetNumberOfSegSlices( numberOfSegSlices );
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

