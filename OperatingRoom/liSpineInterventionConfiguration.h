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

#ifndef liSPINEINTERVENTIONCONFIGURATIONCLASS
#define liSPINEINTERVENTIONCONFIGURATIONCLASS


#if defined(_WIN32)
#include "itkWindows.h"
#endif

#include "liOperatingRoomModel.h"


/**
 *  \warning A change in order on these include files 
 *   generated and ICE Internal Compiler Error on VC++
 */
#include <liSurgeryTableGUI.h>
#include <liOperatingRoomViewGUI.h>
#include <itkImageFileReader.h>
#include <liSpineModelGUI.h>
#include <fltkDisplayGlWindowGUI.h>
#include <fltkSlice3DDrawer.h>
#include <liFluoroscopyCArmMobileGUI.h>
#include <liFluoroscopyUnitViewGUI.h>
#include <liVesselsModelGUI.h>

namespace li {


/**
 *  SpineInterventionConfiguration class put together all the
 *  elements required for image guided spine surgery.
 *  Among the elements there are: a Table, a Fluorosocopy unit, 
 *  Ligths, a Patient model with a 3D spine model composed of
 *  a number of vertebral segments.
 */

class ITK_EXPORT SpineInterventionConfiguration : public itk::Object
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef SpineInterventionConfiguration   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef ::itk::Object  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Fluoroscopy Image Type
   */
  typedef li::FluoroscopyUnitView::ImageType  ImageType;


  /** 
   * Volume Image Type
   */
  // typedef itk::Image<unsigned short, 3> VolumeType;
  typedef li::VertebralSegmentRegistrator::VolumeImageType VolumeType;


  /** 
   * Fluorocopy Image Reader Type
   */
  typedef itk::ImageFileReader< ImageType >   ImageReaderType;


  /** 
   * Volume Image Reader Type
   */
  typedef itk::ImageFileReader< VolumeType >   VolumeReaderType;


  /** 
   * Volume Slicer
   */
  typedef fltk::Slice3DDrawer<VolumeType>         VolumeSlicerType;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( SpineInterventionConfiguration, ::itk::Object );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Draw the model using OpenGL commands
   */
	void DrawGeometry(void) const;


  /**
   * Show the graphic interface
   */
	void Show(void);

  /**
   * Show the graphic interface of the Room View
   */
	void ShowRoomView(void);


  /**
   * Show the graphic interface of the Fluoroscopy View
   */
	void ShowFluoroscopyView(void);


  /**
   * Show the fluoroscopy unit controls
   */
	void ShowFluoroscopyControls(void);


  /**
   * Show the graphic interface
   */
	void ShowTableControls(void);


  /**
   * Show the Spine Model controls
   */
	void ShowSpineControls(void);


  /**
   * Show the Vessels Model controls
   */
	void ShowVesselsControls(void);


  /**
   * Show the graphic interface of the Volume View
   */
	void ShowVolumeView(void);


  /**
   * Show the graphic interface of the Volume Slicer
   */
	void ShowVolumeSlicer(void);


  /**
   * Load a fluoroscopic image
   */
	void LoadFluoroscopy(void);


  /**
   * Load a fluoroscopic image
   */
	void LoadFluoroscopy( const char * filename );


  /**
   * Load a volume image
   */
	void LoadVolume(void);


  /**
   * Load a volume image
   */
	void LoadVolume( const char * filename );


  /**
   * Load a Vessel Model
   */
	void LoadVesselsModel(void);


  /**
   * Load a Vessel Model
   */
	void LoadVesselsModel(const char * filename);


  /**
   * Load a Spine Model
   */
	void LoadSpineModel(void);

  /**
   * Save a Spine Model
   */
	void SaveSpineModel(void);


  /**
   * Load a fluoroscopic image
   */
	void LoadSpineModel( const char * filename );

  /**
   * Save a Spine Model
   */
	void SaveSpineModel( const char * filename );



  /**
   * Hide the graphic interface
   */
	void Hide(void);


  /**
   * Destructor
   */
	~SpineInterventionConfiguration();


protected:
  /**
   * Constructor
   */
	SpineInterventionConfiguration();

private:

  /**
   * Model for the Operating Room
   */
  li::OperatingRoomModel::Pointer     m_OperatingRoomModel; 

   
  /**
   * Surgery Table Graphic Interface
   */
  SurgeryTableGUI                        m_TableGUI;
 

  /**
   * Spine Model Graphic Interface
   */
  SpineModelGUI                          m_SpineGUI;
 

  /**
   * Vessels Model Graphic Interface
   */
  VesselsModelGUI                          m_VesselsGUI;
 


  /**
   * Fluoroscopy Unit Graphic Interface
   */
  FluoroscopyCArmMobileGUI               m_FluoroscopyUnitGUI;
 

  /**
   * View of the Operating Room generated with OpenGL
   */
  OperatingRoomViewGUI                   m_OperatingRoomViewGUI;
 

  /**
   * View from the fluoroscopy unit generated with OpenGL
   */
  FluoroscopyUnitViewGUI                 m_FluoroscopyUnitViewGUI;
 

  /**
   * View of the Volume in 3D
   */
  fltkDisplayGlWindowGUI                 m_VolumeViewGUI;
 
  /**
   * Drawer that displays orthogonal slices from the volume
   */
  VolumeSlicerType::Pointer             m_VolumeSliceDrawer;
  
   /**
   * Fluoroscopy Image
   */
  ImageType::Pointer                     m_FluoroscopyImage;

  /**
   * Fluoroscopy Image Reader
   */
  ImageReaderType::Pointer               m_FluoroscopyImageReader;


  /**
   * Volume Image
   */
  VolumeType::Pointer                    m_VolumeImage;


  /**
   * Volume Image Reader
   */
  VolumeReaderType::Pointer               m_VolumeImageReader;

 
};


} // end namespace li

#endif   
