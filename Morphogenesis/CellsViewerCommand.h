/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewerCommand.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltk_CellsViewerCommand_h
#define __fltk_CellsViewerCommand_h

#include "itkCommand.h"
#include "CellsViewer.h"
#include "CommandEvents.h"


namespace bio {

/**
 *  Implementation of the Command Patter for invoking OpenGL redraw
 */
class CellsViewerCommand : public itk::Command
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CellsViewerCommand   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Command  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will call redraw in the GL Window
   */
  void Execute(      itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CellsViewerCommand, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
   itkNewMacro(Self);

  
  /**
   * Method for setting the pointer to the class that needs
   * to be notified
   */
   void SetCellsViewer( CellsViewer * viewer );


protected:
  /**
   * Constructor
   */
  CellsViewerCommand();


private:


  CellsViewer   * m_CellsViewer;

  
};


} // end namespace bio


#endif
