INCLUDE_REGULAR_EXPRESSION("^.*$")


INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
IF (USE_ITK_FILE)
  INCLUDE (${USE_ITK_FILE})
ENDIF (USE_ITK_FILE)

# Determine what packages are available
# Add an option to use or not use VTK
OPTION(USE_VTK "Use VTK (The Visualization Toolkit) (some applications need this)" 0)
IF(USE_VTK)
  INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
  IF (USE_VTK_FILE)
    INCLUDE (${USE_VTK_FILE})
      SET(HAS_VTK 1)
    # Test for VTK using std libs
    IF(NOT VTK_USE_ANSI_STDLIB)
      MESSAGE(
	"Warning. Your VTK was not built with the VTK_USE_ANSI_STDLIB option ON.\n"
        "Link errors may occur.  Consider re-building VTK with this option ON.\n"
        "For MSVC 6, you MUST turn on the VTK option.  ITK will not link otherwise.")
    ENDIF(NOT VTK_USE_ANSI_STDLIB)
    # Test for VTK building Hybrid
    IF(VTK_USE_HYBRID)
      SET(HAS_VTK_HYBRID 1 CACHE INTERNAL "VTK Hybrid libraries are available")
    ELSE( VTK_USE_HYBRID )
      MESSAGE ("Warning. Your VTK was not built with the VTK_USE_HYBRID option ON. \nPlease reconfigure and recompile VTK with this option before trying to use it with ITK.")
    ENDIF( VTK_USE_HYBRID )
  ELSE (USE_VTK_FILE)
    MESSAGE("The UseVTK.cmake file was not found. You will not be able to build code requiring VTK")
    SET(HAS_VTK 0 CACHE INTERNAL "Use VTK: The Visualization Toolkit, this is required for some applications but not for the ITK toolkit")
  ENDIF (USE_VTK_FILE)
ELSE(USE_VTK)
  SET(HAS_VTK 0)
ENDIF(USE_VTK)

# Look for OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

# Look for Tcl
INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)

# Find X11 (This should be done by FindFLTK...
IF(UNIX)
  INCLUDE (${CMAKE_ROOT}/Modules/FindX11.cmake)
  SET(CMAKE_X_LIBS ${X11_LIBRARIES})
ENDIF(UNIX)


# Add an option to use or not use FLTK (http://www.fltk.org)
OPTION(USE_FLTK "Use FLTK (The Fast Light Toolkit) for GUI (some applications need this)" 0)
IF(USE_FLTK)
  INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
  IF(FLTK_VERSION_1.0.11)
    MESSAGE(SEND_ERROR "FLTK Version 1.0.11 is no longer supported in ITK. Please upgrade to FLTK version 1.1")
  ENDIF(FLTK_VERSION_1.0.11)
ELSE(USE_FLTK)
  SET (HAS_FLTK 0)
ENDIF(USE_FLTK)

#Build the Auxiliary directory on which many applications depend
SUBDIRS(Auxiliary)

#Build the example of how to do simple scalar anisotropic diffusion
#on a PNG image
SUBDIRS (AnisotropicDiffusionImageFilter)

#Build the example of how to do simple feature extraction of gradient edges
#on a PNG image
SUBDIRS (GradientMagnitudeImageFilter)

#Build the example of various methods of segmentation using level sets.
SUBDIRS (LevelSetSegmentation)

#Build the example of how to create a static library for 
# select filters in itk.
SUBDIRS (ITKFilterLib)

#Build the example of how to create a static library for 
# the multi-resolution mutual information registration framework in itk.
SUBDIRS (ITKRegistrationLib)

# Build the BasicIOAndFilter example
SUBDIRS(BasicIOAndFilter)

#Build the parameter file parser necessary for several applications that follow
SUBDIRS(StreamedWatershedSegmentation/ParameterFileParser)

# Build the StreamedWatershedSegmentation example
#SUBDIRS(StreamedWatershedSegmentation)

# Build the AntiAliasBinaryImageFilter example
SUBDIRS(AntiAliasBinaryImageFilter)

# Build the WatershedSegmentation example
SUBDIRS(WatershedSegmentation)

# Build the SimpleLevelSetsExample
SUBDIRS(SimpleLevelSetsExample)

# Build the FloodFilledSpatialFunctionExample
SUBDIRS(FloodFilledSpatialFunction)

# Build the Multiresolution Registration example
SUBDIRS(MultiResMIRegistration)

# Build the MI Validation application
SUBDIRS(MIValidation)

# Build the IBSR Validation applications
SUBDIRS(IBSRValidation)

# Build the multichannel tissue classification validation
SUBDIRS(MultichannelTissueClassificationValidation)

# Build the RawImageReadWrite example
SUBDIRS(RawImageReadWrite)

# Build the PNGImageReadWrite example
SUBDIRS(PNGImageReadWrite)

# Build the ExtractImageRegion example
SUBDIRS(ExtractImageRegion)

# Build the DICOMApp example to output raw and png versions of DICOM file
SUBDIRS(DICOMApp)

# Build the simple ImageViewer example. This requires GLUT to be installed
IF(GLUT_FOUND)
  SUBDIRS(ImageViewer)
ENDIF(GLUT_FOUND)
  
# Applications requiring MetaImages and Fltk GUI 
IF(HAS_FLTK)
  IF(HAS_FLTKIMAGEVIEWER)
    IF(WIN32)
      ADD_DEFINITIONS(-DWIN32)
    ENDIF(WIN32)
    SUBDIRS(AnisotropicDiffusionFltkGui)
    SUBDIRS(DistanceMapFilter)
    SUBDIRS(DicomImageViewer)
    SUBDIRS(GaussianFilter)
    SUBDIRS(GradientRecursiveGaussianFilter)
    SUBDIRS(Curves2DExtractor)
    SUBDIRS(Morphogenesis)
    SUBDIRS(MutualInformationEuler2DRegistration)
    SUBDIRS(ImageRegistration)
    SUBDIRS(ImageRegistration2D)
    SUBDIRS(RawImageReaderViewer)
    SUBDIRS(VoronoiSegmentation)
    SUBDIRS(SimpleFuzzyConnectedness)
#   SUBDIRS(OperatingRoom)
    IF(HAS_VTK)
      SUBDIRS(TripleTKImageDisplay)
      SUBDIRS(RegionGrowingSegmentation)
      SUBDIRS(ThresholdSegmentationLevelSet)
      SUBDIRS(FastMarchingLevelSet)
      SUBDIRS(ShapeDetectionLevelSet)
      SUBDIRS(GeodesicActiveContour)
    ENDIF(HAS_VTK)
    SUBDIRS(MetaImageViewer)
    SUBDIRS(MetaImageColorViewer)
    SUBDIRS(SurfaceGenerator)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_FLTK)

# MetaImage Utilities
SUBDIRS(MetaImageImporter)
SUBDIRS(MetaImageReadWrite)

# Test the conditions for building the MRIBiasCorrection example
SUBDIRS(MRIBiasCorrection)


# Add an option to build FEM applications
SUBDIRS(FEM)

# Add an option to build or not build the FuzzyConnectedness example
SUBDIRS(FuzzyConnectedness)

# Add an option to build or not build the ShapeDetection example
SUBDIRS(ShapeDetection)

# The MRI Registration example needs VTK with tcl wrapping turned on.
IF (HAS_VTK)
  SUBDIRS(MRIRegistration)
ENDIF (HAS_VTK)

# Add an option to build or not build the
# GaussianMinimumErrorClassifier example
SUBDIRS(GaussianMinimumErrorClassifier)

# Add an option to build or not build the EllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(EllipsoidInteriorExteriorSpatialFunction)

# Add an option to build or not build the SymmetricEllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(SymmetricEllipsoidInteriorExteriorSpatialFunction)



# Test the conditions for building the ThinPlateSplines example
IF(HAS_VTK)
  IF(HAS_FLTK)
    IF(HAS_VTK_FLTK_WINDOW_INTERACTOR)
      SUBDIRS(ThinPlateSplines)
    ENDIF(HAS_VTK_FLTK_WINDOW_INTERACTOR)
  ENDIF(HAS_FLTK)
ENDIF(HAS_VTK)

IF(HAS_VTK)
  IF (VTK_WRAP_TCL)
    SUBDIRS(vtkITK)
  ENDIF (VTK_WRAP_TCL)
ENDIF(HAS_VTK)


IF(HAS_VTK)
  IF (VTK_WRAP_TCL)
    SUBDIRS(SegmentationEditor)
  ENDIF (VTK_WRAP_TCL)
ENDIF(HAS_VTK)

LINK_LIBRARIES (
ITKAlgorithms
VXLNumerics
ITKCommon
ITKBasicFilters 
)

