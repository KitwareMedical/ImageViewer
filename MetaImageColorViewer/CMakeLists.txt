INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

SOURCE_FILES ( MetaImageColorViewer_SRCS
  MetaImageColorViewer.cxx
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
  ITKIO
  MetaIO
  ${FLTK_LIBRARY}
  ${OPENGL_LIBRARY}
  ${GLU_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE( MetaImageColorViewer MetaImageColorViewer_SRCS )

SOURCE_FILES(MetaImageColorViewer_GUI_SRCS
  MetaImageColorViewerGUI.fld
  )

IF(HAS_FLTK)
  FLTK_WRAP_UI( MetaImageColorViewer MetaImageColorViewer_GUI_SRCS )
ENDIF(HAS_FLTK)

