
#include "MatroxViewerBase.h"



namespace ISIS 
{

MatroxViewerBase
::MatroxViewerBase() 
{
  m_Reader     = vtkPNGReader::New();
  m_ImageCast  = vtkImageCast::New();
  m_ImageCast->SetInput( m_Reader->GetOutput() );
  m_ImageCast->SetOutputScalarTypeToUnsignedChar();
  m_ImageIsLoaded = false;
}



MatroxViewerBase
::~MatroxViewerBase() 
{
  m_ImageCast->Delete();
  m_Reader->Delete();
}



void 
MatroxViewerBase
::Show()
{
}


void 
MatroxViewerBase
::Hide()
{

}


void 
MatroxViewerBase
::Grab()
{

}


void 
MatroxViewerBase
::Stop()
{
}


void 
MatroxViewerBase
::Quit()
{
}



void 
MatroxViewerBase
::LoadImageFromFile()
{
}



void 
MatroxViewerBase
::LoadImageFromFile(const char * filename)
{
  m_Reader->SetFileName( filename );
  m_Reader->Update();
  m_ImageIsLoaded = true;
}



}

