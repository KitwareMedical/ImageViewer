/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BrainStripValidationApp.cxx
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

#include "BrainStripValidationApp.h"
#include "itkImage.h"
#include "itkExceptionObject.h"


int main(int argc, char *argv[])
{

  if ( argc < 2 )
    {
    std::cout << "Parameter file name missing" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:  BrainStripValidationApp param.file" << std::endl;
    return 1;
    }


  typedef itk::Image<unsigned short,3> ImageType;
  typedef itk::Image<unsigned char, 3> LabelImageType;
  typedef itk::Image<float, 3> RealImageType;
  typedef itk::BrainStripValidationApp<ImageType,LabelImageType,RealImageType> AppType;
  AppType::Pointer app = AppType::New();


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
  app->SetAppendOutputFile( true );

  signed int startSlice;
  unsigned int numberOfSlices;
  char patientID[maxChar];

  // get altas information
  inputStream.getline( lineBuffer, maxChar, '\n' );

  if ( sscanf( lineBuffer, "%s %d %u", 
    patientID, &startSlice, &numberOfSlices ) != 3 )
    {
    std::cout << "Problem with atlas specification: " << lineBuffer << std::endl;
    std::cout << "Usage: ";
    std::cout << "patientID startSlice numberOfSlices";
    std::cout << std::endl;
    return -1;
    }
  app->SetAtlasPatientID( patientID );
  app->SetAtlasStartSliceNumber( startSlice );
  app->SetAtlasNumberOfSlices( numberOfSlices );

  
  // run registration cases one at a time
  while ( !inputStream.eof() )
    {

    inputStream.getline( lineBuffer, maxChar, '\n' );
   
    if ( sscanf( lineBuffer, "%s %d %u %s", 
      patientID, &startSlice, &numberOfSlices, buffer  ) != 4 )
      {
      std::cout << "Problem with subject specification: " << lineBuffer << std::endl;
      std::cout << "Usage: ";
      std::cout << "patientID startSlice numberOfSlices parameterFileName";
      std::cout << std::endl;
      continue;
      }
    
    app->SetSubjectPatientID( patientID );
    app->SetSubjectStartSliceNumber( startSlice );
    app->SetSubjectNumberOfSlices( numberOfSlices );
    app->SetParameterFileName( buffer );

    std::cout << "Running case: ";
    std::cout << " atlas = " << app->GetAtlasPatientID();
    std::cout << " subject = " << app->GetSubjectPatientID();
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

