SET(RawImageReadWrite_SRCS
itkRawImageReadWrite.cxx
)

SET(RawImageReadWritePNG_SRCS
itkRawImageReadWritePNG.cxx
)

SET(LIBRARIES ITKIO)

IF(UNIX)
  SET(LIBRARIES ${LIBRARIES} -lm)
ENDIF(UNIX)

ADD_EXECUTABLE(RawImageReadWrite      ${RawImageReadWrite_SRCS})
TARGET_LINK_LIBRARIES(RawImageReadWrite ${LIBRARIES})
ADD_EXECUTABLE(RawImageReadWritePNG   ${RawImageReadWritePNG_SRCS})
TARGET_LINK_LIBRARIES(RawImageReadWritePNG ${LIBRARIES})


