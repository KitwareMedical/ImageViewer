
# Uncomment the following lines to compile this using CMake
#   if you move this outside of the Insight directory structure
### HERE:  Modified this line to give an appropriate name
#PROJECT(ITKRegistrationLib)

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
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/Numerics
)

LINK_LIBRARIES (
VXLNumerics
ITKNumerics
ITKCommon
ITKIO
ITKBasicFilters 
)


### HERE:  Modified this line
ADD_LIBRARY(Example_ITKRegistrationLib ITKRegistrationLib )

LINK_LIBRARIES (
Example_ITKRegistrationLib
)

ADD_EXECUTABLE(Example_ITKRegistrationLibTest ITKRegistrationLibTest)