INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/IO
)

LINK_DIRECTORIES(${ITK_BINARY_DIR}/Utilities/png)
LINK_DIRECTORIES(${ITK_BINARY_DIR}/Utilities/zlib)

SOURCE_FILES(RawImageReadWrite_SRCS
itkRawImageReadWrite
)

SOURCE_FILES(RawImageReadWritePNG_SRCS
itkRawImageReadWritePNG
)


LINK_LIBRARIES (
ITKCommon
ITKIO
itkpng
itkzlib
)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(RawImageReadWrite      RawImageReadWrite_SRCS)
ADD_EXECUTABLE(RawImageReadWritePNG   RawImageReadWritePNG_SRCS)


