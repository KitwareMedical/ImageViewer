/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewerBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __CellsViewerBase_H
#define __CellsViewerBase_H

#include "itkImageFileReader.h"
#include "fltkSlice2DDrawer.h"
#include "fltkSlice3DDrawer.h"
#include "fltkDisplayGlWindowGUI.h"
#include "CellularAggregate.h"
#include "fltkClippingPlane3DDrawer.h"
#include <map>
#include <time.h>

namespace bio {

template <typename TInputImage>
class SliceDrawer
{
public:
  typedef int Type;
};
    

template <>
class SliceDrawer< itk::Image<unsigned char,2> > 
{
public:
  typedef fltk::Slice2DDrawer< itk::Image<unsigned char, 2> > Type;
};


template <>
class SliceDrawer< itk::Image<unsigned char,3> > 
{
public:
  typedef fltk::Slice3DDrawer< itk::Image<unsigned char, 3> > Type;
};

template <>
class SliceDrawer< itk::Image<double,2> > 
{
public:
  typedef fltk::Slice2DDrawer< itk::Image<double, 2> > Type;
};


template <>
class SliceDrawer< itk::Image<double,3> > 
{
public:
  typedef fltk::Slice3DDrawer< itk::Image<double, 3> > Type;
};

/** \class CellsViewerBase
 * \brief This class is responsible for holding the basic elements of a
 * simulation that are intended to be visualized later.  */
class CellsViewerBase
{
public:
  typedef CellularAggregate::SubstrateType            ImageType;
  typedef ImageType::Pointer                          ImagePointer;
 
  typedef itk::ImageFileReader< ImageType >           ImageReaderType;
  typedef ImageReaderType::Pointer                    ImageReaderPointer;
    
  typedef SliceDrawer< ImageType >::Type              SliceDrawerType;
  typedef SliceDrawerType::Pointer                    SliceDrawerPointer;

  typedef fltk::ClippingPlane3DDrawer                 ClippingPlaneDrawerType;
  typedef ClippingPlaneDrawerType::Pointer            ClippingPlaneDrawerPointer;

  typedef std::map< std::string, ImagePointer >       SubstratesType;

  typedef std::map< std::string, SliceDrawerPointer > SubstratesDrawersType;

  typedef std::map< std::string, ClippingPlaneDrawerPointer > ClippingPlaneDrawersType;

  typedef Cell * (EggProducerFunction)(void);

  typedef std::vector< EggProducerFunction * >        SpeciesContainerType;

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
  virtual void HideClippingPlaneControls(void);
  virtual void HideCellularAggregateControls(void);
  virtual void SetCellsAggregate( CellularAggregate * );
  virtual clock_t GetStartTime(void) const;
  virtual void LoadSubstrate(void);
  virtual void ShowSubstrate( const char * name );
  virtual void ShowClippingPlane( const char * name );
  virtual void ClearAggregate(void);
  virtual void AddSpeciesEggProducer( EggProducerFunction * ); 
  virtual void ShowSpecies( void );
  virtual void SelectSpecies( unsigned int );
  virtual void AddClippingPlane();
  virtual void RemoveClippingPlane();
  virtual void ExportDrawing( void ) const;
  virtual void ExportXFIG( void ) const;
  
  SpeciesContainerType & GetSpecies(void);

  itk::Command * GetRedrawCommand(void);
  itk::Object  * GetNotifier(void);
  
protected:
  SubstratesType                      m_Substrates;
  SubstratesDrawersType               m_SubstrateSliceDrawer;
  ClippingPlaneDrawersType            m_ClippingPlaneDrawer;

private:
  bool                                m_Stop;
  fltkDisplayGlWindowGUI              m_Display;
  CellularAggregate::Pointer          m_Cells;
  SpeciesContainerType                m_SpeciesProducer;
  clock_t                             m_StartTime;
};

} // end namespace bio

#endif
