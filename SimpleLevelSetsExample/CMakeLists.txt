INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
)

LINK_LIBRARIES (
ITKBasicFilters
ITKCommon
VXLNumerics
ITKIO
)

ADD_EXECUTABLE(itk2DSimpleLevelSetsExample itk2DSimpleLevelSetsExample.cxx)
ADD_EXECUTABLE(itk3DSimpleLevelSetsExample itk3DSimpleLevelSetsExample.cxx)


