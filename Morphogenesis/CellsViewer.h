
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

  typedef CellsViewerBase::SubstratesType        SubstratesType;
  typedef CellsViewerBase::SubstratesDrawersType SubstratesDrawersType;

  CellsViewer();

  virtual ~CellsViewer();

  virtual void Quit(void);
  virtual void Run(void);
  virtual void Stop(void);
  virtual void Restart(void);
  virtual void Show(void);
  virtual void ShowControls(void);
  virtual void ShowSpecies(void);
  virtual void HideControls(void);
  virtual void HideSlicerControls(void);
  virtual void HideClippingPlaneControls(void);
  virtual void LoadSubstrate(void);
  virtual void AddSpeciesEggProducer( EggProducerFunction *, const char *); 
  virtual void AddClippingPlane( void );

private:
  
};



} // end namespace bio

#endif
