
#ifndef __CellsViewerBase_H
#define __CellsViewerBase_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "fltkSlice3DDrawer.h"
#include "fltkDisplayGlWindowGUI.h"
#include "CellularAggregate.h"
#include <map>
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
public:
    
  typedef unsigned char                               PixelType;
  
  typedef itk::Image<PixelType, Cell::Dimension >     ImageType;
  typedef ImageType::Pointer                          ImagePointer;
 
  typedef itk::ImageFileReader< ImageType >           ImageReaderType;
  typedef ImageReaderType::Pointer                    ImageReaderPointer;
    
  typedef fltk::Slice3DDrawer<ImageType>              SliceDrawerType;
  typedef SliceDrawerType::Pointer                    SliceDrawerPointer;

  typedef std::map< std::string, ImagePointer >       SubstratesType;

  typedef std::map< std::string, SliceDrawerPointer > SubstratesDrawersType;


public:

	CellsViewerBase();

	virtual ~CellsViewerBase();

	virtual void Quit(void);
	virtual void Run(void);
	virtual void Stop(void);
	virtual void Restart(void);
  virtual void ShowDisplay(void);
  virtual void ShowSlicerControls(void);
  virtual void ShowCellularAggregateControls(void);
  virtual void HideDisplay(void);
  virtual void HideSlicerControls(void);
  virtual void HideCellularAggregateControls(void);
  virtual void SetCellsAggregate( CellularAggregate * );
  virtual clock_t GetStartTime(void) const;
  virtual void LoadSubstrate(void);
  virtual void ShowSubstrate( const char * name );

  itk::Command::Pointer GetRedrawCommand(void);
  itk::Object::Pointer GetNotifier(void);
  

protected:
  
  SubstratesType                      m_Substrates;

  SubstratesDrawersType               m_SubstrateSliceDrawer;


private:
  
	bool                                m_Stop;

  fltkDisplayGlWindowGUI              m_Display;

  CellularAggregate::Pointer          m_Cells;

  clock_t   m_StartTime;

};



} // end namespace bio

#endif
