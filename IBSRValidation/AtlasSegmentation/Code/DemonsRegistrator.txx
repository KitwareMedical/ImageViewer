/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DemonsRegistrator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _DemonsRegistrator_txx
#define _DemonsRegistrator_txx

#include "DemonsRegistrator.h"

#include "itkCommand.h"


namespace itk
{


template <typename TFixedImage, typename TMovingImage, typename TFieldValue>
DemonsRegistrator<TFixedImage,TMovingImage,TFieldValue>
::DemonsRegistrator()
{
  // Images need to be set from the outside
  m_FixedImage  = NULL;
  m_MovingImage = NULL;
  m_DeformationField = NULL;

  // Set up internal registrator with default components
  m_FixedImagePyramid  = FixedImagePyramidType::New();
  m_MovingImagePyramid = MovingImagePyramidType::New();
  m_Registration       = RegistrationType::New();

  m_Registration->SetFixedImagePyramid( m_FixedImagePyramid );
  m_Registration->SetMovingImagePyramid( m_MovingImagePyramid );


  // Setup an registration observer
  typedef SimpleMemberCommand<Self> CommandType;
  typename CommandType::Pointer command = CommandType::New();
  command->SetCallbackFunction( this, &Self::StartNewLevel );

  m_Tag = m_Registration->AddObserver( IterationEvent(), command );


  // Default parameters
  m_NumberOfLevels = 1;

  m_FixedImageShrinkFactors.Fill( 1 );
  m_MovingImageShrinkFactors.Fill( 1 );

  m_NumberOfIterations = UnsignedIntArray(1);
  m_NumberOfIterations.Fill( 10 );
    
}


template <typename TFixedImage, typename TMovingImage, typename TFieldValue>
DemonsRegistrator<TFixedImage,TMovingImage,TFieldValue>
::~DemonsRegistrator()
{

  m_Registration->RemoveObserver( m_Tag );

}


template <typename TFixedImage, typename TMovingImage, typename TFieldValue>
void
DemonsRegistrator<TFixedImage,TMovingImage,TFieldValue>
::Execute()
{

  // Setup the image pyramids
  m_FixedImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
  m_FixedImagePyramid->SetStartingShrinkFactors( 
    m_FixedImageShrinkFactors.GetDataPointer() );

  m_MovingImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
  m_MovingImagePyramid->SetStartingShrinkFactors(
    m_MovingImageShrinkFactors.GetDataPointer() );

  // Setup the registrator
  m_Registration->SetFixedImage( m_FixedImage );
  m_Registration->SetMovingImage( m_MovingImage );
  m_Registration->SetNumberOfLevels( m_NumberOfLevels );
  m_Registration->SetNumberOfIterations( m_NumberOfIterations.data_block() );

  try
    {
    m_Registration->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cout << "Caught an exception: " << std::endl;
    std::cout << err << std::endl;
    throw err;
    }

  m_DeformationField = m_Registration->GetOutput();
  m_DeformationField->DisconnectPipeline();
}


template <typename TFixedImage, typename TMovingImage, typename TFieldValue>
void
DemonsRegistrator<TFixedImage,TMovingImage,TFieldValue>
::StartNewLevel()
{

  std::cout << "--- Starting level " << m_Registration->GetCurrentLevel()
            << std::endl;

}


} // namespace itk


#endif
