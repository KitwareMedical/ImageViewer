INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Examples/StreamedWatershedSegmentation/ParameterFileParser
)

LINK_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Examples/StreamedWatershedSegmentation/ParameterFileParser
)

LINK_LIBRARIES (
param
ITKBasicFilters
ITKCommon
VXLNumerics
ITKIO
)

ADD_EXECUTABLE(itkAntiAliasBinaryImageFilterExample itkAntiAliasBinaryImageFilterExample.cxx)
ADD_EXECUTABLE(itkSampleDataGenerator itkSampleDataGenerator.cxx)
