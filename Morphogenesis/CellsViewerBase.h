
#ifndef __CellsViewerBase_H
#define __CellsViewerBase_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "fltkSlice3DDrawer.h"
#include "fltkDisplayGlWindowGUI.h"
#include "CellularAggregate.h"
#include <time.h>



namespace bio {


/**

  \class CellsViewerBase
  \brief This class is responsible for holding the basic
  elements of a simulation that are intended to be visualized
  later. 

*/
class CellsViewerBase
{

  typedef unsigned char                               PixelType;
  
  typedef itk::Image<PixelType, Cell::Dimension >     ImageType;
  typedef ImageType::Pointer                          ImagePointer;
 
  typedef itk::ImageFileReader< ImageType >           ImageReaderType;
  typedef ImageReaderType::Pointer                    ImageReaderPointer;
    
  typedef fltk::Slice3DDrawer<ImageType>              SliceDrawerType;
  typedef SliceDrawerType::Pointer                    SliceDrawerPointer;

public:

	CellsViewerBase();

	virtual ~CellsViewerBase();

	virtual void Quit(void);
	virtual void Run(void);
	virtual void Stop(void);
	virtual void Restart(void);
  virtual void ShowDisplay(void);
  virtual void ShowSlicerControls(void);
  virtual void HideDisplay(void);
  virtual void SetCellsAggregate( CellularAggregate * );
  virtual clock_t GetStartTime(void) const;
  virtual void LoadImage(void);

  itk::Command::Pointer GetRedrawCommand(void);
  itk::Object::Pointer GetNotifier(void);
  

private:
  
	bool      m_Stop;

  fltkDisplayGlWindowGUI              m_Display;

  ImagePointer                        m_Image;

  ImageReaderPointer                  m_ImageReader;

  SliceDrawerPointer                  m_SliceDrawer;

  CellularAggregate::Pointer  m_Cells;

  clock_t   m_StartTime;

};



}; // end namespace bio

#endif
