/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewerCommand.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
::Execute( const itk::LightObject * caller, unsigned long event )
{

  CellularAggregate::ConstPointer aggregate(
      dynamic_cast< const CellularAggregate * >( caller ) );

  if( event == bio::TimeStepEvent )
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
::Execute( itk::LightObject * caller, unsigned long event )
{
  const itk::LightObject * constcaller = caller;
  Execute( constcaller, event );
}





} // end namespace li


