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


ADD_EXECUTABLE(itkTreeGeneratorApp itkTreeGeneratorApp 
ITKBasicFilters ITKCommon VXLNumerics param chunks )

ADD_EXECUTABLE(itkSegmenterApp itkSegmenterApp
ITKBasicFilters ITKCommon VXLNumerics param chunks )

ADD_EXECUTABLE(itkResolverApp itkResolverApp
ITKBasicFilters ITKCommon VXLNumerics param chunks )

ADD_EXECUTABLE(itkEquivalenceRelabelerApp itkEquivalenceRelabelerApp
ITKBasicFilters ITKCommon VXLNumerics param chunks )

ADD_EXECUTABLE(itkRelabelerApp itkRelabelerApp
ITKBasicFilters ITKCommon VXLNumerics param chunks )
