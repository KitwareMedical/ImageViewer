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


# Add an option to build or not build the Image Registration example
IF(HAS_METAIMAGE)
  IF(HAS_FLTKIMAGEVIEWER)
    SUBDIRS(ImageRegistration)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the Multiresolution Registration example
SUBDIRS(MultiResMIRegistration)

# Add an option to build or not build the Gaussian Filter GUI example
IF(HAS_METAIMAGE)
  IF(HAS_FLTKIMAGEVIEWER)
    SUBDIRS(GaussianFilter)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the Extractor of 2D curves
IF(HAS_METAIMAGE)
  IF(HAS_FLTKIMAGEVIEWER)
    SUBDIRS(Curves2DExtractor)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_METAIMAGE)


# Add an option to build or not build the Image Guided Intervention example
IF(HAS_METAIMAGE)
  IF(HAS_FLTKIMAGEVIEWER)
    SUBDIRS(OperatingRoom)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the PrincipalAxes example
SUBDIRS(PrincipalAxes)

# Add an option to build or not build the Morphogenesis example
IF(HAS_METAIMAGE)
  IF(HAS_FLTKIMAGEVIEWER)
    SUBDIRS(Morphogenesis)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the MRIBiasCorrection example
IF(HAS_METAIMAGE)
  IF(HAS_FLTKIMAGEVIEWER)
    SUBDIRS(MRIBiasCorrection)
  ENDIF(HAS_FLTKIMAGEVIEWER)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the FuzzyConnectedness example
SUBDIRS(FuzzyConnectedness)

# Add an option to build or not build the ShapeDetection example
SUBDIRS(ShapeDetection)

# Add an option to build or not build the MRI Registration example
SUBDIRS(MRIRegistration)

# Add an option to build or not build the TripleTKImageDisplay example
IF(HAS_METAIMAGE)
  SUBDIRS(TripleTKImageDisplay)
ENDIF(HAS_METAIMAGE)

# Add an option to build or not build the EllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(EllipsoidInteriorExteriorSpatialFunction)

# Add an option to build or not build the SymmetricEllipsoidInteriorExteriorSpatialFunction example
SUBDIRS(SymmetricEllipsoidInteriorExteriorSpatialFunction)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

