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


ADD_EXECUTABLE(itkTreeGeneratorApp itkTreeGeneratorApp)
ADD_EXECUTABLE(itkSegmenterApp itkSegmenterApp)
ADD_EXECUTABLE(itkResolverApp itkResolverApp)
ADD_EXECUTABLE(itkEquivalenceRelabelerApp itkEquivalenceRelabelerApp)
ADD_EXECUTABLE(itkRelabelerApp itkRelabelerApp)


TARGET_LINK_LIBRARIES (itkTreeGeneratorApp ITKBasicFilters ITKCommon
VXLNumerics param chunks)

TARGET_LINK_LIBRARIES (itkSegmenterApp ITKBasicFilters ITKCommon
VXLNumerics param chunks)

TARGET_LINK_LIBRARIES (itkResolverApp ITKBasicFilters ITKCommon
VXLNumerics param chunks)

TARGET_LINK_LIBRARIES (itkEquivalenceRelabelerApp ITKBasicFilters ITKCommon
VXLNumerics param chunks)

TARGET_LINK_LIBRARIES (itkRelabelerApp ITKBasicFilters ITKCommon
VXLNumerics param chunks)

