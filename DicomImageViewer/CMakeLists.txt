INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

SET ( DicomImageViewer_SRCS
  DicomImageViewer.cxx
  )

INCLUDE_DIRECTORIES (
  ${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${FLTK_INCLUDE_PATH}
  ${OPENGL_INCLUDE_PATH}
)

LINK_LIBRARIES (
  ITKBasicFilters
  ITKIO
  ITKFltkImageViewer
  ${FLTK_LIBRARY}
  ${OPENGL_LIBRARY}
  ${GLU_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE( DicomImageViewer ${DicomImageViewer_SRCS} )


