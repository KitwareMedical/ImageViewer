
#ifndef __CellsViewer_H
#define __CellsViewer_H

#include "CellsViewerGUI.h"

namespace bio {


/**

  \class CellsViewer
  \brief This class is responsible for holding the basic
  elements of a simulation that are intended to be visualized
  later. 

*/
class CellsViewer : public CellsViewerGUI
{


public:

	CellsViewer();

	virtual ~CellsViewer();

	virtual void Quit(void);
	virtual void Run(void);
	virtual void Stop(void);
	virtual void Restart(void);
  virtual void Show(void);
  virtual void ShowControls(void);
  virtual void HideControls(void);

private:
  
};



}; // end namespace bio

#endif
