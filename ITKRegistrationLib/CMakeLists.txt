
# Uncomment the following lines to compile this using CMake
#   if you move this outside of the Insight directory structure
### HERE:  Modified this line to give an appropriate name
PROJECT(ITKRegistrationLib)

# 
# Find ITK
#
#FIND_PATH( ITK_BINARY_DIR itkConfigure.h )

# Load in the values from ITK if found
#IF ( ITK_BINARY_DIR )
#  LOAD_CACHE(${ITK_BINARY_DIR})
#  INCLUDE (${ITK_SOURCE_DIR}/itkCMakeOptions.cmake)
#ENDIF (ITK_BINARY_DIR )

#INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${ITKRegistrationLib_SOURCE_DIR}/../MultiResMIRegistration/Common
${ITKRegistrationLib_SOURCE_DIR}/../IBSRValidation/Common
)


### HERE:  Modified this line
ADD_LIBRARY(Example_ITKRegistrationLib ITKRegistrationLib.cxx )
TARGET_LINK_LIBRARIES(Example_ITKRegistrationLib ITKNumerics ITKIO ITKBasicFilters ITKCommon )
ADD_EXECUTABLE(Example_ITKRegistrationLibTest ITKRegistrationLibTest.cxx)
TARGET_LINK_LIBRARIES(Example_ITKRegistrationLibTest Example_ITKRegistrationLib)
