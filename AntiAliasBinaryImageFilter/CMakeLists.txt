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
ITKBasicFilters
ITKCommon
VXLNumerics
ITKIO
param
)

ADD_EXECUTABLE(itkAntiAliasBinaryImageFilterExample itkAntiAliasBinaryImageFilterExample)
ADD_EXECUTABLE(itkSampleDataGenerator itkSampleDataGenerator)
