/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCurvatureFlowToVTK.cxx
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

#include <itkCommand.h>
#include <itkImage.h>
#include <itkVTKImageExport.h>
#include <itkRandomImageSource.h>
#include <itkCurvatureFlowImageFilter.h>

#include <vtkImageImport.h>
#include <vtkImageShiftScale.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

struct ProgressDisplay
{
  ProgressDisplay(itk::ProcessObject* process): m_Process(process) {}
  
  void Display()
    {
    float progress = m_Process->GetProgress()*100.0;
    std::cout << "Progress " << progress << " percent." << std::endl;
    }
  
  itk::ProcessObject::Pointer m_Process;
};

int main()
{  
  typedef itk::Image<float, 2> ImageType;
  typedef itk::RandomImageSource<ImageType> ImageSourceType;
  typedef itk::CurvatureFlowImageFilter<ImageType, ImageType> DenoiserType;
  typedef itk::VTKImageExport<ImageType> ImageExportType;  
  
  unsigned long size[2] = {256, 256};
  ImageSourceType::Pointer source = ImageSourceType::New();
  source->SetSize(size);
  source->SetMin(0);
  source->SetMax(1);

  DenoiserType::Pointer denoiser = DenoiserType::New();
  denoiser->SetInput(source->GetOutput());
  denoiser->SetTimeStep(0.15);
  denoiser->SetNumberOfIterations(8);

  ProgressDisplay progressDisplay(denoiser);
  itk::SimpleMemberCommand<ProgressDisplay>::Pointer progressEvent =
    itk::SimpleMemberCommand<ProgressDisplay>::New();
  progressEvent->SetCallbackFunction(&progressDisplay,
                                     &ProgressDisplay::Display);
  denoiser->AddObserver(itk::Command::ProgressEvent, progressEvent);
  
  
  ImageExportType::Pointer exporter = ImageExportType::New();
  exporter->SetInput(denoiser->GetOutput());
  
  vtkImageImport* importer = vtkImageImport::New();
  
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());

  vtkImageShiftScale* shifter = vtkImageShiftScale::New();
  shifter->SetInput(importer->GetOutput());
  shifter->SetScale(256);
  shifter->SetOutputScalarTypeToUnsignedChar();

  vtkRenderer* renderer = vtkRenderer::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();

  renWin->AddRenderer(renderer);
  iren->SetRenderWindow(renWin);

  renderer->SetBackground(0.4392, 0.5020, 0.5647);
  renWin->SetSize(500, 500);

  vtkImageActor* actor = vtkImageActor::New();
  actor->SetInput(shifter->GetOutput());
  renderer->AddActor(actor);

  renWin->Render();
  iren->Start();
  source->SetMin(0.5);
  iren->Start();
  
  iren->Delete();
  renWin->Delete();
  renderer->Delete();
  actor->Delete();
  shifter->Delete();
  importer->Delete();
  
  return 0;
}
