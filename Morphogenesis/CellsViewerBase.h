
#ifndef __CellsViewerBase_H
#define __CellsViewerBase_H

#include "itkLightObject.h"
#include "fltkDisplayGlWindowFlatGUI.h"
#include "Cell.h"

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

	CellsViewerBase();

	virtual ~CellsViewerBase();

	virtual void Quit(void);
	virtual void Run(void);
	virtual void Stop(void);
	virtual void Resume(void);
  virtual void ShowDisplay(void);
  virtual void HideDisplay(void);
  virtual void SetCellsAggregate( Cell::CellsListType * );

  itk::Command::Pointer GetRedrawCommand(void);
  itk::LightObject::Pointer GetNotifier(void);
  

private:
  
	bool     m_Stop;

  fltkDisplayGlWindowFlatGUI  m_Display;

  Cell::CellsListType * m_Cells;

};



}; // end namespace bio

#endif
