#ifndef __RegisterApplication_h
#define __RegisterApplication_h

#include "itkImage.h"
#include "itkPNGImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "Preprocessor.h"
#include "MIEuler2DRegistrator.h"
#include "MISimilarity2DRegistrator.h"
#include "MIAffine2DRegistrator.h"
#include "OutputGenerator.h"

class RegisterApplication
{
public:

  enum { ImageDimension = 2 } ;

  typedef unsigned short PixelType ;
  typedef itk::Image< PixelType, ImageDimension > InputImageType ;
  typedef itk::Image< float, ImageDimension > OutputImageType ;
  typedef itk::PNGImageIO ImageIOType ;
  typedef itk::ImageFileReader< InputImageType > ImageReaderType ;
  typedef itk::ImageFileWriter< InputImageType > ImageWriterType ;
  typedef itk::FlipImageFilter< InputImageType > ImageFlipperType ;

  typedef Preprocessor< InputImageType, OutputImageType > PreprocessorType ;

//  typedef MIEuler2DRegistrator< OutputImageType, OutputImageType > RegistratorType ;
//  typedef MIAffine2DRegistrator< OutputImageType, OutputImageType > RegistratorType ;
  typedef MISimilarity2DRegistrator< OutputImageType, OutputImageType > RegistratorType ;

  typedef PreprocessorType::AffineTransformType TransformType ;
  typedef OutputGenerator< InputImageType > GeneratorType ;

  typedef enum 
  {
    fixedImage,
    movingImage,
    registeredImage,
    mixedChannel
  } DisplayIdentifier;

  RegisterApplication() ;
  virtual ~RegisterApplication() {}

  virtual void Execute() ;

protected:

  virtual void LoadFixedImage() ;
  virtual void LoadMovingImage() ;
  virtual void SaveRegisteredImage(const char* filename) ;
  virtual void UpdateParameters() {}
  virtual void Preprocess() ;
  virtual void Register() ;
  virtual void ComputeTransform() ;
  virtual void GenerateOutput() ;
  virtual void ShowFixedImage() {}   
  virtual void ShowMovingImage() {} 
  virtual void ShowRegisteredImage() {} 
  virtual void ShowMixedChannel() {}
  virtual void SelectDisplay(DisplayIdentifier displayId) ;
  virtual void ShowDisplay() ;

protected:

  std::string m_FixedImageFileName ;
  std::string m_MovingImageFileName ;
  itk::FixedArray< unsigned int, ImageDimension > m_PermutationOrder ;
  itk::FixedArray< bool, ImageDimension > m_FlipAxes ;

  InputImageType::RegionType m_SelectedRegion ;
  InputImageType::SizeType m_SelectedSize ;
  InputImageType::IndexType m_SelectedIndex ;

  int m_NumberOfIterations ;
  double m_LearningRate ;
  double m_TranslationScale ;
  double m_RotationScale ;
  std::vector< int > m_RegionBegin ;
  std::vector< int > m_RegionEnd ;
  std::string m_DirectoryName ;

  InputImageType::Pointer m_FixedImage ;
  InputImageType::Pointer m_MovingImage ;
  InputImageType::Pointer m_RegisteredImage ;

  DisplayIdentifier m_Display ;

  bool m_FixedImageLoaded ;
  bool m_MovingImageLoaded ;
  bool m_RegisteredImageAvailable ;

  ImageReaderType::Pointer m_FixedImageReader ;
  ImageReaderType::Pointer m_MovingImageReader ;
  ImageWriterType::Pointer m_RegisteredImageWriter ;
  ImageFlipperType::Pointer m_FixedImageFlipper ;
  ImageFlipperType::Pointer m_MovingImageFlipper ;
  ImageFlipperType::Pointer m_RegisteredImageFlipper ;
  PreprocessorType::Pointer m_Preprocessor ;
  RegistratorType::Pointer m_Registrator ;
  TransformType::Pointer m_Transform ;
  GeneratorType::Pointer m_Generator ;
} ;

#endif




