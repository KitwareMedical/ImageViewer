PROJECT(SampleProject)

# 
# Find ITK
#
INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
IF (USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

LINK_LIBRARIES (
VXLNumerics
ITKCommon
)


ADD_EXECUTABLE(itkSampleProject itkSampleProject.cxx )

