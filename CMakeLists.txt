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
SUBDIRS(GaussianFilter)
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

