
SET ( MetaImageColorViewer_SRCS
  MetaImageColorViewer.cxx
  )

INCLUDE_DIRECTORIES (
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
)


IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE( MetaImageColorViewer ${MetaImageColorViewer_SRCS} )
TARGET_LINK_LIBRARIES (MetaImageColorViewer
  ITKBasicFilters
  ITKFltkImageViewer
  ITKIO
)

SET(MetaImageColorViewer_GUI_SRCS
  MetaImageColorViewerGUI.fld
  )

FLTK_WRAP_UI( MetaImageColorViewer ${MetaImageColorViewer_GUI_SRCS} )
