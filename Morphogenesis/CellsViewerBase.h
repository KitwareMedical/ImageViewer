
#ifndef __CellsViewerBase_H
#define __CellsViewerBase_H

#include "itkObject.h"
#include "fltkDisplayGlWindowFlatGUI.h"
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


public:

	CellsViewerBase();

	virtual ~CellsViewerBase();

	virtual void Quit(void);
	virtual void Run(void);
	virtual void Stop(void);
	virtual void Restart(void);
  virtual void ShowDisplay(void);
  virtual void HideDisplay(void);
  virtual void SetCellsAggregate( CellularAggregate * );
  virtual clock_t GetStartTime(void) const;

  itk::Command::Pointer GetRedrawCommand(void);
  itk::Object::Pointer GetNotifier(void);
  

private:
  
	bool      m_Stop;

  fltkDisplayGlWindowFlatGUI  m_Display;

  CellularAggregate::Pointer  m_Cells;

  clock_t   m_StartTime;

};



}; // end namespace bio

#endif
