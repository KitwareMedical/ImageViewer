INCLUDE_REGULAR_EXPRESSION("^.*$")

# Look for OpenGL
INCLUDE (${ITK_SOURCE_DIR}/Utilities/ITKFindOpenGL.cmake)

#Build the example of how to create a static library for 
# select filters in itk.
SUBDIRS (ITKFilterLib)

#Build the example of how to create a static library for 
# the multi-resolution mutual information registration framework in itk.
SUBDIRS (ITKRegistrationLib)

# Build the BasicIOAndFilter example
SUBDIRS(BasicIOAndFilter)

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

# Build the multichannel tissue classification validation
SUBDIRS(MultichannelTissueClassificationValidation)

# Build the RawImageReadWrite example
SUBDIRS(RawImageReadWrite)

# Build the PNGImageReadWrite example
SUBDIRS(PNGImageReadWrite)

# Build the ExtractImageRegion example
SUBDIRS(ExtractImageRegion)

# Examples requiring MetaImages and Fltk GUI 
IF(HAS_FLTK)
  IF(HAS_FLTKIMAGEVIEWER)
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
#   SUBDIRS(OperatingRoom)
    IF(HAS_VTK)
      SUBDIRS(TripleTKImageDisplay)
      SUBDIRS(RegionGrowingSegmentation)
      SUBDIRS(ThresholdSegmentationLevelSet)
      SUBDIRS(FastMarchingLevelSet)
      SUBDIRS(ShapeDetectionLevelSet)
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
#SUBDIRS(MRIBiasCorrection)


# Add an option to build FEM examples
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

