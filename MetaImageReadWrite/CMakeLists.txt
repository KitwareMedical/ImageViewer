INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

SOURCE_FILES ( MetaImageReadWrite_SRCS
  MetaImageReadWrite
  )

LINK_LIBRARIES (
  ITKCommon
  ITKBasicFilters
  VXLNumerics
  ITKIO
  MetaIO
)

IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE( MetaImageReadWrite MetaImageReadWrite_SRCS )
