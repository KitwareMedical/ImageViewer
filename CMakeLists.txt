INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
)

#Build the example of how to create a static library for 
# select filters in itk.
SUBDIRS (ITKFilterLib)

#Build the example of how to create a static library for 
# the multi-resolution mutual information registration framework in itk.
SUBDIRS (ITKRegistrationLib)

#Build the parameter file parser necessary for several examples that follow
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

# Build the RawImageReadWrite example
SUBDIRS(RawImageReadWrite)

# Examples requiring MetaImages and Fltk GUI 
IF(HAS_METAIMAGE)
  IF(HAS_FLTK)
    IF(HAS_FLTKIMAGEVIEWER)
      SUBDIRS(DistanceMapFilter)
      SUBDIRS(GaussianFilter)
      SUBDIRS(GradientRecursiveGaussianFilter)
      SUBDIRS(Curves2DExtractor)
      SUBDIRS(Morphogenesis)
      SUBDIRS(MutualInformationEuler2DRegistration)
      SUBDIRS(ImageRegistration)
      SUBDIRS(ImageRegistration2D)
      SUBDIRS(RawImageReaderViewer)
#     SUBDIRS(OperatingRoom)
      IF(HAS_VTK)
			  SUBDIRS(TripleTKImageDisplay)
      ENDIF(HAS_VTK)
      SUBDIRS(SurfaceGenerator)
    ENDIF(HAS_FLTKIMAGEVIEWER)
  ENDIF(HAS_FLTK)
ENDIF(HAS_METAIMAGE)


# Test the conditions for building the MRIBiasCorrection example
IF(HAS_METAIMAGE)
  SUBDIRS(MRIBiasCorrection)
ENDIF(HAS_METAIMAGE)


# Add an option to build FEM examples
IF(HAS_FEM)
  SUBDIRS(FEM)
ENDIF(HAS_FEM)

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
IF(HAS_METAIMAGE)
  SUBDIRS(GaussianMinimumErrorClassifier)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the EllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(EllipsoidInteriorExteriorSpatialFunction)

# Add an option to build or not build the SymmetricEllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(SymmetricEllipsoidInteriorExteriorSpatialFunction)


# Add an option to build or not build the FEM - Mesh example
OPTION(BUILD_FEM_MESH_EXAMPLE "Build the FEM-Mesh example" OFF)
IF(BUILD_FEM_MESH_EXAMPLE)
  IF(HAS_FEM)
    IF(HAS_FLTKIMAGEVIEWER)
      IF(HAS_VTK)
        IF(HAS_VTK_FLTK_WINDOW_INTERACTOR)
          SUBDIRS(FEMMesh)
        ENDIF(HAS_VTK_FLTK_WINDOW_INTERACTOR)
      ENDIF(HAS_VTK)
    ENDIF(HAS_FLTKIMAGEVIEWER)
  ENDIF(HAS_FEM)
ENDIF(BUILD_FEM_MESH_EXAMPLE)
MARK_AS_ADVANCED( BUILD_FEM_MESH_EXAMPLE )
  

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

# Add an option to build the Tcl Wrap for the stl string class
IF( ITK_WRAP_TCL )
  IF( GCCXML )
    IF( CABLE )
      IF( TCL_TCLSH )
        SUBDIRS(TclWrappedSTLString)
      ENDIF( TCL_TCLSH )
    ENDIF( CABLE )
  ENDIF( GCCXML )
ENDIF( ITK_WRAP_TCL )


# Add an option to build the Tcl Wrap for the itk::Image class
IF( ITK_WRAP_TCL )
  IF( GCCXML )
    IF( CABLE )
      IF( TCL_TCLSH )
        SUBDIRS(TclWrappedImage)
      ENDIF( TCL_TCLSH )
    ENDIF( CABLE )
  ENDIF( GCCXML )
ENDIF( ITK_WRAP_TCL )


# Add an option to build the Tcl Wrap for the GaussianFilter example
IF( ITK_WRAP_TCL )
  IF( GCCXML )
    IF( CABLE )
      IF( TCL_TCLSH )
        SUBDIRS(TclWrappedGaussianFilter)
      ENDIF( TCL_TCLSH )
    ENDIF( CABLE )
  ENDIF( GCCXML )
ENDIF( ITK_WRAP_TCL )



LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
)

IF(UNIX)
  IF (NOT ITK_USE_SYSTEM_STDLIB)
  LINK_LIBRARIES (
  -lCio
  )
  ENDIF(NOT ITK_USE_SYSTEM_STDLIB)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

