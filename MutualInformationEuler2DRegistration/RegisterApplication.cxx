#include "RegisterApplication.h"
#include "itkMetaImageIOFactory.h"
#include "itkPNGImageIOFactory.h"

RegisterApplication::RegisterApplication()
{
  m_FixedImageReader = ImageReaderType::New() ;
  m_MovingImageReader = ImageReaderType::New() ;
  m_RegisteredImageWriter = ImageWriterType::New() ;

  m_FixedImageFlipper = ImageFlipperType::New() ;
  m_MovingImageFlipper = ImageFlipperType::New() ;
  m_RegisteredImageFlipper = ImageFlipperType::New() ;
  
  ImageFlipperType::FlipAxesArrayType flipAxes ;
  flipAxes[0] = false ;
  flipAxes[1] = true ;
  m_FixedImageFlipper->SetFlipAxes(flipAxes) ;
  m_MovingImageFlipper->SetFlipAxes(flipAxes) ;
  m_RegisteredImageFlipper->SetFlipAxes(flipAxes) ;

  m_FixedImageLoaded = false ;
  m_MovingImageLoaded = false ;
  m_RegisteredImageAvailable = false ;
  m_DirectoryName = "output" ;

  m_PermutationOrder[0] = 0 ;
  m_PermutationOrder[1] = 1 ;

  m_FlipAxes[0] = 0 ;
  m_FlipAxes[1] = 0 ;

  m_Display = fixedImage ;

  m_Preprocessor = PreprocessorType::New() ;

  m_Registrator = RegistratorType::New() ;

  m_Transform = TransformType::New() ;

  m_Generator = GeneratorType::New() ;

  m_NumberOfIterations = 100 ;
  m_LearningRate = 0.0001 ;
  m_TranslationScale = 0.00001 ;
  m_RotationScale = 1 ;

  // Register a producer of MetaImage and PNG readers
  itk::MetaImageIOFactory::RegisterOneFactory();
  itk::PNGImageIOFactory::RegisterOneFactory();

}

void RegisterApplication::LoadFixedImage()
{
  m_FixedImageReader->SetFileName(m_FixedImageFileName.c_str()) ;
  m_FixedImageReader->Update() ;
  m_FixedImageFlipper->SetInput(m_FixedImageReader->GetOutput()) ;
  m_FixedImageFlipper->Update() ;
  m_FixedImage = m_FixedImageFlipper->GetOutput() ;
  m_FixedImageLoaded = true ;
}

void RegisterApplication::LoadMovingImage()
{
  m_MovingImageReader->SetFileName(m_MovingImageFileName.c_str()) ;
  m_MovingImageReader->Update() ;
  m_MovingImageFlipper->SetInput(m_MovingImageReader->GetOutput()) ;
  m_MovingImageFlipper->Update() ;
  m_MovingImage = m_MovingImageFlipper->GetOutput() ;
  m_MovingImageLoaded = true ;
}

void RegisterApplication::SaveRegisteredImage(const char* filename)
{
  if( !filename )
  {
    return;
  }

  m_RegisteredImageFlipper->SetInput(m_RegisteredImage) ;
  m_RegisteredImageFlipper->Update() ;
  m_RegisteredImageWriter->SetInput(m_RegisteredImageFlipper->GetOutput()) ;
  m_RegisteredImageWriter->SetFileName( filename );
  m_RegisteredImageWriter->Write();
}

void RegisterApplication::Preprocess()
{
  m_Preprocessor->SetInputFixedImage( m_FixedImage );
  m_Preprocessor->SetInputMovingImage( m_MovingImage );
  
  m_Preprocessor->SetPermuteOrder( m_PermutationOrder );
  m_Preprocessor->SetFlipAxes( m_FlipAxes );

  m_Preprocessor->Execute() ;
}

void RegisterApplication::Register()
{
  m_Registrator->SetFixedImage( m_Preprocessor->GetOutputFixedImage() );
  m_Registrator->SetMovingImage( m_Preprocessor->GetOutputMovingImage() );
  m_Registrator->SetNumberOfIterations( m_NumberOfIterations );
  m_Registrator->SetLearningRates( m_LearningRate );
  m_Registrator->SetRotationScale(m_RotationScale) ;
  m_Registrator->SetTranslationScale(m_TranslationScale);
  
  m_Registrator->Execute() ;
}

void RegisterApplication::ComputeTransform()
{
  // Get the results
  m_Registrator->GetTransformParameters() ;
  // compose the preprocess and registration transforms
  m_Transform->SetIdentity();
  m_Transform->Compose( m_Preprocessor->GetPostTransform(), true );
  m_Transform->Compose( m_Registrator->GetAffineTransform(), true );
  m_Transform->Compose( m_Preprocessor->GetPreTransform(), true );

  m_Transform->GetMatrix() ;
  m_Transform->GetOffset() ;
}

void RegisterApplication::GenerateOutput()
{
  m_Generator->SetFixedImage( m_FixedImage );
  m_Generator->SetMovingImage( m_MovingImage );
  m_Generator->SetTransform( m_Transform );
  m_Generator->SetDirectoryName( m_DirectoryName.c_str() );

  m_Generator->Execute();
  
  m_RegisteredImage = m_Generator->GetResampledImage() ;
  m_RegisteredImageAvailable = true ;
}

void RegisterApplication::Execute()
{
  m_Registrator->SetFixedImageRegion(m_SelectedRegion) ;
  this->Preprocess() ;
  this->Register() ;
  this->ComputeTransform() ;
  this->GenerateOutput() ;
}

void RegisterApplication::SelectDisplay(DisplayIdentifier displayId)
{
  m_Display = displayId ;
}

void RegisterApplication::ShowDisplay()
{
  switch( m_Display )
    {
    case fixedImage :
      {
        this->ShowFixedImage() ; 
        break ;
      }
    case movingImage :
      {
        this->ShowMovingImage() ;
        break ;
      }

    case registeredImage :
      {
        this->ShowRegisteredImage() ;
        break ;
      }

    case mixedChannel :
      {
        this->ShowMixedChannel() ;
        break ;
      }
    default:
      return ;
    }
}








