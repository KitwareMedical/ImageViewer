/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsole.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <liImageRegistrationConsole.h>
#include <FL/fl_file_chooser.H>
 



/************************************
 *
 *  Constructor
 *
 ***********************************/
liImageRegistrationConsole
::liImageRegistrationConsole()
{
 
  m_MovingImageViewer.SetLabel( "Moving Image" );
  m_FixedImageViewer.SetLabel( "Fixed Image" );
  m_InputMovingImageViewer.SetLabel( "Input Moving Image" );
  m_MappedMovingImageViewer.SetLabel( "Mapped Moving Image" );

  progressSlider->Observe( m_ResampleMovingImageFilter.GetPointer() );
  progressSlider->Observe( m_ResampleInputMovingImageFilter.GetPointer() );

  fixedImageButton->Observe(        m_FixedImageReader.GetPointer()  );
  loadFixedImageButton->Observe(    m_FixedImageReader.GetPointer()  );
  inputMovingImageButton->Observe(  m_MovingImageReader.GetPointer() );
  loadMovingImageButton->Observe(   m_MovingImageReader.GetPointer() );

  this->ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liImageRegistrationConsole
::~liImageRegistrationConsole()
{

}



 
/************************************
 *
 *  Load Moving Image
 *
 ***********************************/
void
liImageRegistrationConsole
::LoadMovingImage( void )
{

  const char * filename = fl_file_chooser("Moving Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading moving image file...");
  
  try 
  {
    liImageRegistrationConsoleBase::LoadMovingImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("Moving Image Loaded");

  controlsGroup->activate();

}


 
/************************************
 *
 *  Load Fixed Image
 *
 ***********************************/
void
liImageRegistrationConsole
::LoadFixedImage( void )
{

  const char * filename = fl_file_chooser("Fixed Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading fixed image file...");
  
  try 
  {
    liImageRegistrationConsoleBase::LoadFixedImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("Fixed Image Loaded");

  controlsGroup->activate();

}

   


 
/************************************
 *
 *  Save Moving Image
 *
 ***********************************/
void
liImageRegistrationConsole
::SaveMovingImage( void )
{

  const char * filename = fl_file_chooser("Moving Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving moving image file...");
  
  try 
  {
    liImageRegistrationConsoleBase::SaveMovingImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems saving file format");
    return;
  }

  this->ShowStatus("Moving Image Saved");

}





/************************************
 *
 *  Show
 *
 ***********************************/
void
liImageRegistrationConsole
::Show( void )
{
  consoleWindow->show();
}





/************************************
 *
 *  Hide
 *
 ***********************************/
void
liImageRegistrationConsole
::Hide( void )
{
  consoleWindow->hide();
  m_FixedImageViewer.Hide();
  m_MovingImageViewer.Hide();
  m_InputMovingImageViewer.Hide();
  m_MappedMovingImageViewer.Hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
liImageRegistrationConsole
::Quit( void )
{
  this->Hide();
}






 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowStatus( const char * message )
{
  liImageRegistrationConsoleBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Fixed Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowFixedImage( void )
{

  if( !m_FixedImageIsLoaded )
    {
    return;
    }

  m_FixedImageViewer.SetImage( m_FixedImageReader->GetOutput() );  
  m_FixedImageViewer.Show();
}


 
/************************************
 *
 *  Show Input Moving Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowInputMovingImage( void )
{

  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  m_InputMovingImageViewer.SetImage( m_MovingImageReader->GetOutput() );  
  m_InputMovingImageViewer.Show();
}



 
/************************************
 *
 *  Show Moving Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowMovingImage( void )
{

  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  this->GenerateMovingImage();

  m_MovingImageViewer.SetImage( m_ResampleInputMovingImageFilter->GetOutput() );  
  m_MovingImageViewer.Show();

}



 
/************************************
 *
 *  Show Mapped Moving Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowMappedMovingImage( void )
{

  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  m_MappedMovingImageViewer.SetImage( m_ResampleMovingImageFilter->GetOutput() );
  m_MappedMovingImageViewer.Show();
}







 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
liImageRegistrationConsole
::Execute( void )
{

  this->ShowStatus("Registering Moving Image against Fixed Image ...");

  liImageRegistrationConsoleBase::Execute();

  this->ShowStatus("Registration done ");
  
}




 
/************************************
 *
 *  Update the parameters of the 
 *  Transform
 *
 ***********************************/
void
liImageRegistrationConsole
::UpdateTransformParameters( void )
{

  typedef   itk::AffineTransform<double,3> TransformType;

  TransformType::Pointer affineTransform = TransformType::New();

  TransformType::OffsetType           offset;
  TransformType::OutputVectorType     axis;

  const double  angle = angleRotation->value() * atan( 1.0 ) / 45.0 ;

  axis[0] = xRotation->value();
  axis[1] = yRotation->value();
  axis[2] = zRotation->value();

  offset[0] = xTranslation->value();
  offset[1] = yTranslation->value();
  offset[2] = zTranslation->value();

  affineTransform->Rotate3D( axis, angle );
  affineTransform->SetOffset( offset );

  TransformType::MatrixType matrix;

  matrix = affineTransform->GetMatrix();
  offset = affineTransform->GetOffset();

  std::cout << "Matrix = " << matrix << std::endl;
  std::cout << "Offset = " << offset << std::endl;

  const unsigned long numberOfParamenters = 
                             affineTransform->GetNumberOfParameters();

  TransformParametersType  transformationParameters( numberOfParamenters );

  unsigned int counter = 0;

  for(unsigned int i=0; i<ImageDimension; i++)
  {
    for(unsigned int j=0; j<ImageDimension; j++)
    {
      transformationParameters[counter++] = matrix[i][j];
    }
  }

  for(unsigned int k=0; k<ImageDimension; k++)
  {
    transformationParameters[counter++] = offset[k];
  }



}




 
/************************************
 *
 *  Generate Moving Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
liImageRegistrationConsole
::GenerateMovingImage( void )
{
  
  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  movingImageButton->selection_color( FL_RED );
  movingImageButton->value( 1 );
  movingImageButton->redraw();
  
  AffineTransformType::OffsetType offset;

  offset[0] = xTranslation->value();
  offset[1] = yTranslation->value();
  offset[2] = zTranslation->value();

  AffineTransformType::OutputVectorType axis;
  AffineTransformType::ScalarType       angle;
  
  angle   = angleRotation->value() * atan( 1.0 ) / 45.0 ;
  axis[0] = xRotation->value();
  axis[1] = yRotation->value();
  axis[2] = zRotation->value();

  m_InputTransform->SetIdentity();
  m_InputTransform->SetOffset( offset );
  m_InputTransform->Rotate3D( axis, angle );

  liImageRegistrationConsoleBase::GenerateMovingImage();
  
  movingImageButton->selection_color( FL_GREEN );
  movingImageButton->value( 1 );
  movingImageButton->redraw();

}




 
/************************************
 *
 *  Generate Mapped Moving Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
liImageRegistrationConsole
::GenerateMappedMovingImage( void )
{
  
  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  mappedMovingImageButton->selection_color( FL_RED );
  mappedMovingImageButton->value( 1 );
  mappedMovingImageButton->redraw();
  
  liImageRegistrationConsoleBase::GenerateMappedMovingImage();
  
  mappedMovingImageButton->selection_color( FL_GREEN );
  mappedMovingImageButton->value( 1 );
  mappedMovingImageButton->redraw();

}

