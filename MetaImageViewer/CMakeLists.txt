
SET ( MetaImageViewer_SRCS
  MetaImageViewer.cxx
  )

INCLUDE_DIRECTORIES (
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
)

LINK_LIBRARIES (
  ITKBasicFilters
  ITKIO
  ITKFltkImageViewer
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

FLTK_WRAP_UI( MetaImageViewer ${MetaImageViewer_GUI_SRCS} )
