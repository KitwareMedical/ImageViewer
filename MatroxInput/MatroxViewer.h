
#ifndef ISISMatroxViewer_h
#define ISISMatroxViewer_h



#include "MatroxViewerGUI.h"

// VTK Includes
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageActor.h"
#include "vtkVideoSource.h"
#include "vtkImageMapper.h"



namespace ISIS 
{

class MatroxViewer : public MatroxViewerGUI
{

public:
  MatroxViewer();
  ~MatroxViewer();
  void Show();
  void Hide();
  void Grab(); 
  void Stop();
  void Quit();
  void LoadImageFromFile();
  

private:

  vtkRenderWindow * m_RenderWindow;
  vtkRenderer     * m_Renderer;
  vtkVideoSource  * m_VideoSource;
  vtkImageMapper  * m_ImageMapper;
  vtkImageActor   * m_ImageActor;

};



}



#endif
