INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Utilities/png
)

LINK_LIBRARIES (
ITKBasicFilters
ITKCommon
ITKIO
VXLNumerics
itkpng
itkzlib
)

ADD_EXECUTABLE(itkWatershedSegmentationExample itkWatershedSegmentationExample)
ADD_EXECUTABLE(itkWSRawToPNG itkWSRawToPNG)
