
#ifndef __CellsViewerBase_H
#define __CellsViewerBase_H

#include "fltkDisplayGlWindowFlatGUI.h"

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

private:
  
	bool     m_Stop;

  fltkDisplayGlWindowFlatGUI  m_Display;

};



}; // end namespace bio

#endif
