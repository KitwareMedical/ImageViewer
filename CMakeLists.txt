INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
)


# Add an option to build or not build the Image Registration example
OPTION(BUILD_IMAGE_REGISTRATION "Build the image registration example")
IF(BUILD_IMAGE_REGISTRATION)
SUBDIRS(ImageRegistration)
ENDIF(BUILD_IMAGE_REGISTRATION)

# Add an option to build or not build the Multiresolution Registration example
OPTION(BUILD_MULTIRESOLUTION "Build the multiresolution example")
IF(BUILD_MULTIRESOLUTION)
SUBDIRS(MultiResMIRegistration)
ENDIF(BUILD_MULTIRESOLUTION)

# Add an option to build or not build the Gaussian Filter GUI example
OPTION(BUILD_GAUSSIANFILTER "Build the Gaussian filter example")
IF(BUILD_GAUSSIANFILTER)
  IF(HAS_METAIMAGE)
    SUBDIRS(GaussianFilter)
  ENDIF(HAS_METAIMAGE)
ENDIF(BUILD_GAUSSIANFILTER)

# Add an option to build or not build the Image Guided Intervention example
OPTION(BUILD_IMAGE_GUIDED_INTERVENTION "Build the image guided intervention example")
IF(BUILD_IMAGE_GUIDED_INTERVENTION)
SUBDIRS(OperatingRoom)
ENDIF(BUILD_IMAGE_GUIDED_INTERVENTION)

# Add an option to build or not build the PrincipalAxes example
OPTION(BUILD_PRINCIPAL_AXES "Build the principal axes example")
IF(BUILD_PRINCIPAL_AXES)
SUBDIRS(PrincipalAxes)
ENDIF(BUILD_PRINCIPAL_AXES)

# Add an option to build or not build the Morphogenesis example
OPTION(BUILD_MORPHOGENESIS "Build the morphogenesis example")
IF(BUILD_MORPHOGENESIS)
SUBDIRS(Morphogenesis)
ENDIF(BUILD_MORPHOGENESIS)

# Add an option to build or not build the MRIBiasCorrection example
OPTION(BUILD_MRI_BIAS_CORRECTION "Build the MRI bias correction example")
IF(BUILD_MRI_BIAS_CORRECTION)
SUBDIRS(MRIBiasCorrection)
ENDIF(BUILD_MRI_BIAS_CORRECTION)

# Add an option to build or not build the FuzzyConnectedness example
OPTION(BUILD_FUZZY_CONNECTEDNESS "Build the Fuzzy connectedness example")
IF(BUILD_FUZZY_CONNECTEDNESS)
SUBDIRS(FuzzyConnectedness)
ENDIF(BUILD_FUZZY_CONNECTEDNESS)

# Add an option to build or not build the ShapeDetection example
OPTION(BUILD_SHAPE_DETECTION "Build the Shape Detection example")
IF(BUILD_SHAPE_DETECTION)
SUBDIRS(ShapeDetection)
ENDIF(BUILD_SHAPE_DETECTION)

# Add an option to build or not build the MRI Registration example
OPTION(BUILD_MRI_REGISTRATION "Build the MRI Registration example")
IF(BUILD_MRI_REGISTRATION)
SUBDIRS(MRIRegistration)
ENDIF(BUILD_MRI_REGISTRATION)

# Add an option to build or not build the TripleTKImageDisplay example
OPTION(BUILD_TRIPLETKID "Build the Triple TK Image Display example")
IF(BUILD_TRIPLETKID)
SUBDIRS(TripleTKImageDisplay)
ENDIF(BUILD_TRIPLETKID)


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

