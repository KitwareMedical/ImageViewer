/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIRegistration.cxx
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

#include "vtk2itk2vtk.h"

// vtk classes
#include "vtkImageReader.h"
#include "vtkImageExport.h"
#include "vtkTransform.h"

// itk classes
#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkNormalizeImageFilter.h"
#include "itkChangeInformationImageFilter.h"
#include "itkShrinkImageFilter.h"
#include "itkQuaternionRigidTransform.h"
#include "itkQuaternionRigidTransformGradientDescentOptimizer.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegistrationMethod.h"
#include "vnl/vnl_math.h"
#include "OptionList.h"

void print_usage();

int main(int argc, char **argv)
{
  unsigned int sf[3];
  if (argc < 2)
    {
    print_usage();
    exit(1);
    }

  // Process Options
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
  
  // Declare all the filters
  typedef itk::Image<unsigned short,3> InputType;
  typedef itk::Image<float,3> OutputType;
  typedef itk::NormalizeImageFilter<InputType,OutputType> NormalizeFilter;
  typedef itk::ChangeInformationImageFilter<OutputType> ChangeInformationFilter;
  typedef itk::ShrinkImageFilter<OutputType,OutputType> ShrinkFilter;

  // Registration types
  typedef itk::QuaternionRigidTransform<double> TransformType;
  typedef itk::QuaternionRigidTransformGradientDescentOptimizer OptimizerType;
  typedef itk::MutualInformationImageToImageMetric<OutputType, OutputType> MetricType;
  typedef itk:: LinearInterpolateImageFunction<OutputType, double> InterpolatorType;
  typedef itk::ImageRegistrationMethod<OutputType,OutputType> RegistrationType;


  // Moving ----------------
  vtkImageReader *movingReader = vtkImageReader::New();
    movingReader->SetDataScalarTypeToUnsignedShort();
    movingReader->SetFilePrefix(study1Prefix.c_str());
    movingReader->SetDataByteOrderToBigEndian();
    movingReader->SetDataExtent(0, study1Resolution[0] - 1,
                                0, study1Resolution[1] - 1,
                                1, study1Resolution[2]);
    movingReader->SetDataSpacing(study1Spacing[0],
                                 study1Spacing[1],
                                 study1Spacing[2]);
  vtkImageExport *movingVtkExporter = vtkImageExport::New();
    movingVtkExporter->SetInput(movingReader->GetOutput());

  typedef itk::VTKImageImport<InputType> ImageImportType;
  ImageImportType::Pointer movingItkImporter = ImageImportType::New();

  ConnectPipelines(movingVtkExporter, movingItkImporter);

  NormalizeFilter::Pointer movingNormalize = NormalizeFilter::New();
    movingNormalize->SetInput(movingItkImporter->GetOutput());

  sf[0] = shrinkFactors[0];
  sf[1] = shrinkFactors[1];
  sf[2] = shrinkFactors[2];
  ShrinkFilter::Pointer movingShrink = ShrinkFilter::New();
    movingShrink->SetInput(movingNormalize->GetOutput());
    movingShrink->SetShrinkFactors(sf);

  ChangeInformationFilter::Pointer movingChange = ChangeInformationFilter::New();
    movingChange->SetInput(movingShrink->GetOutput());
    movingChange->CenterImageOn();

  // Fixed ----------------
   vtkImageReader *fixedReader = vtkImageReader::New();
    fixedReader->SetDataScalarTypeToUnsignedShort();
    fixedReader->SetFilePrefix(study2Prefix.c_str());
    fixedReader->SetDataByteOrderToBigEndian();
    fixedReader->SetDataExtent(0, study2Resolution[0] - 1,
                               0, study2Resolution[1] - 1,
                               1, study2Resolution[2]);
    fixedReader->SetDataSpacing(study2Spacing[0],
                                study2Spacing[1],
                                study2Spacing[2]);

  vtkImageExport *fixedVtkExporter = vtkImageExport::New();
    fixedVtkExporter->SetInput(fixedReader->GetOutput());

  ImageImportType::Pointer fixedItkImporter = ImageImportType::New();

  ConnectPipelines(fixedVtkExporter, fixedItkImporter);

  NormalizeFilter::Pointer fixedNormalize = NormalizeFilter::New();
    fixedNormalize->SetInput(fixedItkImporter->GetOutput());

  sf[0] = shrinkFactors[0];
  sf[1] = shrinkFactors[1];
  sf[2] = shrinkFactors[2];
  ShrinkFilter::Pointer fixedShrink = ShrinkFilter::New();
    fixedShrink->SetInput(fixedNormalize->GetOutput());
    fixedShrink->SetShrinkFactors(sf);

  ChangeInformationFilter::Pointer fixedChange = ChangeInformationFilter::New();
    fixedChange->SetInput(fixedShrink->GetOutput());
    fixedChange->CenterImageOn();
  try
    {
    fixedChange->Update();
    movingChange->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception detected: "  << e;
    return -1;
    }

//-----------------------------------------------------------
// Set up the registrator
//-----------------------------------------------------------
  MetricType::Pointer         metric        = MetricType::New();
  TransformType::Pointer      transform     = TransformType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();
  RegistrationType::ParametersType guess(transform->GetNumberOfParameters() );
  guess.Fill(0); guess[1] = 1.0;

  // The guess is: a quaternion followed by a translation
  std::cerr << "Enter a 7 tuple guess: ";
  std::cin >> guess;
  std::cerr << guess << std::endl;
  registration->SetInitialTransformParameters (guess);
  
  // Set translation scale
  typedef OptimizerType::ScalesType ScaleType;

  ScaleType scales(transform->GetNumberOfParameters());
  scales.Fill( 1.0 );
  for( unsigned j = 4; j < 7; j++ )
    {
    scales[j] = 1.0 / vnl_math_sqr(translateScale);
    }

  // Set metric related parameters
  metric->SetMovingImageStandardDeviation( standardDeviation );
  metric->SetFixedImageStandardDeviation( standardDeviation );
  metric->SetNumberOfSpatialSamples( numberOfSamples );

  // Connect up the components
  registration->SetMetric(metric);
  registration->SetOptimizer(optimizer);
  registration->SetTransform(transform);
  registration->SetInterpolator(interpolator);
  registration->SetFixedImage(fixedChange->GetOutput());
  registration->SetMovingImage(movingChange->GetOutput());

  // Setup the optimizer
  optimizer->SetScales(scales);
  optimizer->MaximizeOn();

  optimizer->SetNumberOfIterations( numberOfIterations );
  optimizer->SetLearningRate( learningRate );

  // Start registration

 registration->StartRegistration();

  // Get the results
  RegistrationType::ParametersType solution = 
    registration->GetLastTransformParameters();

  vnl_quaternion<double> quat(solution[0],solution[1],solution[2],solution[3]);
  vnl_matrix_fixed<double,3,3> mat = quat.rotation_matrix();
  
  // Convert the vnl matrix to a vtk mtrix
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

  std::cout << solution[0] << " " 
            << solution[1] << " " 
            << solution[2] << " " 
            << solution[3] << " " 
            << solution[4] << " " 
            << solution[5] << " " 
            << solution[6] << std::endl;

  // Write a vtk tcl script to load the data sets and register them
  std::ofstream fptr("reg.tcl", ios::out);
  fptr << "set study1Prefix \""
        << study1Prefix.c_str() << "\"" << std::endl;
  fptr << "set study1Extent \""
        << 0 << " " << study1Resolution[0] - 1 << " "
        << 0 << " " << study1Resolution[1] - 1 << " "
        << 1 << " " << study1Resolution[2] << "\"" << std::endl;
  fptr << "set study1Spacing \""
        << study1Spacing[0] << " "
        << study1Spacing[1] << " "
        << study1Spacing[2] << "\"" << std::endl;

  fptr << "set study2Prefix \""
        << study2Prefix.c_str() << "\"" << std::endl;
  fptr << "set study2Extent \""
        << 0 << " " << study2Resolution[0] - 1 << " "
        << 0 << " " << study2Resolution[1] - 1 << " "
        << 1 << " " << study2Resolution[2] << "\"" << std::endl;
  fptr << "set study2Spacing \""
        << study2Spacing[0] << " "
        << study2Spacing[1] << " "
        << study2Spacing[2] << "\"" << std::endl;

  fptr << "set rotateX "
       << aTrans->GetOrientation()[0] << std::endl;
  fptr << "set rotateY "
       << aTrans->GetOrientation()[1] << std::endl;
  fptr << "set rotateZ "
       << aTrans->GetOrientation()[2] << std::endl;
  fptr << "set translate \""
        << aTrans->GetPosition()[0] << " "
        << aTrans->GetPosition()[1] << " "
        << aTrans->GetPosition()[2] << "\"" << std::endl;
    
  
  return 0;
}

void print_usage()
{
  std::cerr << "RegisterAD $Revision: 1.1 $  $Date: 2002-05-30 02:05:56 $"  << std::endl;

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
