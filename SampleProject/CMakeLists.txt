PROJECT(SampleProject)

# Find ITK
INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)

# Use ITK.  This sets up include/link directories for us.
IF (USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

# Add the executable with our source file.
ADD_EXECUTABLE(itkSampleProject itkSampleProject.cxx)

# We only need ITK's Common components.  Link to the common libraries.
TARGET_LINK_LIBRARIES(itkSampleProject ${ITK_Common_LIBS})
