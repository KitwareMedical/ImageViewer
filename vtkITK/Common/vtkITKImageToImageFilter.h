// .NAME vtkITKImageToImageFilter - Abstract base class for connecting ITK and VTK
// .SECTION Description
// vtkITKImageToImageFilter provides a foo

#ifndef __vtkITKImageToImageFilter_h
#define __vtkITKImageToImageFilter_h


#include "vtkImageToImageFilter.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "itkCommand.h"
#include "itkProcessObject.h"

#undef itkExceptionMacro  
#define itkExceptionMacro(x) \
  { \
  std::ostrstream message; \
  itk::ExceptionMacroDetail::OStrStreamCleanup messageCleanup(message); \
  message << "itk::ERROR: " << this->GetNameOfClass() \
          << "(" << this << "): " x << std::ends; \
  std::cerr << message.str() << std::endl; \
  }

class VTK_EXPORT vtkITKImageToImageFilter : public vtkImageToImageFilter
{
public:
  static vtkITKImageToImageFilter *New()
   {
     return new vtkITKImageToImageFilter;
   };
  
  vtkTypeMacro(vtkITKImageToImageFilter,vtkImageToImageFilter);

  void PrintSelf(ostream& os, vtkIndent indent)
  {
    Superclass::PrintSelf ( os, indent );
    this->vtkExporter->PrintSelf ( os, indent );
    this->vtkImporter->PrintSelf ( os, indent );
  };
  
  // Description:
  // This method considers the sub filters MTimes when computing this objects
  // modified time.
  unsigned long int GetMTime()
  {
    unsigned long int t1, t2;
  
    t1 = this->Superclass::GetMTime();
    t2 = this->vtkExporter->GetMTime();
    if (t2 > t1)
      {
      t1 = t2;
      }
    t2 = this->vtkImporter->GetMTime();
    if (t2 > t1)
      {
      t1 = t2;
      }
    return t1;
  };

  // Description:
  // Pass modified message to sub filters.
  void Modified()
  {
    this->vtkExporter->Modified();
    this->vtkImporter->Modified();
  };
  
  // Description:
  // Pass DebugOn.
  void DebugOn()
  {
    this->m_Process->DebugOn();
  };
  
  // Description:
  // Pass DebugOff.
  void DebugOff()
  {
    this->m_Process->DebugOff();
  };
  
  // Description:
  // This method returns the cache to make a connection
  // It justs feeds the request to the sub filter.
  virtual vtkImageData *GetOutput() { return this->vtkImporter->GetOutput(); };
  virtual vtkImageData *GetOutput(int idx)
  {
    return (vtkImageData *) this->vtkImageSource::GetOutput(idx);
  };
  
  // Description:
  // Set the Input of the filter.
  virtual void SetInput(vtkImageData *Input)
  {
    this->vtkProcessObject::SetNthInput(0, Input);
    this->vtkExporter->SetInput(Input);
  };

protected:
  vtkITKImageToImageFilter()
  {
    // Need an import, export, and a ITK pipeline
    this->vtkExporter = vtkImageExport::New();
    this->vtkImporter = vtkImageImport::New();
    this->m_Process = NULL;
    this->m_ProgressCommand = ProgressMemberCommand::New();
    this->m_ProgressCommand->SetCallbackFunction ( this, &vtkITKImageToImageFilter::GetProgressFromITKFilter );
  };
  ~vtkITKImageToImageFilter()
  {
    this->vtkExporter->Delete();
    this->vtkImporter->Delete();
  };

  //BTX
  void GetProgressFromITKFilter ()
  {
    if ( this->m_Process )
      {
      this->UpdateProgress ( m_Process->GetProgress() );
      }
  };
  
  void LinkITKProgressToVTKProgress ( itk::ProcessObject* process )
  {
    this->m_Process = process;
    this->m_Process->AddObserver ( itk::ProgressEvent(), this->m_ProgressCommand );
  };

  typedef itk::SimpleMemberCommand<vtkITKImageToImageFilter> ProgressMemberCommand;
  typedef ProgressMemberCommand::Pointer ProgressMemberCommandPointer;
  
  itk::ProcessObject::Pointer m_Process;
  ProgressMemberCommandPointer m_ProgressCommand;
  
  // ITK Progress object
  // To/from VTK
  vtkImageImport* vtkImporter;
  vtkImageExport* vtkExporter;  
  //ETX
  
  // Dummy ExecuteData
  void ExecuteData (vtkDataObject *) {};
  
private:
  vtkITKImageToImageFilter(const vtkITKImageToImageFilter&);  // Not implemented.
  void operator=(const vtkITKImageToImageFilter&);  // Not implemented.
};

// vtkCxxRevisionMacro(vtkITKImageToImageFilter, "$Revision: 1.2 $" );
// template <class InputType, class OutputType >
// template <class InputType, class OutputType >
// vtkStandardNewMacro(vtkITKImageToImageFilter);

#endif




