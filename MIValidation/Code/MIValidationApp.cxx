/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIValidationApp.cxx
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
#include "VanderbiltModality.h"

#include "VanderbiltValidationApp.h"
#include "itkExceptionObject.h"


int main(int argc, char *argv[])
{

  if ( argc < 2 )
    {
    std::cout << "Parameter file name missing" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:  VanderbiltValdation param.file" << std::endl;
    return 1;
    }


  typedef itk::VanderbiltValidationApp<signed short> AppType;
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

  // get the vanderbilt directory name
  inputStream.getline( lineBuffer, maxChar, '\n' );
  
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find directory name." << std::endl;
    }
  app->SetImageDirectoryName( buffer );

  // get the output filename
  inputStream.getline( lineBuffer, maxChar, '\n' );
   
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find output file name. " << std::endl;
    }
  app->SetOutputFileName( buffer );
  app->SetAppendOutputFile( true );
  
  // get the investigator string
  inputStream.getline( lineBuffer, maxChar, '\n' );
  app->SetInvestigatorString( lineBuffer );

  // get the site string
  inputStream.getline( lineBuffer, maxChar, '\n' );
  app->SetSiteString( lineBuffer );

  // get the method string
  inputStream.getline( lineBuffer, maxChar, '\n' );
  app->SetMethodString( lineBuffer );

  // run registration cases one at a time
  while ( !inputStream.eof() )
    {

    unsigned int patientID, fromModality, toModality, invertTransform;
    inputStream.getline( lineBuffer, maxChar, '\n' );
   
    if ( sscanf( lineBuffer, "%u %u %u %u %s", 
      &patientID, &fromModality, &toModality, &invertTransform, buffer ) != 5 )
      {
      std::cout << "Problem with registration case: " << lineBuffer << std::endl;
      std::cout << "Usage: " << std::endl;
      std::cout << "patientID fromModality toModality invertOutputTransform parameterFilename";
      std::cout << std::endl;
      continue;
      }
    
    app->SetPatientNumber( patientID );
    app->SetFromModality( static_cast<itk::VanderbiltModality::ModalityType>( fromModality ) );
    app->SetToModality( static_cast<itk::VanderbiltModality::ModalityType>( toModality ) );
    app->SetInvertTransform( !(!invertTransform) );
    app->SetParameterFileName( buffer );

    std::cout << "Running case: patient=" << patientID;
    std::cout << " from=" << itk::VanderbiltModality::Description[fromModality];
    std::cout << " to=" << itk::VanderbiltModality::Description[toModality];
    std::cout << " invert=" << !(!invertTransform);
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

