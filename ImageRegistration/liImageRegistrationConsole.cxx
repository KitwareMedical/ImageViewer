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
#include <itkFileIOMetaImage.h>
 


/************************************
 *
 *  Constructor
 *
 ***********************************/
liImageRegistrationConsole
::liImageRegistrationConsole()
{
 
  FileIOMetaImageFactory * factory = new FileIOMetaImageFactory;
  ObjectFactoryBase * factoryBase = static_cast<ObjectFactoryBase *>( factory );
  ObjectFactoryBase::RegisterFactory( factoryBase );  

  m_InputViewer           = new InputImageViewerType;
  m_ReferenceViewer       = new ImageViewerType;
  m_MappedReferenceViewer = new ImageViewerType;

  m_InputViewer->SetLabel( "Target Image" );
  m_ReferenceViewer->SetLabel( "Reference Image" );
  m_MappedReferenceViewer->SetLabel( "Mapped Reference Image" );

  m_Reader->AddObserver( itk::StartEvent(), targetButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::EndEvent(), targetButton->GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::ModifiedEvent(), targetButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), targetButton->GetRedrawCommand().GetPointer() );
  
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

  if( m_InputViewer ) 
  {
    delete m_InputViewer;
    m_InputViewer = 0;
  }

  if( m_ReferenceViewer ) 
  {
    delete m_ReferenceViewer;
    m_ReferenceViewer = 0;
  }

  if( m_MappedReferenceViewer ) 
  {
    delete m_MappedReferenceViewer;
    m_MappedReferenceViewer = 0;
  }


}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liImageRegistrationConsole
::Load( void )
{

  const char * filename = fl_file_chooser("Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading image file...");
  
  try 
  {
    liImageRegistrationConsoleBase::Load( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("File Loaded");

  controlsGroup->activate();

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
  m_InputViewer->Hide();
  m_ReferenceViewer->Hide();
  m_MappedReferenceViewer->Hide();
  aboutWindow->hide();
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
 *  Show Progress
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowProgress( float fraction )
{
  liImageRegistrationConsoleBase::ShowProgress( fraction );
  progressSlider->value( fraction );
  Fl::check();
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
 *  Show Target Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowTarget( void )
{

  if( !(m_ImageLoaded ) )
  {
    ShowStatus("Please load an image first");
    return;
  }

  m_InputViewer->SetImage( m_Reader->GetOutput() );  
  m_InputViewer->Show();

}


 
/************************************
 *
 *  Show Reference Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowReference( void )
{

  if( !(m_ImageLoaded ) )
  {
    ShowStatus("Please load an image first");
    return;
  }

  m_ReferenceViewer->SetImage( m_ReferenceImage );  
  m_ReferenceViewer->Show();

}



 
/************************************
 *
 *  Show Mapped Reference Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowMappedReference( void )
{

  if( !(m_ImageLoaded ) )
  {
    ShowStatus("Please load an image first");
    return;
  }

  m_MappedReferenceViewer->SetImage( m_MappedReferenceImage );
  m_MappedReferenceViewer->Show();

}




 
/************************************
 *
 *  Show About Window
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowAbout( void )
{

  aboutWindow->show();

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

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }


  this->ShowStatus("Registering Reference against Target ...");

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

  ParametersType transformationParameters;

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

  m_TargetMapper->GetTransform()->SetParameters(
                                              transformationParameters);


}




 
/************************************
 *
 *  Generate Reference Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
liImageRegistrationConsole
::GenerateReference( void )
{
  
  referenceButton->selection_color( FL_RED );
  referenceButton->value( 1 );
  referenceButton->redraw();
  
  liImageRegistrationConsoleBase::GenerateReference();
  
  referenceButton->selection_color( FL_GREEN );
  referenceButton->value( 1 );
  referenceButton->redraw();

}




 
/************************************
 *
 *  Generate Mapped Reference Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
liImageRegistrationConsole
::GenerateMappedReference( void )
{
  
  mappedReferenceButton->selection_color( FL_RED );
  mappedReferenceButton->value( 1 );
  mappedReferenceButton->redraw();
  
  liImageRegistrationConsoleBase::GenerateMappedReference();
  
  mappedReferenceButton->selection_color( FL_GREEN );
  mappedReferenceButton->value( 1 );
  mappedReferenceButton->redraw();

}

