SUBDIRS(IView)

LINK_LIBRARIES(
MetaImageIO
ITKCommon
ITKIO
ITKBasicFilters
VXLNumerics
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Auxiliary/MetaImage
${ITK_BINARY_DIR}/Code/BasicFilters
)

SOURCE_FILES(MetaImageIO_SRCS
MetaFileLib 
MetaImageLib
itkWriteMetaImage 
MetaUtils
itkFileIOMetaImageCommon
itkFileIOMetaImage
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
)

ADD_LIBRARY(
MetaImageIO MetaImageIO_SRCS
)

ADD_EXECUTABLE(
itkFileIOMetaImageExample itkFileIOMetaImageExample
)


