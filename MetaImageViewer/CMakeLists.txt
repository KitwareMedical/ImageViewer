INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

SET ( MetaImageViewer_SRCS
  MetaImageViewer.cxx
  )

INCLUDE_DIRECTORIES (
  ${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
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

ADD_EXECUTABLE( MetaImageViewer ${MetaImageViewer_SRCS} )

SET(MetaImageViewer_GUI_SRCS
  MetaImageViewerGUI.fld
  )

IF(HAS_FLTK)
  FLTK_WRAP_UI( MetaImageViewer ${MetaImageViewer_GUI_SRCS} )
ENDIF(HAS_FLTK)
