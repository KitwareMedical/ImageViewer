
#ifndef ISISMatroxViewerBase_h
#define ISISMatroxViewerBase_h

#include "vtkPNGReader.h"
#include "vtkImageCast.h"

namespace ISIS 
{

class MatroxViewerBase
{

public:
  MatroxViewerBase();
  virtual ~MatroxViewerBase();
  
  /** Display the FLTK GUI interface */
  virtual void Show();

  /** Hide the FLTK GUI interface */
  virtual void Hide();
  
  /** Acquire images from the video grabber */
  virtual void Grab(); 

  /** Stop image acquisition from the grabber */
  virtual void Stop();

  /** Quit the application */
  virtual void Quit();

  /** Load image from a file */
  virtual void LoadImageFromFile();
  virtual void LoadImageFromFile(const char *);

protected:

  vtkPNGReader *  m_Reader;
  vtkImageCast *  m_ImageCast;
  bool            m_ImageIsLoaded;
};



}


#endif
