/**
 *
 * This is an example on how to use the Iview Class within an
 * itk code.
 * All functionnalities are not describe here.
 * (see IView.h for all methods)
 * You need to include IViewGUI.cpp in your project
 *
 */

#include <itkPhysicalImage.h>
#include <itkFileIOMetaImage.h>
#include <itkFileIOToImageFilter.h>
#include <IView.h>


void main()
{
 
  /*Define the Image type*/
  typedef itk::PhysicalImage<unsigned short,3>  myImageType;
  myImageType::Pointer m_input_image = myImageType::New();
  
  /* Load the image*/
  typedef FileIOToImageFilter<myImageType> myLoaderType;
  ObjectFactoryBase::RegisterFactory(new FileIOMetaImageFactory);
  myLoaderType::Pointer m_loader = myLoaderType::New();

  std::cout << "loading image..." << std::endl;
  //Set the image file you want to load
  m_loader->SetFileToLoad("MyImageFile.mhd"); 
  m_loader->Update();
  m_input_image  = m_loader->GetOutput(); 

  std::cout << "Image loaded..." << std::endl;
  
  /*Define the Overlay type*/
  typedef itk::PhysicalImage<unsigned char,3>  myOverlayType;
  myOverlayType::Pointer m_overlay_image = myOverlayType::New();

  typedef FileIOToImageFilter<myOverlayType> myOverlayLoaderType;
  ObjectFactoryBase::RegisterFactory(new FileIOMetaImageFactory);
  myOverlayLoaderType::Pointer m_overlay_loader = myOverlayLoaderType::New();

  std::cout << "loading overlay..." << std::endl;
  //Set the overlay file you want to load
  m_overlay_loader->SetFileToLoad("MyOverlayFile.mhd"); 
  m_overlay_loader->Update();
  m_overlay_image  = m_overlay_loader->GetOutput(); 

  std::cout << "Overlay loaded..." << std::endl;
  
  /*Show the image*/
  std::cout << "Creating window..." << std::endl; 
  typedef IView<myImageType::PixelType,unsigned char> ImageViewerType;
  ImageViewerType* m_Viewer    = new ImageViewerType;
  
  m_Viewer->SetImage( m_input_image ); 
  m_Viewer->SetOverlay( m_overlay_image );
  m_Viewer->SetLabel("My Image...");
  m_Viewer->show();

  std::cout << "Window created." << std::endl; 
  Fl::run();

}
