EXECUTABLES(
itkFileIOMetaImageExample
)

LINK_LIBRARIES(
MetaImageIO
ITKCommon
ITKBasicFilters
VXLNumerics
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Code/BasicFilters
)

LIBRARY(
MetaImageIO
)

SOURCE_FILES(
MetaFileLib 
MetaImageLib
itkWriteMetaImage 
MetaUtils
itkFileIOMetaImageCommon
itkFileIOMetaImage
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Unsupported/MetaImage
)