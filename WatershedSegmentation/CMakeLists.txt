INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Examples/WatershedSegmentationExample
)

LINK_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
)

LINK_LIBRARIES (
ITKBasicFilters
ITKCommon
ITKIO
VXLNumerics
)

ADD_EXECUTABLE(itkWatershedSegmentationExample itkWatershedSegmentationExample)

