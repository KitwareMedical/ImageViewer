CMAKE_MINIMUM_REQUIRED(VERSION 1.6)
PROJECT(InsightApplications)
INCLUDE_REGULAR_EXPRESSION("^.*$")

# Tell CMake's FIND_PACKAGE command where to look for our Find<pkg>
# modules.
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${InsightApplications_SOURCE_DIR})

IF(NOT WIN32)
  OPTION(BUILD_SHARED_LIBS "Build Insight-Applications with shared libraries." OFF)
ENDIF(NOT WIN32)

# Don't build anything unless the version of CMake is high enough.
# The matching ELSE/ENDIF should be the last lines in the file.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.5)

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build InsightApplications without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

#-----------------------------------------------------------------------------
# Configure Dart testing support.
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH)
IF(BUILD_TESTING)
  ENABLE_TESTING()
ENDIF(BUILD_TESTING)

# Determine what packages are available
# Add an option to use or not use VTK
OPTION(USE_VTK "Use VTK (The Visualization Toolkit) (some applications need this)" OFF)
IF(USE_VTK)
  FIND_PACKAGE(VTK)
  IF (VTK_FOUND)
    INCLUDE (${VTK_USE_FILE})

    # Test for VTK using std libs
    IF(NOT VTK_USE_ANSI_STDLIB)
      MESSAGE("Warning.  Your VTK was not built with the VTK_USE_ANSI_STDLIB "
              "option ON.  Link errors may occur.  Consider re-building VTK "
              "with this option ON.  For MSVC 6, you MUST turn on the VTK "
              "option.  ITK will not link otherwise.")
    ENDIF(NOT VTK_USE_ANSI_STDLIB)

    # Test for VTK building Hybrid
    IF(NOT VTK_USE_HYBRID)
      MESSAGE("Your VTK was not built with the VTK_USE_HYBRID option ON.  "
              "Please reconfigure and recompile VTK with this option before "
              "trying to use it with ITK.")
      SET(VTK_FOUND 0)
    ENDIF(NOT VTK_USE_HYBRID)
  ENDIF (VTK_FOUND)
ENDIF(USE_VTK)

# Look for OpenGL.
FIND_PACKAGE(OpenGL)

IF(OPENGL_INCLUDE_PATH)
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_PATH})
ENDIF(OPENGL_INCLUDE_PATH)

# Add an option to use or not use FLTK (http://www.fltk.org)
OPTION(USE_FLTK "Use FLTK (The Fast Light Toolkit) for GUI (some applications need this)" OFF)
IF(USE_FLTK)
  FIND_PACKAGE(FLTK)
  IF(FLTK_FOUND)
    INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
  ENDIF(FLTK_FOUND)
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
IF(FLTK_FOUND)
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
  #SUBDIRS(OperatingRoom)
  IF(VTK_FOUND)
    SUBDIRS(TripleTKImageDisplay)
    SUBDIRS(RegionGrowingSegmentation)
    SUBDIRS(ThresholdSegmentationLevelSet)
    SUBDIRS(FastMarchingLevelSet)
    SUBDIRS(ShapeDetectionLevelSet)
    SUBDIRS(GeodesicActiveContour)
  ENDIF(VTK_FOUND)
  SUBDIRS(MetaImageViewer)
  SUBDIRS(MetaImageColorViewer)
  SUBDIRS(SurfaceGenerator)
ENDIF(FLTK_FOUND)

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

# Add an option to build or not build the
# GaussianMinimumErrorClassifier example
SUBDIRS(GaussianMinimumErrorClassifier)

# Add an option to build or not build the EllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(EllipsoidInteriorExteriorSpatialFunction)

# Add an option to build or not build the SymmetricEllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(SymmetricEllipsoidInteriorExteriorSpatialFunction)

# Test the conditions for building the ThinPlateSplines example
IF(VTK_FOUND)
  IF(FLTK_FOUND)
    SUBDIRS(ThinPlateSplines)
  ENDIF(FLTK_FOUND)

  IF(VTK_WRAP_TCL)
    SUBDIRS(MRIRegistration vtkITK SegmentationEditor)
  ENDIF(VTK_WRAP_TCL)
ENDIF(VTK_FOUND)

LINK_LIBRARIES (
ITKAlgorithms
VXLNumerics
ITKCommon
ITKBasicFilters 
)

# If the version of CMake was too old, complain and build nothing.
# These should be the last lines in this file.
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.5)
  MESSAGE(SEND_ERROR
          "This version of CMake is too old to build InsightApplications.  "
          "Please upgrade to CMake 1.6.")
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.5)
