INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

SOURCE_FILES ( MetaImageViewer_SRCS
  MetaImageViewer
  )

INCLUDE_DIRECTORIES (
  ${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${FLTK_INCLUDE_PATH}
  ${OPENGL_INCLUDE_PATH}
)

LINK_LIBRARIES (
  ITKCommon
  ITKBasicFilters
  VXLNumerics
  FltkImageViewer
  MetaIO
  itkpng
  itkzlib
  ${FLTK_LIBRARY}
  ${OPENGL_LIBRARY}
  ${GLU_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE( MetaImageViewer MetaImageViewer_SRCS )

SOURCE_FILES(MetaImageViewer_GUI_SRCS
  MetaImageViewerGUI.fld
  )

IF(HAS_FLTK)
  FLTK_WRAP_UI( MetaImageViewer MetaImageViewer_GUI_SRCS )
ENDIF(HAS_FLTK)
