/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit (ITK)
  Module:    RegisterAD.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2000 National Library of Medicine
All rights reserved.

See COPYRIGHT.txt for copyright details.

=========================================================================*/
#include <ostream>
#include <iostream>
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkImportImageFilter.h"
#include "itkImageToImageRigidMutualInformationGradientDescentRegistration.h"
#include "vnl/vnl_math.h"

#include "OptionList.h"

#include "vtkTransform.h"
#include "vtkImageReader.h"
#include "vtkImageExport.h"
#include "vtkImageShiftScale.h"
#include "vtkImageStatistics.h"
#include "vtkImageShrink3D.h"
#include "vtkImageChangeInformation.h"
#include "vtkImageCast.h"
#include "vtkImageGradientMagnitude.h"
void print_usage();

void main(int argc, char **argv)
{
  if (argc < 2)
    {
    print_usage();
    exit(1);
    }

  OptionList options(argc, argv) ;
  std::string study1Prefix;
  std::vector<int> study1Resolution(3);
  std::vector<double> study1Spacing(3);
  std::string study2Prefix;
  std::vector<int> study2Resolution(3);
  std::vector<double> study2Spacing(3);
  std::vector<int> shrinkFactors;
  float translateScale;
  float standardDeviation;
  float learningRate;
  int numberOfIterations;
  int numberOfSamples;
  try
    {
    options.GetStringOption("study1Prefix", &study1Prefix, true);
    options.GetMultiDoubleOption("study1Spacing", &study1Spacing, true);
    options.GetMultiIntOption("study1Resolution", &study1Resolution, true);
    options.GetStringOption("study2Prefix", &study2Prefix, true);
    options.GetMultiDoubleOption("study2Spacing", &study2Spacing, true);
    options.GetMultiIntOption("study2Resolution", &study2Resolution, true);
    options.GetMultiIntOption("shrink", &shrinkFactors, true);
    translateScale = options.GetDoubleOption("translateScale", true);
    standardDeviation = options.GetDoubleOption("standardDeviation", true);
    learningRate = options.GetDoubleOption("learningRate", true);
    numberOfIterations = options.GetIntOption("numberOfIterations", true);
    numberOfSamples = options.GetIntOption("numberOfSamples", true);
    }
  catch(OptionList::RequiredOptionMissing e)
    {
    std::cerr << "Error: The '" << e.OptionTag
	      << "' option is required but missing." 
	      << std::endl ;
    exit(0) ;
    }
  

  // Read two studies
  vtkImageReader *aReader1 = vtkImageReader::New();
    aReader1->SetDataScalarTypeToUnsignedShort();
    aReader1->SetFilePrefix(study1Prefix.c_str());
    aReader1->SetDataByteOrderToBigEndian();
    aReader1->SetDataExtent(0,study1Resolution[0] - 1,
			    0,study1Resolution[1] - 1,
			    1, study1Resolution[2]);
    aReader1->SetDataSpacing(study1Spacing[0],
			     study1Spacing[1],
			     study1Spacing[2]);
    aReader1->Update();

  vtkImageCast *floatImage1 = vtkImageCast::New();
    floatImage1->SetInput(aReader1->GetOutput());
    floatImage1->SetOutputScalarTypeToFloat();

  vtkImageStatistics *stat1 = vtkImageStatistics::New();
    stat1->SetInput(floatImage1->GetOutput());
    stat1->Update();
  vtkImageShiftScale *scaleImage1 = vtkImageShiftScale::New();
    scaleImage1->SetInput(floatImage1->GetOutput());
    scaleImage1->SetScale(1.0 / stat1->GetStandardDeviationValue());
    scaleImage1->SetShift(-stat1->GetMeanValue());
    scaleImage1->Update();
      
  vtkImageChangeInformation *centerImage1 = vtkImageChangeInformation::New();
    centerImage1->SetInput(scaleImage1->GetOutput());
    centerImage1->CenterImageOn();
  vtkImageShrink3D *shrinkImage1 = vtkImageShrink3D::New();
    shrinkImage1->SetInput(centerImage1->GetOutput());
    shrinkImage1->SetShrinkFactors(shrinkFactors[0],
				   shrinkFactors[1],
				   shrinkFactors[2]);
    shrinkImage1->Update();
  // Export the original data
  int originalBounds1[6];
    shrinkImage1->GetOutput()->GetUpdateExtent(originalBounds1);
     
  int originalSizeX1 = originalBounds1[1] - originalBounds1[0] + 1;
  int originalSizeY1 = originalBounds1[3] - originalBounds1[2] + 1;
  int originalSizeZ1 = originalBounds1[5] - originalBounds1[4] + 1;

  float *rawReference = new float[originalSizeX1*originalSizeY1*originalSizeZ1];
  vtkImageExport *anExporter1 = vtkImageExport::New();
    anExporter1->SetInput(shrinkImage1->GetOutput());
    anExporter1->Export(rawReference);

  vtkImageReader *aReader2 = vtkImageReader::New();
    aReader2->SetFilePrefix(study2Prefix.c_str());
    aReader2->SetDataScalarTypeToUnsignedShort();
    aReader2->SetDataByteOrderToBigEndian();
    aReader2->SetDataExtent(0,study2Resolution[0] - 1,
			    0,study2Resolution[1] - 1,
			    1,study2Resolution[2]);
    aReader2->SetDataSpacing(study2Spacing[0],
			     study2Spacing[1],
			     study2Spacing[2]);
    aReader2->Update();
  vtkImageCast *floatImage2 = vtkImageCast::New();
    floatImage2->SetInput(aReader2->GetOutput());
    floatImage2->SetOutputScalarTypeToFloat();
  vtkImageStatistics *stat2 = vtkImageStatistics::New();
    stat2->SetInput(floatImage2->GetOutput());
    stat2->Update();
  vtkImageShiftScale *scaleImage2 = vtkImageShiftScale::New();
    scaleImage2->SetInput(floatImage2->GetOutput());
    scaleImage2->SetScale(1.0 / stat2->GetStandardDeviationValue());
    scaleImage2->SetShift(-stat2->GetMeanValue());
  vtkImageChangeInformation *centerImage2 = vtkImageChangeInformation::New();
    centerImage2->SetInput(scaleImage2->GetOutput());
    centerImage2->CenterImageOn();


  vtkImageShrink3D *shrinkImage2 = vtkImageShrink3D::New();
    shrinkImage2->SetInput(centerImage2->GetOutput());
    shrinkImage2->SetShrinkFactors(shrinkFactors[0],
				   shrinkFactors[1],
				   shrinkFactors[2]);
    shrinkImage2->Update();

  int originalBounds2[6];
    shrinkImage2->GetOutput()->GetUpdateExtent(originalBounds2);
     
  int originalSizeX2 = originalBounds2[1] - originalBounds2[0] + 1;
  int originalSizeY2 = originalBounds2[3] - originalBounds2[2] + 1;
  int originalSizeZ2 = originalBounds2[5] - originalBounds2[4] + 1;

  float *rawTarget = new float[originalSizeX2*originalSizeY2*originalSizeZ2];
  vtkImageExport *anExporter2 = vtkImageExport::New();
    anExporter2->SetInput(shrinkImage2->GetOutput());
    anExporter2->Export(rawTarget);

  // Create an ImportImageFilter filter for the reference and target
  typedef itk::ImportImageFilter<float, 3> ImportImageFilter;

  ImportImageFilter::Pointer importReference = ImportImageFilter::New();
  ImportImageFilter::Pointer importTarget    = ImportImageFilter::New();

  typedef ImportImageFilter::OutputImageType  ReferenceImageType;
  typedef ImportImageFilter::OutputImageType  TargetImageType;

  // Create a region for the reference
  itk::ImageRegion<3> region;

  ReferenceImageType::SizeType referenceSize= {{originalSizeX1,originalSizeY1,originalSizeZ1}};
  ReferenceImageType::IndexType referenceIndex = {{0,0,0}};
  ReferenceImageType::RegionType referenceRegion;

  referenceRegion.SetSize( referenceSize );
  referenceRegion.SetIndex( referenceIndex );

  importReference->SetRegion( referenceRegion );
  importReference->SetImportPointer( rawReference, originalSizeX1*originalSizeY1*originalSizeZ1, true);

  // set the parameters for the physical image
  float originalSpacing1[3];
  shrinkImage1->GetOutput()->GetSpacing(originalSpacing1);
  importReference->SetSpacing(originalSpacing1);

  float originalOrigin1[3];
  shrinkImage1->GetOutput()->GetOrigin(originalOrigin1);
  importReference->SetOrigin(originalOrigin1);

  // Now the target
  TargetImageType::SizeType targetSize = {{originalSizeX2,originalSizeY2,originalSizeZ2}};
  TargetImageType::IndexType targetIndex = {{0,0,0}};
  TargetImageType::RegionType targetRegion;

  targetRegion.SetSize( targetSize );
  targetRegion.SetIndex( targetIndex );

  importTarget->SetRegion( targetRegion );
  importTarget->SetImportPointer( rawTarget, originalSizeX2*originalSizeY2*originalSizeZ2, true);

  // set the parameters for the image
  float originalSpacing2[3];
  shrinkImage2->GetOutput()->GetSpacing(originalSpacing2);
  importTarget->SetSpacing(originalSpacing2);

  float originalOrigin2[3];
  shrinkImage2->GetOutput()->GetOrigin(originalOrigin2);
  importTarget->SetOrigin(originalOrigin2);

//-----------------------------------------------------------
// Set up the registrator
//-----------------------------------------------------------
  typedef itk::ImageToImageRigidMutualInformationGradientDescentRegistration<
    ReferenceImageType,TargetImageType> RegistrationType;

  RegistrationType::Pointer registrationMethod = RegistrationType::New();
  RegistrationType::ParametersType guess; guess.Fill(0); guess[1] = 1.0;

  std::cerr << "Enter a 7 tuple guess: ";
  std::cin >> guess;
  std::cerr << guess << std::endl;
  registrationMethod->SetParameters (guess);
  
  // connect the images
  importReference->Update();
  registrationMethod->SetReference(importReference->GetOutput());

  importTarget->Update();
  registrationMethod->SetTarget(importTarget->GetOutput());

  // set translation scale
  typedef RegistrationType::OptimizerType OptimizerType;
  typedef OptimizerType::TransformType::ParametersType ScaleType;

  ScaleType scales;
  scales.Fill( 1.0 );
  for( unsigned j = 4; j < 7; j++ )
    {
    scales[j] = 1.0 / vnl_math_sqr(translateScale);
    }

  registrationMethod->GetOptimizer()->GetTransform()->SetScale( scales );
  
  registrationMethod->SetNumberOfIterations( numberOfIterations );
  registrationMethod->SetLearningRate( learningRate );

  // set metric related parameters
  registrationMethod->GetMetric()->SetTargetStandardDeviation( standardDeviation );
  registrationMethod->GetMetric()->SetReferenceStandardDeviation( standardDeviation );
  registrationMethod->GetMetric()->SetNumberOfSpatialSamples( numberOfSamples );

  // start registration
  registrationMethod->StartRegistration();

  // get the results
  RegistrationType::ParametersType solution = 
    registrationMethod->GetParameters();

  vnl_quaternion<double> quat(solution[0],solution[1],solution[2],solution[3]);
  vnl_matrix_fixed<double,3,3> mat = quat.rotation_matrix();
  
  double result[16];
  result[0] = mat(0,0);
  result[1] = mat(0,1);
  result[2] = mat(0,2);
  result[3] = solution[4];
  result[4] = mat(1,0);
  result[5] = mat(1,1);
  result[6] = mat(1,2);
  result[7] = solution[5];
  result[8] = mat(2,0);
  result[9] = mat(2,1);
  result[10] = mat(2,2);
  result[11] = solution[6];
  result[12] = 0;
  result[13] = 0;
  result[14] = 0;
  result[15] = 1;

  vtkMatrix4x4 *resMatrix = vtkMatrix4x4::New();
    resMatrix->DeepCopy(result);

  vtkTransform *aTrans = vtkTransform::New();
  aTrans->SetMatrix(resMatrix);
  std::cerr << "\tOrientation: "
       << aTrans->GetOrientation()[0] << ", "
       << aTrans->GetOrientation()[1] << ", "
       << aTrans->GetOrientation()[2] << std::endl;

  std::cerr << "\tPosition: "
       << aTrans->GetPosition()[0] << ", "
       << aTrans->GetPosition()[1] << ", "
       << aTrans->GetPosition()[2] << std::endl;

  std::cout << solution << std::endl;

  // Write a vtk tcl script to load the data sets and register them
  ostream *fptr = new ofstream("reg.tcl", ios::out);
  *fptr << "set study1Prefix \""
	<< study1Prefix.c_str() << "\"" << endl;
  *fptr << "set study1Extent \""
	<< 0 << " " << study1Resolution[0] - 1 << " "
    	<< 0 << " " << study1Resolution[1] - 1 << " "
    	<< 1 << " " << study1Resolution[2] << "\"" << endl;
  *fptr << "set study1Spacing \""
	<< study1Spacing[0] << " "
    	<< study1Spacing[1] << " "
    	<< study1Spacing[2] << "\"" << endl;

  *fptr << "set study2Prefix \""
	<< study2Prefix.c_str() << "\"" << endl;
  *fptr << "set study2Extent \""
	<< 0 << " " << study2Resolution[0] - 1 << " "
    	<< 0 << " " << study2Resolution[1] - 1 << " "
    	<< 1 << " " << study2Resolution[2] << "\"" << endl;
  *fptr << "set study2Spacing \""
	<< study2Spacing[0] << " "
    	<< study2Spacing[1] << " "
    	<< study2Spacing[2] << "\"" << endl;

  *fptr << "set rotateX "
       << aTrans->GetOrientation()[0] << endl;
  *fptr << "set rotateY "
       << aTrans->GetOrientation()[1] << endl;
  *fptr << "set rotateZ "
       << aTrans->GetOrientation()[2] << endl;
  *fptr << "set translate \""
	<< aTrans->GetPosition()[0] << " "
	<< aTrans->GetPosition()[1] << " "
	<< aTrans->GetPosition()[2] << "\"" << endl;
    
  delete fptr;
  
  exit(0);
}

void print_usage()
{
  std::cerr << "RegisterAD $Revision: 1.5 $  $Date: 2001-09-21 00:07:19 $"  << std::endl;

  std::cerr <<  " usage: RegisterAD" << std::endl;
  std::cerr <<  "    --study1Prefix prefix" << std::endl;
  std::cerr <<  "    --study1Resolution x y z" << std::endl;
  std::cerr <<  "    --study1Spacing x y z" << std::endl;
  std::cerr <<  "    --study2Prefix prefix" << std::endl;
  std::cerr <<  "    --study2Resolution x y z" << std::endl;
  std::cerr <<  "    --study2Spacing x y z" << std::endl;
  std::cerr <<  "    --shrink i j k" << std::endl;
  std::cerr <<  "    --translateScale s" << std::endl;
  std::cerr <<  "    --numberOfIterations n" << std::endl;
  std::cerr <<  "    --learningRate r" << std::endl;
  std::cerr <<  "    --standardDeviation s" << std::endl;
  std::cerr <<  "    --numberOfSamples n" << std::endl;
}
