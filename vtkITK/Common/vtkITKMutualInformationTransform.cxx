/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkITKMutualInformationTransform.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "vtkITKMutualInformationTransform.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkTransform.h"

#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"

#include "itkExceptionObject.h"
// turn itk exceptions into vtk errors
#undef itkExceptionMacro  
#define itkExceptionMacro(x) \
  { \
  std::ostrstream message; \
  itk::ExceptionMacroDetail::OStrStreamCleanup messageCleanup(message); \
  message << "itk::ERROR: " << this->GetNameOfClass() \
          << "(" << this << "): " x << std::ends; \
  std::cerr << message.str() << std::endl; \
  }

#undef itkGenericExceptionMacro  
#define itkGenericExceptionMacro(x) \
  { \
  std::ostrstream message; \
  itk::ExceptionMacroDetail::OStrStreamCleanup messageCleanup(message); \
  message << "itk::ERROR: " x << std::ends; \
  std::cerr << message.str() << std::endl; \
  }

// itk classes

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
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

vtkCxxRevisionMacro(vtkITKMutualInformationTransform, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKMutualInformationTransform);

//----------------------------------------------------------------------------
vtkITKMutualInformationTransform::vtkITKMutualInformationTransform()
{
  this->SourceImage=NULL;
  this->TargetImage=NULL;
  this->SourceStandardDeviation = 2.0;
  this->TargetStandardDeviation = 2.0;
  this->LearningRate = .005;
  this->TranslateScale = 64;
  this->NumberOfResolutions = 4;
  this->NumberOfIterations = 500;  
  this->NumberOfSamples = 50;
}

//----------------------------------------------------------------------------
vtkITKMutualInformationTransform::~vtkITKMutualInformationTransform()
{
  if(this->SourceImage)
    { 
    this->SourceImage->Delete();
    }
  if(this->TargetImage)
    { 
    this->TargetImage->Delete();
    }
}

//----------------------------------------------------------------------------
void vtkITKMutualInformationTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << "SourceStandardDeviation: " << this->SourceStandardDeviation  << endl;
  os << "TargetStandardDeviation: " << this->SourceStandardDeviation  << endl;
  os << "LearningRate: " << this->LearningRate  << endl;
  os << "TranslateScale: " << this->TranslateScale  << endl;
  os << "NumberOfResolutions: " << this->NumberOfResolutions  << endl;
  os << "NumberOfSamples: " << this->NumberOfSamples  << endl;
  os << "NumberOfIterations: " << this->NumberOfIterations  << endl;

  os << "SourceImage: " << this->SourceImage << "\n";
  if(this->SourceImage) 
    {
    this->SourceImage->PrintSelf(os,indent.GetNextIndent());
    }
  os << "TargetImage: " << this->TargetImage << "\n";
  if(this->TargetImage)
    { 
    this->TargetImage->PrintSelf(os,indent.GetNextIndent());
    }
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class TInput, class TOutput>
static void vtkITKMutualInformationExecute(vtkITKMutualInformationTransform *self,
                               vtkImageData *source,
                               vtkImageData *target,
                               TInput  *vtkNotUsed(dummy1),
                               TOutput *vtkNotUsed(dummy2))
                               
{
  // Declare the input and output types
  typedef itk::Image<TInput,3> InputType;
  typedef itk::Image<TOutput,3> OutputType;

  // Declare all the filters
  typedef itk::NormalizeImageFilter<InputType,OutputType> NormalizeFilter;
  typedef itk::ShrinkImageFilter<OutputType,OutputType> ShrinkFilter;
  typedef itk::ChangeInformationImageFilter<OutputType> ChangeInformationFilter;

  // Declare the registration types
  typedef itk::QuaternionRigidTransform<double> TransformType;
  typedef itk::QuaternionRigidTransformGradientDescentOptimizer OptimizerType;
  typedef itk::MutualInformationImageToImageMetric<OutputType, OutputType> MetricType;
  typedef itk::LinearInterpolateImageFunction<OutputType, double> InterpolatorType;
  typedef itk::ImageRegistrationMethod<OutputType,OutputType> RegistrationType;

  // Source
  vtkImageExport *movingVtkExporter = vtkImageExport::New();
    movingVtkExporter->SetInput(source);

  typedef itk::VTKImageImport<InputType> ImageImportType;
  ImageImportType::Pointer movingItkImporter = ImageImportType::New();

  ConnectPipelines(movingVtkExporter, movingItkImporter);

  NormalizeFilter::Pointer movingNormalize = NormalizeFilter::New();
    movingNormalize->SetInput(movingItkImporter->GetOutput());

  unsigned int sf[3];
  sf[0] = 4;
  sf[1] = 4;
  sf[2] = 1;

  ShrinkFilter::Pointer movingShrink = ShrinkFilter::New();
    movingShrink->SetInput(movingNormalize->GetOutput());
    movingShrink->SetShrinkFactors(sf);

  ChangeInformationFilter::Pointer movingChange = ChangeInformationFilter::New();
    movingChange->SetInput(movingShrink->GetOutput());
    movingChange->CenterImageOn();

  // Target
  vtkImageExport *fixedVtkExporter = vtkImageExport::New();
    fixedVtkExporter->SetInput(target);

  ImageImportType::Pointer fixedItkImporter = ImageImportType::New();

  ConnectPipelines(fixedVtkExporter, fixedItkImporter);

  NormalizeFilter::Pointer fixedNormalize = NormalizeFilter::New();
    fixedNormalize->SetInput(fixedItkImporter->GetOutput());

  sf[0] = 4;
  sf[1] = 4;
  sf[2] = 1;

  ShrinkFilter::Pointer fixedShrink = ShrinkFilter::New();
    fixedShrink->SetInput(fixedNormalize->GetOutput());
    fixedShrink->SetShrinkFactors(sf);

  ChangeInformationFilter::Pointer fixedChange = ChangeInformationFilter::New();
    fixedChange->SetInput(fixedShrink->GetOutput());
    fixedChange->CenterImageOn();

    fixedChange->Update();
    movingChange->Update();

}

//----------------------------------------------------------------------------
// Update the 4x4 matrix. Updates are only done as necessary.
 
void vtkITKMutualInformationTransform::InternalUpdate()
{

  if (this->SourceImage == NULL || this->TargetImage == NULL)
    {
    this->Matrix->Identity();
    return;
    }

  if (this->SourceImage->GetScalarType() != this->TargetImage->GetScalarType())
    {
    vtkErrorMacro (<< "Source type " << this->SourceImage->GetScalarType()
                   << "Does not match Target type" << this->TargetImage->GetScalarType());
    this->Matrix->Identity();
    return;
    }

  switch (this->SourceImage->GetScalarType())
    {
    vtkTemplateMacro5(vtkITKMutualInformationExecute, this,
                 this->SourceImage, this->TargetImage,
                 static_cast<VTK_TT *>(0),
                 static_cast<VTK_TT *>(0));
                 
    default:
      vtkGenericWarningMacro("InternalUpdate: Unknown input ScalarType");
      return;
    }
}

//------------------------------------------------------------------------
unsigned long vtkITKMutualInformationTransform::GetMTime()
{
  unsigned long result = this->vtkLinearTransform::GetMTime();
  unsigned long mtime = result;

  if (this->SourceImage)
    {
    mtime = this->SourceImage->GetMTime(); 
    if (mtime > result)
      {
      result = mtime;
      }
    }
  if (this->TargetImage)
    {
    mtime = this->TargetImage->GetMTime(); 
    if (mtime > result)
      {
      result = mtime;
      }
    }
  return result;
}
//------------------------------------------------------------------------
void vtkITKMutualInformationTransform::SetSourceImage(vtkImageData *source)
{
  if (this->SourceImage == source)
    {
    return;
    }

  if (this->SourceImage)
    {
    this->SourceImage->Delete();
    }

  source->Register(this);
  this->SourceImage = source;

  this->Modified();
}

//------------------------------------------------------------------------
void vtkITKMutualInformationTransform::SetTargetImage(vtkImageData *target)
{
  if (this->TargetImage == target)
    {
    return;
    }

  if (this->TargetImage)
    {
    this->TargetImage->Delete();
    }

  target->Register(this);
  this->TargetImage = target;
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkITKMutualInformationTransform::Inverse()
{
  vtkImageData *tmp1 = this->SourceImage;
  vtkImageData *tmp2 = this->TargetImage;
  this->TargetImage = tmp1;
  this->SourceImage = tmp2;
  this->Modified();
}

//----------------------------------------------------------------------------
vtkAbstractTransform *vtkITKMutualInformationTransform::MakeTransform()
{
  return vtkITKMutualInformationTransform::New(); 
}

//----------------------------------------------------------------------------
void vtkITKMutualInformationTransform::InternalDeepCopy(vtkAbstractTransform *transform)
{
  vtkITKMutualInformationTransform *t = (vtkITKMutualInformationTransform *)transform;

  this->SetSourceStandardDeviation(t->SourceStandardDeviation);
  this->SetTargetStandardDeviation(t->TargetStandardDeviation);
  this->SetLearningRate(t->LearningRate);
  this->SetTranslateScale(t->TranslateScale);
  this->SetNumberOfResolutions(t->NumberOfResolutions);
  this->SetNumberOfSamples(t->NumberOfSamples);    

  this->Modified();
}


