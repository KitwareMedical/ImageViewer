/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewerCommand.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "CellsViewerCommand.h"
#include "itkProcessObject.h"
#include "fltkCommandEvents.h"


namespace bio {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CellsViewerCommand
::CellsViewerCommand(void)  
{
}



//--------------------------------------------------
//
//    Connect the Bacterial Colony
//
//--------------------------------------------------
void
CellsViewerCommand
::SetCellsViewer( CellsViewer * viewer )
{
  m_CellsViewer = viewer;
}



//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CellsViewerCommand
::Execute( const itk::Object * caller, const itk::EventObject & event )
{

  CellularAggregate::ConstPointer aggregate(
      dynamic_cast< const CellularAggregate * >( caller ) );

  if( typeid( event ) == typeid( bio::TimeStepEvent ) )
    {
    if( m_CellsViewer )
      {
      m_CellsViewer->timeValueOutput->value( 
                      ( clock() - m_CellsViewer->GetStartTime() ) / CLOCKS_PER_SEC  );
      m_CellsViewer->iterationValueOutput->value( 
                                   m_CellsViewer->iterationValueOutput->value() + 1 );
      m_CellsViewer->numberOfCellsValueOutput->value( aggregate->GetNumberOfCells() );
      }
    }

}



//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CellsViewerCommand
::Execute( itk::Object * caller, const itk::EventObject & event )
{
  const itk::Object * constcaller = caller;
  Execute( constcaller, event );
}





} // end namespace li


