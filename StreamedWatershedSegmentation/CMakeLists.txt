PROJECT( StreamedWatershedSegmentation )

SUBDIRS(
ParameterFileParser
VolumeChunker
)

INCLUDE_DIRECTORIES(
${StreamedWatershedSegmentation_SOURCE_DIR}
${StreamedWatershedSegmentation_SOURCE_DIR}/ParameterFileParser
${StreamedWatershedSegmentation_SOURCE_DIR}/VolumeChunker
)


ADD_EXECUTABLE(itkTreeGeneratorApp itkTreeGeneratorApp.cxx)
ADD_EXECUTABLE(itkSegmenterApp itkSegmenterApp.cxx)
ADD_EXECUTABLE(itkResolverApp itkResolverApp.cxx)
ADD_EXECUTABLE(itkEquivalenceRelabelerApp itkEquivalenceRelabelerApp.cxx)
ADD_EXECUTABLE(itkRelabelerApp itkRelabelerApp.cxx)


TARGET_LINK_LIBRARIES (itkTreeGeneratorApp ITKBasicFilters ITKCommon
 param chunks)

TARGET_LINK_LIBRARIES (itkSegmenterApp ITKBasicFilters ITKCommon
 param chunks)

TARGET_LINK_LIBRARIES (itkResolverApp ITKBasicFilters ITKCommon
 param chunks)

TARGET_LINK_LIBRARIES (itkEquivalenceRelabelerApp ITKBasicFilters ITKCommon
 param chunks)

TARGET_LINK_LIBRARIES (itkRelabelerApp ITKBasicFilters ITKCommon
 param chunks)

