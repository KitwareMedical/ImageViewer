PROJECT(SampleProject)

# 
# Find ITK
#
FIND_PATH( ITK_BINARY_DIR itkConfigure.h )

# Load in the values from ITK if found
IF ( ITK_BINARY_DIR )
  LOAD_CACHE(${ITK_BINARY_DIR})
  INCLUDE (${ITK_SOURCE_DIR}/itkCMakeOptions.cmake)
ENDIF (ITK_BINARY_DIR )


LINK_LIBRARIES (
VXLNumerics
ITKCommon
)


ADD_EXECUTABLE(itkSampleProject itkSampleProject.cxx )

