/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMBrainStripValidationApp.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include <fstream>

#include "itkImage.h"
#include "itkExceptionObject.h"
#include "FEMBrainStripValidationApp.h"

// to use:  FEMBrainStripValidationApp U:\itk\Insight\Examples\IBSRValidation\FEMAtlasSegmentation\Inputs\input_1.txt 
typedef itk::Image<unsigned char,3> ImageType;
typedef itk::Image<unsigned char, 3> LabelImageType;
typedef itk::Image<float, 3> RealImageType;

typedef itk::fem::Element3DC0LinearHexahedronMembrane   ElementType;
typedef itk::fem::Element3DC0LinearTetrahedronMembrane   ElementType2;

typedef itk::fem::ImageMetricLoad<RealImageType,RealImageType>     ImageLoadType;
template class itk::fem::ImageMetricLoadImplementation<ImageLoadType>;
typedef ElementType::LoadImplementationFunctionPointer     LoadImpFP;
typedef ElementType::LoadType                              ElementLoadType;
typedef itk::fem::VisitorDispatcher<ElementType,ElementLoadType, LoadImpFP>   
                                                          DispatcherType;
typedef itk::fem::VisitorDispatcher<ElementType2,ElementLoadType, LoadImpFP>   
                                                          DispatcherType2;


typedef itk::FEMBrainStripValidationApp<ImageType,LabelImageType,RealImageType> AppType;

int main(int argc, char *argv[])
{

  if ( argc < 2 )
    {
    std::cout << "Parameter file name missing" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:  FEMBrainStripValidationApp param.file" << std::endl;
//    return 1;
    }

  // registering the load for both hexahedron and tetrahedron elements
  DispatcherType::RegisterVisitor((ImageLoadType*)0, 
          &(itk::fem::ImageMetricLoadImplementation<ImageLoadType>::ImplementImageMetricLoad));
  DispatcherType2::RegisterVisitor((ImageLoadType*)0, 
          &(itk::fem::ImageMetricLoadImplementation<ImageLoadType>::ImplementImageMetricLoad));


  AppType::Pointer app = AppType::New();


  // Parse the input file and set up the app
  std::ifstream inputStream( "U:\\itk\\Insight\\Examples\\IBSRValidation\\FEMAtlasSegmentation\\Inputs\\input_resample.txt" /*argv[1]*/, std::ios::in );

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

  
  // get the FEM config filename
  inputStream.getline( lineBuffer, maxChar, '\n' );
   
  if ( sscanf( lineBuffer, "%s", buffer ) != 1 )
    {
    std::cout << "Can't find FEM config file name. " << std::endl;
    return -1;
    }
  app->SetFEMConfigurationFileName( buffer );

  signed int startSlice;
  unsigned int numberOfSlices,xsize,ysize;
  char patientID[maxChar];

  // get altas information
  inputStream.getline( lineBuffer, maxChar, '\n' );

  if ( sscanf( lineBuffer, "%s %d %d %d %u", 
    patientID, &xsize, &ysize, &startSlice, &numberOfSlices ) != 5 )
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
  app->SetImageXSize(xsize);
  app->SetImageYSize(ysize);

  
  // run registration cases one at a time
  while ( !inputStream.eof() )
    {

    inputStream.getline( lineBuffer, maxChar, '\n' );
   
    if ( sscanf( lineBuffer, "%s %d %d %d %u %s", 
      patientID,&xsize, &ysize, &startSlice, &numberOfSlices, buffer  ) != 6 )
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

