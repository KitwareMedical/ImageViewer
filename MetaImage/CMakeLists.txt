IF(HAS_FLTK)
  SUBDIRS(IView)
ENDIF(HAS_FLTK)

LINK_LIBRARIES(
MetaImageIO
ITKCommon
ITKIO
ITKBasicFilters
VXLNumerics
)


SET(MetaImageIO_SRCS
MetaFileLib.cxx
MetaImageLib.cxx
itkWriteMetaImage.cxx 
MetaUtils.cxx
itkFileIOMetaImageCommon.cxx
itkFileIOMetaImage.cxx
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
)

ADD_LIBRARY(
MetaImageIO ${MetaImageIO_SRCS}
)

SET(itkFileIOMetaImageExample_SRCS
itkFileIOMetaImageExample
)

ADD_EXECUTABLE(
itkFileIOMetaImageExample ${itkFileIOMetaImageExample_SRCS}
)


