/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralSampleGenerator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include "liVertebralSampleGenerator.h"
#include "liSampleGeneratorPedicle.h"
#include "liSampleGeneratorBodyPlane.h"
#include "liCommandEvents.h"


namespace li {


//----------------------------------------------------
//
//    Creator
//
//----------------------------------------------------
VertebralSampleGenerator::VertebralSampleGenerator() 
{

  m_LeftPedicleGenerator = new liSampleGeneratorPedicle();
  m_LeftPedicleGenerator->SetName("Left Pedicle");
  m_SampleGenerator.Add( m_LeftPedicleGenerator );

  m_RightPedicleGenerator = new liSampleGeneratorPedicle();
  m_RightPedicleGenerator->SetName("Right Pedicle");
  m_SampleGenerator.Add( m_RightPedicleGenerator );

  m_BottomBodyGenerator = new liSampleGeneratorBodyPlane();
  m_BottomBodyGenerator->SetName("Body Botton");
  m_SampleGenerator.Add( m_BottomBodyGenerator );

  m_TopBodyGenerator = new liSampleGeneratorBodyPlane();
  m_TopBodyGenerator->SetName("Body Top");
  m_SampleGenerator.Add( m_TopBodyGenerator );

  m_PointSet = PointSetType::New();

}




//----------------------------------------------------
//
//    Destructor
//
//----------------------------------------------------

VertebralSampleGenerator::~VertebralSampleGenerator() 
{
   
}




//----------------------------------------------------
//
//    Get the samples in a PointSet structure
//
//----------------------------------------------------
VertebralSampleGenerator::PointSetType::ConstPointer 
VertebralSampleGenerator::GetPointSet( void ) const
{
  return m_PointSet.GetPointer();
}


//----------------------------------------------------
//
//    Show the Sample Generator
//
//----------------------------------------------------
void VertebralSampleGenerator::Show(void ) 
{
  m_SampleGenerator.Show();
}







//----------------------------------------------------
//
//    Hide the Sample Generator
//
//----------------------------------------------------
void VertebralSampleGenerator::Hide(void ) 
{
  m_SampleGenerator.Hide();
}





//----------------------------------------------------
//
//    Generate the Samples
//
//----------------------------------------------------
void VertebralSampleGenerator::GenerateSamples(void ) 
{
  UpdateParameters(); 
  m_SampleGenerator.GenerateSamples( m_Samples );
  TransferSamplesToPointSet();
  m_VertebralSegment->InvokeEvent( li::PatientMovedEvent() );
  
}






//----------------------------------------------------
//
//    Update Sample Generator Parameters
//
//----------------------------------------------------
void VertebralSampleGenerator::UpdateParameters(void ) 
{
  
  if( !m_VertebralSegment ) 
  {
    std::cerr << "Vertebral Sample Generator has no VertebralSegment connected"
      << std::endl;
    return;
  }

  liSample::PointType position;

  const li::VertebralBody    * body     = 
                                m_VertebralSegment->GetVertebralBody();

  const li::VertebralPedicle * pedicles = 
                                m_VertebralSegment->GetVertebralPedicle();

  position[0] = 0;
  position[1] = 0;
  position[2] = 0;

  const double lrx = body->GetLowerEndXRadius();
  const double lry = body->GetLowerEndYRadius();

  const double bottomBodyRadius = m_BottomBodyGenerator->GetRadius();

  m_BottomBodyGenerator->SetPosition( position );
  m_BottomBodyGenerator->SetScaleX( lrx/bottomBodyRadius );
  m_BottomBodyGenerator->SetScaleY( lry/bottomBodyRadius );
  


  position[0] = 0;
  position[1] = 0;
  position[2] = body->GetHeight();

  const double urx = body->GetUpperEndXRadius();
  const double ury = body->GetUpperEndYRadius();

  const double topBodyRadius = m_TopBodyGenerator->GetRadius();

  m_TopBodyGenerator->SetPosition( position );
  m_TopBodyGenerator->SetScaleX( urx/topBodyRadius );
  m_TopBodyGenerator->SetScaleY( ury/topBodyRadius );
  

  const double lprx = pedicles->GetHorizontalRadius();
  const double lpry = pedicles->GetVerticalRadius();

  const double leftPedicleRadius = m_LeftPedicleGenerator->GetRadius();

  position[0] = pedicles->GetHorizontalPosition();
  position[1] = pedicles->GetLength()/2.0 + pedicles->GetFrontalPosition();
  position[2] = pedicles->GetVerticalPosition();
  m_LeftPedicleGenerator->SetPosition( position );
  m_LeftPedicleGenerator->SetScaleX( lprx / leftPedicleRadius );
  m_LeftPedicleGenerator->SetScaleY( lpry / leftPedicleRadius );
  

  position[0] = -position[0];
  const double rightPedicleRadius = m_RightPedicleGenerator->GetRadius();
  
  m_RightPedicleGenerator->SetPosition( position );
  m_RightPedicleGenerator->SetScaleX( lprx / rightPedicleRadius );
  m_RightPedicleGenerator->SetScaleY( lpry / rightPedicleRadius );
  
}



//----------------------------------------------------
//
//    Set Vertebral Segment
//
//----------------------------------------------------
void 
VertebralSampleGenerator::SetVertebralSegment( VertebralSegment * segment )
{ 
  m_VertebralSegment = segment; 

  m_VertebralSegment->GetLeftPediclePointSet()->SetPointSet(
                      m_LeftPedicleGenerator->GetPointSet() ); 

  m_VertebralSegment->GetRightPediclePointSet()->SetPointSet(
                      m_RightPedicleGenerator->GetPointSet() ); 
  
  m_VertebralSegment->GetTopBodyPointSet()->SetPointSet(
                      m_TopBodyGenerator->GetPointSet() ); 
  
  m_VertebralSegment->GetBottomBodyPointSet()->SetPointSet(
                      m_BottomBodyGenerator->GetPointSet() ); 

  UpdateParameters(); 
}





//----------------------------------------------------
//
//    Set Vertebral Segment
//
//----------------------------------------------------
void 
VertebralSampleGenerator::TransferSamplesToPointSet( void )
{ 
  
  PointSetType::PointsContainer::Pointer     
                  points = PointSetType::PointsContainer::New();

  PointSetType::PointDataContainer::Pointer  
                  values = PointSetType::PointDataContainer::New();

  SetOfSamplesContainerType::const_iterator  samples 
    = m_Samples.begin();

  unsigned long numberOfSamples = 0;

  while( samples != m_Samples.end() )
  {
    numberOfSamples += samples->size();
    samples++;
  }

  points->Reserve( numberOfSamples );
  values->Reserve( numberOfSamples );
  


  // Cumulative Transfer from each subset
  samples = m_Samples.begin(); 

  while( samples != m_Samples.end() )
  { 

    PointSetType::PointsContainer::Iterator      pi = points->Begin();
    PointSetType::PointDataContainer::Iterator   vi = values->Begin();

    SamplesContainerType::const_iterator it = samples->begin();
    while( it != samples->end() )
    {
      pi.Value() = it->point;
      vi.Value() = it->value;
      ++it;
      ++pi;
      ++vi;
    }

    samples++;
  }
  
  m_PointSet->SetPoints( points );
  m_PointSet->SetPointData( values );

}


} // end namespace li



