/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandTubesGeneratePointSet.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkShape3D.h"
#include "liCommandTubesGeneratePointSet.h"
#include <FL/Fl.H>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandTubesGeneratePointSet::CommandTubesGeneratePointSet(void)  
{
}



//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandTubesGeneratePointSet::Execute( itk::Object * unit, 
                                        const itk::EventObject & event )
{
  
  if( typeid( event ) == typeid( FluoroscopyUnitMovedEvent ) ||
      typeid( event ) == typeid( PatientMovedEvent         ) ||
      typeid( event ) == typeid( TableMovedEvent           )   )
  {
    if( m_FluoroscopyUnit && m_Tubes )
    {

      itk::Point<double,3>  sourcePosition;
      itk::Point<double,3>  detectorPosition;
      
      sourcePosition   = m_FluoroscopyUnit->GetSourcePosition();
      detectorPosition = m_FluoroscopyUnit->GetDetectorPosition();
      
      itk::Vector<double,3> lineOfSigth = detectorPosition - sourcePosition;

      m_Tubes->GeneratePointSet( lineOfSigth );

    }
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandTubesGeneratePointSet::Execute(const itk::Object * unit, 
                                      const itk::EventObject & event )
{

  if( typeid( event ) == typeid( FluoroscopyUnitMovedEvent )   ||
      typeid( event ) == typeid( PatientMovedEvent         )   ||
      typeid( event ) == typeid( TableMovedEvent           )    )
  {
    if( m_FluoroscopyUnit && m_Tubes )
    {

      itk::Point<double,3>  sourcePosition;
      itk::Point<double,3>  detectorPosition;
      
      sourcePosition   = m_FluoroscopyUnit->GetSourcePosition();
      detectorPosition = m_FluoroscopyUnit->GetDetectorPosition();
      
      itk::Vector<double,3> lineOfSigth = detectorPosition - sourcePosition;

      m_Tubes->GeneratePointSet( lineOfSigth );

    }
  }

}




} // end namespace li


