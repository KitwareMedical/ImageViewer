/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralSegmentRegistrator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <liVertebralSegmentRegistrator.h>


namespace li {


//----------------------------------------------------
//
//    Creator
//
//----------------------------------------------------
VertebralSegmentRegistrator::VertebralSegmentRegistrator() 
{

  m_VertebralSegment  = VertebralSegment::New();

  m_VolumeRegistrator = VolumeRegistrationMethod::New();

}




//----------------------------------------------------
//
//    Destructor
//
//----------------------------------------------------

VertebralSegmentRegistrator::~VertebralSegmentRegistrator() 
{
   
}




//----------------------------------------------------
//
//    Get Vertebral Segment
//
//----------------------------------------------------
VertebralSegment::Pointer 
VertebralSegmentRegistrator::GetVertebralSegment( void ) 
{
  return m_VertebralSegment;
}



//----------------------------------------------------
//
//    Get Volume Registrator
//
//----------------------------------------------------
VertebralSegmentRegistrator::VolumeRegistrationMethod::Pointer
VertebralSegmentRegistrator::GetVolumeRegistrator( void ) 
{
  return m_VolumeRegistrator;
}



} // end namespace li



