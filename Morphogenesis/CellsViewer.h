/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __CellsViewer_H
#define __CellsViewer_H

#include "CellsViewerGUI.h"
#include <string>

namespace bio {

/** \class CellsViewer
 * \brief This class is responsible for holding the basic elements of a
 * simulation that are intended to be visualized later.  */
class CellsViewer : public CellsViewerGUI
{
public:
  typedef CellsViewerBase::SubstratesType        SubstratesType;
  typedef CellsViewerBase::SubstratesDrawersType SubstratesDrawersType;

  CellsViewer();

  virtual ~CellsViewer();

  virtual void Quit();
  virtual void Run();
  virtual void Stop();
  virtual void Restart();
  virtual void Show();
  virtual void ShowControls();
  virtual void ShowSpecies();
  virtual void HideControls();
  virtual void HideSlicerControls();
  virtual void HideClippingPlaneControls();
  virtual void LoadSubstrate();
  virtual void AddSpeciesEggProducer( EggProducerFunction *, const char *); 
  virtual void AddClippingPlane();

  virtual void SetCellsAggregate( CellularAggregate * );

private:
  

};



} // end namespace bio

#endif
