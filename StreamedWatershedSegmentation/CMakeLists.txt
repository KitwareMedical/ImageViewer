SUBDIRS(
ParameterFileParser
VolumeChunker
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Examples/StreamedWatershedSegmentation
${ITK_SOURCE_DIR}/Examples/StreamedWatershedSegmentation/ParameterFileParser
${ITK_SOURCE_DIR}/Examples/StreamedWatershedSegmentation/VolumeChunker
)

LINK_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Examples/StreamedWatershedSegmentation/ParameterFileParser
${ITK_SOURCE_DIR}/Examples/StreamedWatershedSegmentation/VolumeChunker
)

LINK_LIBRARIES (
ITKBasicFilters
ITKCommon
VXLNumerics
chunks
param
)

ADD_EXECUTABLE(itkTreeGeneratorApp itkTreeGeneratorApp)
ADD_EXECUTABLE(itkSegmenterApp itkSegmenterApp)
ADD_EXECUTABLE(itkResolverApp itkResolverApp)
ADD_EXECUTABLE(itkEquivalenceRelabelerApp itkEquivalenceRelabelerApp)
ADD_EXECUTABLE(itkRelabelerApp itkRelabelerApp)
