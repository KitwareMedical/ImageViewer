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


#include <liVesselsRegistrator.h>


namespace li {


//----------------------------------------------------
//
//    Creator
//
//----------------------------------------------------
VesselsRegistrator::VesselsRegistrator() 
{

  m_Vessels  = VectorTubes3D::New();

  m_Registrator = RegistrationMethod::New();

  m_Registrator->PrepareRegistration();

}




//----------------------------------------------------
//
//    Destructor
//
//----------------------------------------------------

VesselsRegistrator::~VesselsRegistrator() 
{
   
}



//----------------------------------------------------
//
//    Get  Registrator
//
//----------------------------------------------------
VesselsRegistrator::RegistrationMethod::Pointer
VesselsRegistrator::GetRegistrator( void ) 
{
  return m_Registrator;
}



//----------------------------------------------------
//
//    Get  Vessels
//
//----------------------------------------------------
VectorTubes3D::Pointer
VesselsRegistrator::GetVessels( void ) 
{
  return m_Vessels;
}



//----------------------------------------------------
//
//    Set  Vessels
//
//----------------------------------------------------
void 
VesselsRegistrator::SetVessels( VectorTubes3D * vessels ) 
{
  m_Vessels = vessels;
  m_Registrator->SetTarget( m_Vessels->GetPointSet() );
  m_Registrator->PrepareRegistration();

}



//----------------------------------------------------
//
//    Set  Fluoroscopic Image
//
//----------------------------------------------------
void 
VesselsRegistrator::SetImage( FluoroscopicImageType * image ) 
{
  m_Image = image;
  m_Registrator->SetReference( m_Image );
  m_Registrator->PrepareRegistration();

}




} // end namespace li



