PROJECT(SampleProject)

# Find ITK.
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

# Add the executable with our source file.
ADD_EXECUTABLE(itkSampleProject itkSampleProject.cxx)

# We only need ITK's Common components.  Link to the common library.
TARGET_LINK_LIBRARIES(itkSampleProject ITKCommon)
